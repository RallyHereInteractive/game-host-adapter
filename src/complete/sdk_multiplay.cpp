/*
Copyright 2023 RallyHere

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "sdk.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include "rh_format.h"
#include "rh_vector.h"

#if PLATFORM_LINUX
#include <unistd.h>
#include <sys/inotify.h>
#endif

#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "boost/asio.hpp"
#include "boost/url.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/json/monotonic_resource.hpp"

#include "auth.h"

#include "file_watcher.hpp"

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>
using udp = boost::asio::ip::udp; // from <boost/asio/ip/udp.hpp>

#include "sdk_helpers.hpp"
#include "sdk_formatters.hpp"


namespace rallyhere
{
struct MultiplayServer
{
    rallyhere::string allocation_uuid{};
    rallyhere::string session_id{};
    rallyhere::string session_auth{};
    rallyhere::string connection_ip{};
    rallyhere::string connection_port{};
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, MultiplayServer const& req)
{
    jv = {
        { "AllocationUUID", req.allocation_uuid },
        { "SessionId", req.session_id },
    };
    if (!req.session_auth.empty())
        jv.as_object()["SessionAuth"] = req.session_auth;
    if (!req.connection_ip.empty())
        jv.as_object()["ConnectionIP"] = req.connection_ip;
    if (!req.connection_port.empty())
        jv.as_object()["ConnectionPort"] = req.connection_port;
}

MultiplayServer tag_invoke(boost::json::value_to_tag<MultiplayServer>, boost::json::value const& jv)
{
    MultiplayServer req{};
    req.allocation_uuid = jv.at("AllocationUUID").as_string().c_str();
    auto session_id_src = jv.as_object().if_contains("SessionId");
    if (session_id_src && session_id_src->is_string())
    {
        req.session_id = session_id_src->as_string().c_str();
    }
    auto session_auth_src = jv.as_object().if_contains("SessionAuth");
    if (session_auth_src && session_auth_src->is_string())
    {
        req.session_auth = session_auth_src->as_string().c_str();
    }
    auto connection_ip_src = jv.as_object().if_contains("ConnectionIP");
    if (connection_ip_src && connection_ip_src->is_string())
    {
        req.connection_ip = connection_ip_src->as_string().c_str();
    }
    auto connection_port_src = jv.as_object().if_contains("ConnectionPort");
    if (connection_port_src && connection_port_src->is_string())
    {
        req.connection_port = connection_port_src->as_string().c_str();
    }
    return req;
}

void GameInstanceAdapter::SetupMultiplay()
{
    for (auto&& arg : m_Arguments)
    {
        rallyhere::string tmp;
        if (ParseArgument("rhmultiplayfile=", arg, m_MultiplayServerFile))
        {
            continue;
        }
    }
}

void GameInstanceAdapter::ConnectMultiplay(base_callback_function_t callback, void* user_data)
{
    if (callback)
        callback(RH_STATUS_OK, user_data);
}

void GameInstanceAdapter::ReadyMultiplay(base_callback_function_t callback, void* user_data)
{
    // Start watching the server json file
    auto wrapper = [this]() {
        CheckServerJson();
    };
    auto alloc = i3d::one::StandardAllocator<ServerJsonWatcher>{};
    m_MultiplayServerFileWatcher = std::allocate_shared<ServerJsonWatcher>(alloc, net::make_strand(m_IoContext), m_MultiplayServerFile, wrapper, log());
    if (m_MultiplayServerFileWatcher->fail())
    {
        log().log(RH_LOG_LEVEL_ERROR, "Failed to watch server json file");
        auto ec = m_MultiplayServerFileWatcher->error_code();
        RallyHereStatusCode status = RH_STATUS_ERROR;
        if (ec == boost::system::errc::no_such_file_or_directory)
        {
            status = RH_STATUS_NO_SERVER_JSON;
        }
        if (callback)
            callback(status, user_data);
        return;
    }
    m_MultiplayServerFileWatcher->run();
    // Setup A2S
    auto a2s_alloc = i3d::one::StandardAllocator<a2s_listener>{};
    m_A2SListener = std::allocate_shared<a2s_listener>(a2s_alloc, net::make_strand(m_IoContext), m_A2SQueryPort, log(), m_A2SChallenge);
    if (m_A2SListener->fail())
    {
        log().log(RH_LOG_LEVEL_ERROR, "Failed to start A2S listener");
        if (callback)
            callback(RH_STATUS_ERROR, user_data);
        return;
    }
    RallyHereStatsBaseProvided provided{};
    memset(&provided, 0xff, sizeof(provided));
    m_A2SListener->update_server_info(m_StatsBase, provided, [this, callback, user_data]() {
        // Tell them we're done
        if (callback)
            callback(RH_STATUS_OK, user_data);
        // Go ahead and call the method that reads the server json at least once
        CheckServerJson();
    });
    m_A2SListener->run();
}

void GameInstanceAdapter::CheckServerJson()
{
    auto source = LoadFileToString(m_MultiplayServerFile);
    boost::json::error_code ec;
    custom_resource mr;
    auto jv = boost::json::parse(source, ec, &mr);
    if (ec)
    {
        log().log(RH_LOG_LEVEL_ERROR, "parse server json: {} :{}", ec.message(), source);
        if (m_OnAllocatedCallback)
        {
            m_OnAllocatedCallback(nullptr, RH_STATUS_COULD_NOT_PARSE_SERVER_JSON, m_OnAllocatedUserData);
        }
        return;
    }
    try
    {
        MultiplayServer server{ boost::json::value_to<MultiplayServer>(jv) };
        if (!server.session_id.empty())
        {
            m_AllocationId = server.allocation_uuid;
            m_SessionId = server.session_id;
            m_SessionAuth = server.session_auth;
            m_PublicHost = server.connection_ip;
            m_AllocatedPublicPort = server.connection_port;
            m_Allocated = true;
            log().log(RH_LOG_LEVEL_INFO, "Server json parse was successful allocation_id={} session_id={}", m_AllocationId, m_SessionId);
            auto allocation_info = i3d::one::allocator::create<rallyhere::StringMap>();
            allocation_info->Set("allocation_id", m_AllocationId);
            allocation_info->Set("session_id", m_SessionId);
            allocation_info->Set("session_auth", m_SessionAuth);
            allocation_info->Set("public_host", m_PublicHost);
            allocation_info->Set("public_port", m_AllocatedPublicPort);
            if (LastAllocatedGauge)
                LastAllocatedGauge->SetToCurrentTime();
            if (m_OnAllocatedCallback)
                m_OnAllocatedCallback(reinterpret_cast<RallyHereStringMapPtr>(allocation_info), RH_STATUS_OK, m_OnAllocatedUserData);
            else
                i3d::one::allocator::destroy(allocation_info);
            StopCheckingServerJson();
        }
    }
    catch (const std::exception& e)
    {
        log().log(RH_LOG_LEVEL_ERROR, "parse server json structure: {}", e.what());
        m_OnAllocatedCallback(nullptr, RH_STATUS_COULD_NOT_PARSE_SERVER_JSON, m_OnAllocatedUserData);
        StopCheckingServerJson();
    }
}

void GameInstanceAdapter::StopCheckingServerJson()
{
    if (!m_MultiplayServerFileWatcher)
        return;
    m_MultiplayServerFileWatcher->cancel_later();
    m_MultiplayServerFileWatcher.reset();
}

} // namespace rallyhere