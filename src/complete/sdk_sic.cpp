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
#include "boost/algorithm/string.hpp"

#include "auth.h"

#include "file_watcher.hpp"

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

#include "cpp-base64/base64.hpp"

#include "sdk_helpers.hpp"
#include "sdk_formatters.hpp"

namespace boost
{
namespace certify
{
// sni_hostname incorrectly uses static typing, so it can't handle any variation of the stream type even if that
// stream implements the interface
template<class StreamT>
void sni_hostname(StreamT& stream, boost::string_view hostname, boost::system::error_code& ec)
{
    auto ret = SSL_set_tlsext_host_name(stream.native_handle(), hostname.data());
    if (ret == 0)
        ec = { static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
    else
        ec = {};
}
} // namespace certify
} // namespace boost

namespace rallyhere
{
struct SICRegisterRequest
{
    rallyhere::string id{};
    rallyhere::string group_tags{};
    rallyhere::string public_host{};
    int public_port{};
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, SICRegisterRequest const& req)
{
    jv = {
        { "id", req.id },
        { "group_tags", req.group_tags },
        { "public_host", req.public_host },
        { "public_port", req.public_port },
    };
}

SICRegisterRequest tag_invoke(boost::json::value_to_tag<SICRegisterRequest>, boost::json::value const& jv)
{
    SICRegisterRequest req{};
    req.id = jv.at("id").as_string().c_str();
    req.group_tags = jv.at("group_tags").as_string().c_str();
    req.public_host = jv.at("public_host").as_string().c_str();
    req.public_port = jv.at("public_port").as_int64();
    return req;
}

struct SICRegisterRequestModel
{
    rallyhere::vector<SICRegisterRequest> items{};
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, SICRegisterRequestModel const& req)
{
    boost::json::array arr(jv.storage());
    for (auto& item : req.items)
    {
        arr.emplace_back(boost::json::value_from(item));
    }
    jv = {
        { "items", arr },
    };
}

SICRegisterRequestModel tag_invoke(boost::json::value_to_tag<SICRegisterRequestModel>, boost::json::value const& jv)
{
    SICRegisterRequestModel req{};
    auto arr = jv.at("items").as_array();
    for (auto& item : arr)
    {
        req.items.push_back(boost::json::value_to<SICRegisterRequest>(item));
    }
    return req;
}

struct SICBaseModel
{
    bool success{};
    int error_code{};
    rallyhere::string error_message{};

    bool is_ready_to_poll()
    {
        if (success)
            return true;
        // This is the error code on the API that corresponds to us being in the middle of an allocation
        const int REGISTER_ALLOCATING = 6;
        if (error_code == REGISTER_ALLOCATING)
            return true;
        return false;
    }
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, SICBaseModel const& req)
{
    jv = {
        { "success", req.success },
        { "error_code", req.error_code },
        { "error_message", req.error_message },
    };
}

SICBaseModel tag_invoke(boost::json::value_to_tag<SICBaseModel>, boost::json::value const& jv)
{
    SICBaseModel req{};
    req.success = jv.at("success").as_bool();
    req.error_code = jv.at("error_code").as_int64();
    auto error_message_src = jv.as_object().if_contains("error_message");
    if (error_message_src && error_message_src->is_string())
        req.error_message = error_message_src->as_string().c_str();
    return req;
}

struct SICRegisterModel
{
    rallyhere::vector<SICBaseModel> items{};

    bool is_any_ready_to_poll()
    {
        for (auto&& item : items)
        {
            if (item.is_ready_to_poll())
                return true;
        }
        return false;
    }
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, SICRegisterModel const& req)
{
    boost::json::array arr(jv.storage());
    for (auto& item : req.items)
    {
        arr.emplace_back(boost::json::value_from(item));
    }
    jv = {
        { "items", arr },
    };
}

SICRegisterModel tag_invoke(boost::json::value_to_tag<SICRegisterModel>, boost::json::value const& jv)
{
    SICRegisterModel req{};
    auto arr = jv.at("items").as_array();
    for (auto& item : arr)
    {
        req.items.push_back(boost::json::value_to<SICBaseModel>(item));
    }
    return req;
}

struct SICPoll : public SICBaseModel
{
    rallyhere::string state{};
    rallyhere::string allocation_id{};
    rallyhere::string session_id{};
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, SICPoll const& req)
{
    jv = {
        { "success", req.success },
        { "error_code", req.error_code },
        { "error_message", req.error_message },
        { "state", req.state },
        { "allocation_id", req.allocation_id },
        { "session_id", req.session_id },
    };
}

SICPoll tag_invoke(boost::json::value_to_tag<SICPoll>, boost::json::value const& jv)
{
    SICPoll req{};
    req.success = jv.at("success").as_bool();
    req.error_code = jv.at("error_code").as_int64();
    auto error_message_src = jv.as_object().if_contains("error_message");
    if (error_message_src && error_message_src->is_string())
        req.error_message = error_message_src->as_string().c_str();
    req.state = jv.at("state").as_string().c_str();
    auto allocation_id_src = jv.as_object().if_contains("allocation_id");
    if (allocation_id_src && allocation_id_src->is_string())
        req.allocation_id = allocation_id_src->as_string().c_str();
    auto session_id_src = jv.as_object().if_contains("session_id");
    if (session_id_src && session_id_src->is_string())
        req.session_id = session_id_src->as_string().c_str();
    return req;
}

using SICPollOrBase = std::variant<SICPoll, SICBaseModel>;

struct SICPollModel
{
    rallyhere::vector<SICPollOrBase> items{};
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, SICPollModel const& req)
{
    boost::json::array arr(jv.storage());
    for (auto& item : req.items)
    {
        arr.emplace_back(boost::json::value_from(item));
    }
    jv = {
        { "items", arr },
    };
}

SICPollModel tag_invoke(boost::json::value_to_tag<SICPollModel>, boost::json::value const& jv)
{
    SICPollModel req{};
    auto arr = jv.at("items").as_array();
    for (auto& item : arr)
    {
        try
        {
            req.items.push_back(boost::json::value_to<SICPoll>(item));
        }
        catch(const std::exception& e)
        {
            req.items.push_back(boost::json::value_to<SICBaseModel>(item));
        }
    }
    return req;
}

class session : public std::enable_shared_from_this<session>
{
  public:
    using callback_function_t = std::function<void(session&)>;

  public:
    explicit session(net::any_io_executor ex, ssl::context& ssl_ctx, boost::asio::cancellation_slot cancellation_slot) : m_Resolver(ex), m_Stream(ex, ssl_ctx), m_Cancelled{false}, m_CancellationSlot{cancellation_slot}
    {
        m_CancellationSlot.assign([this](boost::asio::cancellation_type_t cancellation_type) { cancel_handler(cancellation_type); });
    }

    ~session()
    {
        m_CancellationSlot.clear();
    }

    bool is_secure()
    {
        if (m_Url.has_scheme())
            return "https" == m_Url.scheme();
        return true;
    }

    void cancel_handler(boost::asio::cancellation_type_t cancellation_type)
    {
        m_Cancelled = true;
        m_Resolver.cancel();
        beast::get_lowest_layer(m_Stream).cancel();
    }

    void run(const rallyhere::string& in_url_str, http::request<string_body>&& request, callback_function_t callback, logger logger, bool log_request)
    {
        m_Logger = logger;
        m_Request = std::move(request);
        m_Callback = callback;
        m_LogRequest = log_request;

        m_SourceUrl = in_url_str;
        auto result = boost::urls::parse_uri(m_SourceUrl);
        if (!result)
        {
            failure(result.error(), "parse_uri");
            return;
        }
        m_Url = *result;

        std::string_view scheme = "https";
        if (m_Url.has_scheme())
        {
            scheme = m_Url.scheme();
        }
        std::string_view service = "https";
        if (m_Url.has_port())
        {
            service = m_Url.port();
        }
        m_Resolver.async_resolve(
            m_Url.encoded_host(),
            service,
            boost::asio::bind_allocator(i3d::one::StandardAllocator<int>{}, boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session::on_resolve, shared_from_this()))));
    }

    template<typename Rep, typename Period>
    void run(const rallyhere::string& in_url_str, http::request<string_body>&& request, callback_function_t callback, logger logger, bool log_request, std::chrono::duration<Rep, Period> timeout)
    {
        m_Timeout = std::chrono::duration_cast<decltype(m_Timeout)>(timeout);
        run(in_url_str, std::move(request), callback, logger, log_request);
    }

    void on_resolve(beast::error_code ec, tcp::resolver::results_type results)
    {
        if (check_cancelled()) return;
        if (ec)
        {
            failure(ec, "resolve");
            return;
        }
        // Set a timeout on the operation
        beast::get_lowest_layer(m_Stream).expires_after(m_Timeout);

        // Make the connection on the IP address we get from a lookup
        beast::get_lowest_layer(m_Stream).async_connect(
            results,
            boost::asio::bind_allocator(i3d::one::StandardAllocator<int>{}, boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session::on_connect, shared_from_this()))));
    }

    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
    {
        if (check_cancelled()) return;
        if (ec)
        {
            failure(ec, "connect");
            return;
        }
        if (is_secure())
        {
            // What I had to do to make the below code work is a strong argument for why certify isn't a part of boost
            // yet. Although I find it extremely stupid that Boost.URL added a bunch of new string_views and *didn't* provide
            // conversions for any of them to boost::string_view.
            std::string_view tmphost = m_Url.encoded_host();
            boost::string_view host{ tmphost.data(), tmphost.size() };
            boost::certify::detail::set_server_hostname(m_Stream.native_handle(), host, ec);
            if (ec)
            {
                failure(ec, "set_server_hostname");
                return;
            }
            // sni_hostname incorrectly uses static typing, so it can't handle any variation of the stream type even if that
            // stream implements the interface
            boost::certify::sni_hostname(m_Stream, host, ec);
            if (ec)
            {
                failure(ec, "set_server_hostname");
                return;
            }

            m_Stream.async_handshake(
                ssl::stream_base::client,
                boost::asio::bind_allocator(i3d::one::StandardAllocator<int>{},
                                            boost::asio::bind_cancellation_slot(
                                                m_CancelSignal.slot(), beast::bind_front_handler(&session::on_handshake, shared_from_this()))));
        }
        else
        {
            initiate_write();
        }
    }

    void on_handshake(beast::error_code ec)
    {
        if (check_cancelled()) return;
        if (ec)
            return failure(ec, "handshake");
        // Set a timeout on the operation
        beast::get_lowest_layer(m_Stream).expires_after(m_Timeout);
        if (m_LogRequest)
            m_Logger.log(RH_LOG_LEVEL_TRACE, "{}", m_Request);
        initiate_write();
    }

    void initiate_write()
    {
        if (is_secure())
        {
            http::async_write(
                m_Stream,
                m_Request,
                boost::asio::bind_allocator(
                    i3d::one::StandardAllocator<int>{},
                    boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session::on_write, shared_from_this()))));
        }
        else
        {
            http::async_write(
                boost::beast::get_lowest_layer(m_Stream),
                m_Request,
                boost::asio::bind_allocator(
                    i3d::one::StandardAllocator<int>{},
                    boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session::on_write, shared_from_this()))));
        }
    }

    void on_write(beast::error_code ec, std::size_t bytes_transferred)
    {
        if (check_cancelled()) return;
        boost::ignore_unused(bytes_transferred);
        if (ec)
            return failure(ec, "write");
        if (is_secure())
        {
            http::async_read(
                m_Stream,
                m_Buffer,
                m_Response,
                boost::asio::bind_allocator(
                    i3d::one::StandardAllocator<int>{},
                    boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session::on_read, shared_from_this()))));
        }
        else
        {
            http::async_read(
                boost::beast::get_lowest_layer(m_Stream),
                m_Buffer,
                m_Response,
                boost::asio::bind_allocator(
                    i3d::one::StandardAllocator<int>{},
                    boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session::on_read, shared_from_this()))));
        }
    }

    void on_read(beast::error_code ec, std::size_t bytes_transferred)
    {
        if (check_cancelled()) return;
        boost::ignore_unused(bytes_transferred);
        if (ec)
            return failure(ec, "read");
        m_Logger.log(RH_LOG_LEVEL_TRACE, "{}", m_Response);
        http::status status = m_Response.result();
        if (http::status_class::successful == http::to_status_class(status))
            m_Status = RH_STATUS_OK;
        else
            m_Status = static_cast<RallyHereStatusCode>(status);
        callback();

        // Set a timeout on the operation
        beast::get_lowest_layer(m_Stream).expires_after(m_Timeout);

        // Gracefully close the stream
        if (is_secure())
        {
            m_Stream.async_shutdown(boost::asio::bind_allocator(
                i3d::one::StandardAllocator<int>{},
                boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session::on_shutdown, shared_from_this()))));
        }
        else
        {
            beast::get_lowest_layer(m_Stream).socket().shutdown(tcp::socket::shutdown_both, ec);
            if (ec && ec != beast::errc::not_connected)
                return failure(ec, "shutdown");
        }
    }

    void on_shutdown(beast::error_code ec)
    {
        if (check_cancelled()) return;
        if (ec == net::error::eof || ec == boost::asio::ssl::error::stream_truncated)
        {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            // https://github.com/boostorg/beast/issues/824
            ec = {};
        }
        if (ec)
            return failure(ec, "shutdown");

        // If we get here then the connection is closed gracefully
    }

    void failure(boost::system::error_code ec, const char* what)
    {
        m_Logger.log(RH_LOG_LEVEL_ERROR, "{}: {}", what, ec.message());
        m_Status = RH_STATUS_ERROR;
        m_SystemErrorCode = ec;
        callback();
    }

    // The callback can only be called once so make sure we delete it after using it once
    void callback()
    {
        if (!m_Callback)
            return;
        if (m_CallbackCount >= m_CallbackLimit)
            return;
        m_CallbackCount++;
        m_Callback(*this);
    }

    bool check_cancelled()
    {
        if (m_Cancelled)
        {
            m_Status = RH_STATUS_CANCELLED;
            callback();
            return true;
        }
        return false;
    }

  public:
    tcp::resolver m_Resolver;
    beast::ssl_stream<beast::tcp_stream> m_Stream;
    beast::basic_flat_buffer<i3d::one::StandardAllocator<char>> m_Buffer;
    http::request<string_body> m_Request;
    http::response<string_body> m_Response;
    rallyhere::Status m_Status{ RH_STATUS_OK };
    boost::system::error_code m_SystemErrorCode{};
    rallyhere::string m_SourceUrl;
    boost::urls::url_view m_Url;
    callback_function_t m_Callback;
    logger m_Logger{};
    bool m_Cancelled{ false };
    boost::asio::cancellation_slot m_CancellationSlot;
    boost::asio::cancellation_signal m_CancelSignal;
    std::chrono::milliseconds m_Timeout{ 10000 };
    bool m_LogRequest{ false };
    size_t m_CallbackLimit{ 1 };
    size_t m_CallbackCount{ 0 };
};

void GameInstanceAdapter::SetupSIC()
{
    m_SicServerId = boost::asio::ip::host_name();
    m_SicHostName = m_SicServerId;
    m_Port = "7777";
    m_PublicPort.clear();
    m_MultiHome = "0.0.0.0";
    if (m_RallyHereUrl.back() == '/')
        m_RallyHereUrl.pop_back();
    m_SicPollUrl = m_RallyHereUrl + "/instances/sic_registration/v1/state";
    m_SicLoginUrl = m_RallyHereUrl + "/users/v1/login";
    m_SicRegistrationUrl = m_RallyHereUrl + "/instances/sic_registration/v3/register";

    for (auto&& arg : m_Arguments)
    {
        rallyhere::string tmp;
        if (ParseArgument("rhcredentialsfile=", arg, m_SICCredentials.m_CredentialsFile))
        {
            auto lines = LoadFileToStringArray(m_SICCredentials.m_CredentialsFile);
            if (2 != lines.size())
            {
                m_Status = { RH_STATUS_CREDENTIALS_FILE_NOT_TWO_LINES };
                return;
            }
            m_SICCredentials.m_APIUserName = lines[0];
            m_SICCredentials.m_APIPassword = lines[1];
            log().log(RH_LOG_LEVEL_INFO, "Loaded rhapi username from file: {}", m_SICCredentials.m_APIUserName);
            continue;
        }
        if (ParseArgument("rhsicprofileid=", arg, m_SicProfileId))
        {
            continue;
        }
        if (ParseArgument("rhsicserverid=", arg, m_SicServerId))
        {
            continue;
        }
        if (ParseArgument("rhsichostname=", arg, m_SicHostNameCli))
        {
            continue;
        }
        if (ParseArgument("rhsichostnamequerylocal=", arg, tmp))
        {
            if (tmp == "y" || tmp == "1" || tmp == "true" || tmp == "yes" || tmp == "on")
                m_SicHostNameQueryLocal = true;
            else if (tmp == "n" || tmp == "0" || tmp == "false" || tmp == "no" || tmp == "off")
                m_SicHostNameQueryLocal = false;
        }
        if (ParseArgument("rhsicappendhostnametolabels=", arg, tmp))
        {
            if (tmp == "y" || tmp == "1" || tmp == "true" || tmp == "yes" || tmp == "on")
                m_AppendHostNameToLabels = true;
            else if (tmp == "n" || tmp == "0" || tmp == "false" || tmp == "no" || tmp == "off")
                m_AppendHostNameToLabels = false;
        }
        if (ParseArgument("PORT=", arg, m_Port))
        {
            continue;
        }
        if (ParseArgument("MULTIHOME=", arg, m_MultiHome))
        {
            continue;
        }
        if (ParseArgument("rhsicid=", arg, m_SicId))
        {
            continue;
        }
        if (ParseArgument("rhsicgrouptags=", arg, m_SicGroupTags))
        {
            continue;
        }
        if (ParseArgument("rhsicappendgrouptag=", arg, tmp))
        {
            m_ExtraSicGroupTags.push_back(tmp);
            continue;
        }
        if (ParseArgument("rhsicappendlabels=", arg, tmp))
        {
            m_ExtraLabels.push_back(tmp);
            continue;
        }
        if (ParseArgument("rhsicappendadditionalinfolabels=", arg, tmp))
        {
            m_ExtraAdditionalInfoLabels.push_back(tmp);
            continue;
        }
        if (ParseArgument("rhpublichost=", arg, m_SicPublicHostCli))
        {
            continue;
        }
        if (ParseArgument("rhsicpollintervalseconds=", arg, tmp))
        {
            try
            {
                m_SicPollInterval = std::chrono::seconds(boost::lexical_cast<std::chrono::seconds::rep>(tmp));
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_POLL_INTERVAL_MUST_BE_NUMERIC};
            }
        }
        if (ParseArgument("rhsicprometheusbind=", arg, m_SicPrometheusBind))
        {
            m_SicPrometheusBindSet = true;
            if (m_SicPrometheusPortSet)
            {
                log().log(RH_LOG_LEVEL_INFO, "Ignoring sic prometheus bind because it was set from the port", m_SicPrometheusBind);
            }
            continue;
        }
        if (ParseArgument("sicprometheusport=", arg, m_SicPrometheusPortCli))
        {
            m_SicPrometheusPortSet = true;
            if (!m_SicPrometheusBindSet)
            {
                rallyhere::memory_buffer buffer;
                fmt::format_to(std::back_inserter(buffer), "0.0.0.0:{}", m_SicPrometheusPortCli);
                m_SicPrometheusBind = { buffer.data(), buffer.size() };
                log().log(RH_LOG_LEVEL_INFO, "Setting prometheus bind from the port {} -> {}", m_SicPrometheusPortCli, m_SicPrometheusBind);
            }
            else
            {
                log().log(RH_LOG_LEVEL_INFO, "Ignoring sic prometheus port bind is already set to {}", m_SicPrometheusBind);
            }
            continue;
        }
        if (ParseArgument("rhsicrequesttimeoutms=", arg, tmp))
        {
            try
            {
                m_SicRequestTimeout = std::chrono::milliseconds(boost::lexical_cast<std::chrono::milliseconds::rep>(tmp));
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_REQUEST_TIMEOUT_MUST_BE_NUMERIC};
            }
        }
        if (ParseArgument("rhpublicport=", arg, m_PublicPort))
        {
            continue;
        }
        if (ParseArgument("rhmaxpollfailures=", arg, tmp))
        {
            try
            {
                m_PollMaxFailures = boost::lexical_cast<size_t>(tmp);
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_MAX_POLL_FAILURES_MUST_BE_NUMERIC };
            }
        }
        if (ParseArgument("rhcredentialsas=", arg, tmp))
        {
            if ("clientid" == tmp)
            {
                m_SICCredentials.m_UseCredentialsAsClientId = true;
            }
        }
        if (ParseArgument("rhsicregistrationurl=", arg, m_SicRegistrationUrl))
        {
            continue;
        }
        if (ParseArgument("rhsicloginurl=", arg, m_SicLoginUrl))
        {
            continue;
        }
        if (ParseArgument("rhsicpollurl=", arg, m_SicPollUrl))
        {
            continue;
        }
    }
    if (m_SicProfileId.empty())
    {
        m_Status = { RH_STATUS_NO_SIC_PROFILE_ID_PROVIDED };
        return;
    }
    try
    {
        boost::lexical_cast<short>(m_Port);
    }
    catch (const boost::bad_lexical_cast&e)
    {
        log().log(RH_LOG_LEVEL_ERROR, "Invalid port {}", m_Port);
        m_Status = { RH_STATUS_BIND_PORT_MUST_BE_NUMERIC };
        return;
    }
    if (m_PublicPort.empty())
        m_PublicPort = m_Port;
    try
    {
        boost::lexical_cast<short>(m_PublicPort);
    }
    catch (const boost::bad_lexical_cast&e)
    {
        log().log(RH_LOG_LEVEL_ERROR, "Invalid public port {}", m_PublicPort);
        m_Status = { RH_STATUS_PUBLIC_PORT_MUST_BE_NUMERIC };
        return;
    }
    rallyhere::memory_buffer buffer;
    if (m_SicId.empty())
    {
        buffer.clear();
        fmt::format_to(std::back_inserter(buffer), "sic_{}_{}", m_SicServerId, m_Port);
        m_SicId = { buffer.data(), buffer.size() };
    }
    char *env_var = std::getenv("HIREZ_SIC_REPORTED_PUBLIC_HOST");
    if (env_var != nullptr)
    {
        m_SicHostNameEnvHost = env_var;
    }
    env_var = std::getenv("HIREZ_SIC_REPORTED_PUBLIC_IP");
    if (env_var != nullptr)
    {
        m_SicHostNameEnvIp = env_var;
    }
    rallyhere::string hostname;
    if (m_SicHostNameQueryLocal)
    {
#if PLATFORM_LINUX
        hostname.resize(HOST_NAME_MAX + 1, '\0');
#else
        hostname.resize(256, '\0');
#endif
        if (gethostname(hostname.data(), hostname.size()) != 0)
        {
            hostname.clear();
        }
    }
    rallyhere::stringstream sstr;
    sstr << "Provided hostname options: ";
    sstr << "default " << m_SicHostName << " ";
    if (!m_SicHostNameCli.empty())
    {
        sstr << "rhsichostname=" << m_SicHostNameCli << " ";
    }
    if (!hostname.empty())
    {
        sstr << "querylocal " << hostname << " ";
    }
    log().log(RH_LOG_LEVEL_INFO, sstr.str());
    if (!m_SicHostNameCli.empty())
    {
        m_SicHostName = m_SicHostNameCli;
    }
    if (!hostname.empty())
    {
        m_SicHostName = hostname;
    }
    sstr.str("");
    sstr << "Provided public host options: ";
    if (!m_SicHostNameEnvHost.empty())
    {
        sstr << "HIREZ_SIC_REPORTED_PUBLIC_HOST=" << m_SicHostNameEnvHost << " ";
    }
    if (!m_SicHostNameEnvIp.empty())
    {
        sstr << "HIREZ_SIC_REPORTED_PUBLIC_IP=" << m_SicHostNameEnvIp << " ";
    }
    if (!m_SicPublicHostCli.empty())
    {
        sstr << "rhpublichost=" << m_SicPublicHostCli << " ";
    }
    log().log(RH_LOG_LEVEL_INFO, sstr.str());

    if (!m_SicPublicHostCli.empty())
    {
        m_SicPublicHost = m_SicPublicHostCli;
    }
    else if (!m_SicHostNameEnvHost.empty())
    {
        m_SicPublicHost = m_SicHostNameEnvHost;
    }
    else if (!m_SicHostNameEnvIp.empty())
    {
        m_SicPublicHost = m_SicHostNameEnvIp;
    }
    else
    {
        log().log(RH_LOG_LEVEL_ERROR, "No reporting public provided. set env variable HIREZ_SIC_REPORTED_PUBLIC_HOST");
        m_Status = { RH_STATUS_MUST_PROVIDE_REPORTED_PUBLIC_HOST };
        return;
    }
    log().log(RH_LOG_LEVEL_INFO, "Using public host: {} public port: {}", m_SicPublicHost, m_PublicPort);
    if (m_SicGroupTags.empty())
    {
        buffer.clear();
        fmt::format_to(std::back_inserter(buffer), "profile_id:{},server_id:{},hostname:{}", m_SicProfileId, m_SicServerId, m_SicHostName);
        m_SicGroupTags = { buffer.data(), buffer.size() };
    }
    for (auto&& tag : m_ExtraSicGroupTags)
    {
        m_SicGroupTags += ",";
        m_SicGroupTags += tag;
    }

    env_var = std::getenv("SIC_PROMETHEUS_ADDITIONAL_LABELS");
    if (env_var != nullptr)
    {
        rallyhere::vector<rallyhere::string> labels;
        boost::split(labels, env_var, boost::is_any_of(","), boost::token_compress_on);
        for (auto &label : labels)
        {
            rallyhere::vector<rallyhere::string> kv;
            boost::split(kv, label, boost::is_any_of(":"), boost::token_compress_on);
            m_Labels.Set(kv[0].c_str(), kv[1]);
        }
    }
    for (auto&& label : m_ExtraLabels)
    {
        rallyhere::vector<rallyhere::string> kv;
        boost::split(kv, label, boost::is_any_of(":"), boost::token_compress_on);
        m_Labels.Set(kv[0].c_str(), kv[1]);
    }
    if (m_AppendHostNameToLabels)
    {
        m_Labels.Set("hostname", m_SicHostName.c_str());
    }
    env_var = std::getenv("SIC_PROMETHEUS_ADDITIONAL_INFO_LABELS");
    if (env_var != nullptr)
    {
        rallyhere::vector<rallyhere::string> labels;
        boost::split(labels, env_var, boost::is_any_of(","), boost::token_compress_on);
        for (auto &label : labels)
        {
            rallyhere::vector<rallyhere::string> kv;
            boost::split(kv, label, boost::is_any_of(":"), boost::token_compress_on);
            m_InternalAdditionalInfoLabels.Set(kv[0].c_str(), kv[1]);
        }
    }
    for (auto&& label : m_ExtraAdditionalInfoLabels)
    {
        rallyhere::vector<rallyhere::string> kv;
        boost::split(kv, label, boost::is_any_of(":"), boost::token_compress_on);
        m_InternalAdditionalInfoLabels.Set(kv[0].c_str(), kv[1]);
    }

    rallyhere::string upperedState = m_LastPolledState;
    upperedState[0] = toupper(upperedState[0]);
    m_InternalAdditionalInfoLabels.get()["sic_state"] = upperedState;
    m_InternalLabels.Set("sic_id", m_SicId);

    if (m_SICCredentials.m_UseCredentialsAsClientId)
    {
        m_SICCredentials.m_APIClientId = m_SICCredentials.m_APIUserName;
        m_SICCredentials.m_APISecret = m_SICCredentials.m_APIPassword;
        m_SICCredentials.m_APIUserName.clear();
        m_SICCredentials.m_APIPassword.clear();
    }
}

template<class>
inline constexpr bool always_false_v = false;

void GameInstanceAdapter::DoPollSIC(std::function<void(RallyHereStringMapPtr, const RallyHereStatusCode&)> next)
{
    auto alloc = i3d::one::StandardAllocator<session>{};
    auto session_ptr = std::allocate_shared<session>(alloc, net::make_strand(m_IoContext), m_SslContext, m_CancelSignal.slot());
    auto url = m_SicPollUrl;
    auto request_pair = BuildPollRequest(url);
    if (request_pair.second)
    {
        log().log(RH_LOG_LEVEL_ERROR, "poll: {}", request_pair.second.message());
        next(nullptr, RH_STATUS_ERROR);
        return;
    }
    auto wrapper = [this, next](session& session) {
        if (session.m_Status.code() == RH_STATUS_CANCELLED)
            return;
        if (!session.m_Status)
        {
            if (m_PollFailures++ > m_PollMaxFailures)
                next(nullptr, session.m_Status.code());
            return;
        }
        boost::json::error_code ec;
        auto jv = boost::json::parse(session.m_Response.body(), ec, &m_CustomResource);
        if (ec)
        {
            session.m_Logger.log(RH_LOG_LEVEL_ERROR, "parse poll response: {}", ec.message());
            if (m_PollFailures++ > m_PollMaxFailures)
                next(nullptr, RH_STATUS_COULD_NOT_PARSE_RESPONSE);
            return;
        }
        if (!m_FakeAllocationData.empty())
        {
            jv = boost::json::parse(m_FakeAllocationData, ec, &m_CustomResource);
            session.m_Logger.log(RH_LOG_LEVEL_DEBUG, "fake allocation data: {}", jv);
            m_FakeAllocationData = "";
        }
        try
        {
            SICPollModel response(boost::json::value_to<SICPollModel>(jv));
            std::visit(
                [&](auto&& item) {
                    using T = std::decay_t<decltype(item)>;
                    if constexpr (std::is_same_v<SICPoll, T>)
                    {
                        if (!item.success)
                        {
                            session.m_Logger.log(RH_LOG_LEVEL_ERROR, "Poll response was not successful: {}:{}", item.error_code, item.error_message);
                            if (m_PollFailures++ > m_PollMaxFailures)
                                next(nullptr, RH_STATUS_ERROR);
                            return;
                        }
                        if (m_LastPolledState != item.state)
                        {
                            rallyhere::string upperedState = item.state;
                            upperedState[0] = toupper(upperedState[0]);
                            m_InternalAdditionalInfoLabels.get()["sic_state"] = upperedState;
                            m_LastPolledState = item.state;
                            RefreshAdditionalInfoLabels();
                        }
                        if (item.state == "allocated")
                        {
                            // Store and report the allocation id
                            m_AllocationId = item.allocation_id;
                            m_SessionId = item.session_id;
                            m_PublicHost = m_SicPublicHost;
                            m_AllocatedPublicPort = m_PublicPort;
                            m_Allocated = true;
                            session.m_Logger.log(RH_LOG_LEVEL_INFO, "Poll response was successful allocation_id={} session_id={}", item.allocation_id, item.session_id);
                            auto allocation_info = i3d::one::allocator::create<rallyhere::StringMap>();
                            allocation_info->Set("allocation_id", item.allocation_id);
                            allocation_info->Set("session_id", item.session_id);
                            allocation_info->Set("public_host", m_PublicHost);
                            allocation_info->Set("public_port", m_AllocatedPublicPort);
                            AddBindIpsAndPorts(allocation_info);
                            if (LastAllocatedGauge)
                                LastAllocatedGauge->SetToCurrentTime();
                            next(reinterpret_cast<RallyHereStringMapPtr>(allocation_info), RH_STATUS_OK);
                        }
                        else if (item.state == "allocating" || item.state == "deallocated")
                        {
                            m_PollFailures = 0;
                        }
                        else
                        {
                            session.m_Logger.log(RH_LOG_LEVEL_ERROR, "Poll response was successful but had unknown state {}", item.state);
                            if (m_PollFailures++ > m_PollMaxFailures)
                                next(nullptr, RH_STATUS_POLL_UNKNOWN_STATE);
                        }
                    }
                    else if constexpr (std::is_same_v<SICBaseModel, T>)
                    {
                        session.m_Logger.log(RH_LOG_LEVEL_ERROR, "Poll response was not successful: {}:{}", item.error_code, item.error_message);
                        if (m_PollFailures++ > m_PollMaxFailures)
                            next(nullptr, RH_STATUS_ERROR);
                    }
                    else
                        static_assert(always_false_v<T>, "non-exhaustive visitor!");
                },
                response.items.at(0));
        }
        catch (const std::exception& e)
        {
            session.m_Logger.log(RH_LOG_LEVEL_ERROR, "parse poll response: {}", e.what());
            if (m_PollFailures++ > m_PollMaxFailures)
                next(nullptr, RH_STATUS_COULD_NOT_PARSE_RESPONSE);
        }
    };
    session_ptr->run(url, std::move(request_pair.first), wrapper, log(), true, m_SicRequestTimeout);
}

void GameInstanceAdapter::PollSIC()
{
    auto final = [this](RallyHereStringMapPtr allocation_info, const RallyHereStatusCode& status) {
        if (!m_OnAllocatedCallback)
            return;
        if (RH_STATUS_OK == status)
        {
            m_OnAllocatedCallback(reinterpret_cast<RallyHereStringMapPtr>(allocation_info), RH_STATUS_OK, m_OnAllocatedUserData);
        }
        else
        {
            m_OnAllocatedCallback(nullptr, status, m_OnAllocatedUserData);
        }
    };
    auto dopollsic = [this, final](const RallyHereStatusCode& status) {
        DoPollSIC(final);
    };
    auto loginifforbiddenafterrefresh = [this, dopollsic, final](RallyHereStringMapPtr allocation_info, const RallyHereStatusCode& status) {
        if (status == RH_STATUS_HTTP_FORBIDDEN)
        {
            Login(dopollsic);
        }
        else
        {
            final(allocation_info, status);
        }
    };
    auto dopollsicthenloginifforbidden = [this, loginifforbiddenafterrefresh](const RallyHereStatusCode& status) {
        DoPollSIC(loginifforbiddenafterrefresh);
    };
    auto refreshifneeded = [this, dopollsicthenloginifforbidden, final](RallyHereStringMapPtr allocation_info, const RallyHereStatusCode& status) {
        if (status == RH_STATUS_HTTP_FORBIDDEN)
        {
            Login(dopollsicthenloginifforbidden);
        }
        else
        {
            final(allocation_info, status);
        }
    };
    DoPollSIC(refreshifneeded);
}

void GameInstanceAdapter::Login(std::function<void(const RallyHereStatusCode&)> next)
{
    if (m_RefreshTokenValid)
    {
        log().log(RH_LOG_LEVEL_INFO, "Attempting login to SIC with refresh token");
    }
    else
    {
        log().log(RH_LOG_LEVEL_INFO, "Attempting login to SIC with credentials");
    }
    auto alloc = i3d::one::StandardAllocator<session>{};
    auto session_ptr = std::allocate_shared<session>(alloc, net::make_strand(m_IoContext), m_SslContext, m_CancelSignal.slot());
    auto url = m_SicLoginUrl;
    auto request_pair = BuildLoginRequest(url);
    if (request_pair.second)
    {
        log().log(RH_LOG_LEVEL_ERROR, "login: {}", request_pair.second.message());
        next(RH_STATUS_ERROR);
        return;
    }
    auto wrapper = [this, next](session& session) {
        if (!session.m_Status)
        {
            if (session.m_Status.code() == RH_STATUS_HTTP_FORBIDDEN && m_RefreshTokenValid)
            {
                log().log(RH_LOG_LEVEL_INFO, "Login failed, retrying with credentials");
                m_RefreshToken.clear();
                m_RefreshTokenValid = false;
                Login(next);
                return;
            }
            next(session.m_Status.code());
            return;
        }
        boost::json::error_code ec;
        auto jv = boost::json::parse(session.m_Response.body(), ec, &m_CustomResource);
        if (ec)
        {
            session.m_Logger.log(RH_LOG_LEVEL_ERROR, "parse login response: {}", ec.message());
            next(RH_STATUS_COULD_NOT_PARSE_RESPONSE);
            return;
        }
        if (!jv.is_object())
        {
            session.m_Logger.log(RH_LOG_LEVEL_ERROR, "login response is not an object");
            next(RH_STATUS_COULD_NOT_PARSE_RESPONSE);
            return;
        }
        m_LastAuthResult = jv.get_object();
        auto access_token_src = m_LastAuthResult.if_contains("access_token");
        if (!access_token_src)
        {
            session.m_Logger.log(RH_LOG_LEVEL_ERROR, "login response does not contain access_token");
            m_AccessToken.clear();
            next(RH_STATUS_RESPONSE_NO_ACCESS_TOKEN);
            return;
        }
        m_AccessToken = access_token_src->as_string().c_str();
        auto refresh_token_src = m_LastAuthResult.if_contains("refresh_token");
        if (refresh_token_src)
        {
            m_RefreshToken = refresh_token_src->as_string().c_str();
            m_RefreshTokenValid = true;
        }
        session.m_Logger.log(RH_LOG_LEVEL_INFO, "SIC login complete");
        session.m_Logger.log(RH_LOG_LEVEL_TRACE, "access token: {} refresh token: {}", m_AccessToken, m_RefreshToken);
        next(session.m_Status.code());
    };
    session_ptr->run(url, std::move(request_pair.first), wrapper, log(), false, m_SicRequestTimeout);
}

void GameInstanceAdapter::ConnectSIC(base_callback_function_t callback, void* user_data)
{
    auto next = [callback, user_data](const RallyHereStatusCode& code)
    {
        callback(code, user_data);
    };
    /*
    auto forcerefreshtoken = [this, next](const RallyHereStatusCode& code)
    {
        if (!code)
            Login(next);
        else
            next(code);
    };
    auto forcerefreshtokentwice = [this, next, forcerefreshtoken](const RallyHereStatusCode& code)
    {
        if (!code)
            Login(forcerefreshtoken);
        else
            next(code);
    };
    */
    Login(next);
}

void GameInstanceAdapter::ReadySIC(base_callback_function_t callback, void* user_data)
{
    if (m_A2SQueryPort != 0)
    {
        log().log(RH_LOG_LEVEL_INFO, "Setting up A2S for SIC");
        // Setup A2S
        auto a2s_alloc = i3d::one::StandardAllocator<a2s_listener>{};
        m_A2SListener = std::allocate_shared<a2s_listener>(a2s_alloc, net::make_strand(m_IoContext), m_A2SQueryPort, log(), m_A2SChallenge);
        if (m_A2SListener->fail())
        {
            log().log(RH_LOG_LEVEL_ERROR, "Failed to start A2S listener");
            if (callback)
                callback(RH_STATUS_A2S_COULD_NOT_START, user_data);
            return;
        }
        RallyHereStatsBaseProvided provided{};
        memset(&provided, 0xff, sizeof(provided));
        m_A2SListener->update_server_info(m_StatsBase, provided, []() {});
        m_A2SListener->run();
    }
    try
    {
        RebuildPrometheus();
    }
    catch (const std::exception& e)
    {
        log().log(RH_LOG_LEVEL_ERROR, "prometheus failed: {}", e.what());
        callback(RH_STATUS_PROMETHEUS_COULD_NOT_START, user_data);
        return;
    }
    log().log(RH_LOG_LEVEL_INFO, "Attempting SIC registration");
    CallAfterAuthValidation([=]() {
      auto alloc = i3d::one::StandardAllocator<session>{};
      auto session_ptr = std::allocate_shared<session>(alloc, net::make_strand(m_IoContext), m_SslContext, m_CancelSignal.slot());
      auto url = m_SicRegistrationUrl;
      auto request_pair = BuildRegistrationRequest(url);
      if (request_pair.second)
      {
          log().log(RH_LOG_LEVEL_ERROR, "ready: {}", request_pair.second.message());
          callback(RH_STATUS_ERROR, user_data);
          return;
      }
      auto wrapper = [this, callback, user_data](session& session) {
        if (!session.m_Status)
        {
            if (403 == session.m_Status.code())
            {
                RetryAuthorization();
                return;
            }
            callback(session.m_Status.code(), user_data);
            return;
        }
        boost::json::error_code ec;
        auto jv = boost::json::parse(session.m_Response.body(), ec, &m_CustomResource);
        if (ec)
        {
            session.m_Logger.log(RH_LOG_LEVEL_ERROR, "parse ready response: {} :{}", ec.message(), session.m_Response.body());
            callback(RH_STATUS_COULD_NOT_PARSE_RESPONSE, user_data);
            return;
        }
        try
        {
            SICRegisterModel response(boost::json::value_to<SICRegisterModel>(jv));
            if (response.is_any_ready_to_poll())
            {
                m_Registered = true;
                session.m_Logger.log(RH_LOG_LEVEL_INFO, "SIC registration complete");
                session.m_Logger.log(RH_LOG_LEVEL_TRACE, "ready {}", jv);
                try
                {
                    RebuildPrometheus();
                }
                catch (const std::exception& e)
                {
                    session.m_Logger.log(RH_LOG_LEVEL_ERROR, "prometheus failed: {}", e.what());
                    callback(RH_STATUS_PROMETHEUS_FAILED_AFTER_REGISTRATION, user_data);
                    return;
                }
                callback(session.m_Status.code(), user_data);
            }
            else
            {
                m_Registered = false;
                session.m_Logger.log(RH_LOG_LEVEL_ERROR, "ready failed {}", jv);
                callback(RH_STATUS_ERROR, user_data);
            }
        }
        catch (const std::exception& e)
        {
            session.m_Logger.log(RH_LOG_LEVEL_ERROR, "read ready response: {}", e.what());
            callback(RH_STATUS_COULD_NOT_PARSE_RESPONSE, user_data);
        }
      };
      session_ptr->run(url, std::move(request_pair.first), wrapper, log(), true, m_SicRequestTimeout);
    });
}

void basic_with_base64_data(rallyhere::memory_buffer& buffer, const rallyhere::string& left, const rallyhere::string& right)
{
    rallyhere::memory_buffer base64buffer;
    fmt::format_to(std::back_inserter(base64buffer), "{}:{}", left, right);
    base64<rallyhere::string> encoder;
    auto encoded = encoder.encode(std::string_view{ base64buffer.data(), base64buffer.size() }, false);
    fmt::format_to(std::back_inserter(buffer), "Basic {}", std::string_view{ encoded.data(), encoded.size() });
}

std::pair<http::request<string_body>, boost::system::error_code> GameInstanceAdapter::BuildLoginRequest(const rallyhere::string& in_url_str)
{
    http::request<string_body> m_Request;
    rallyhere::string m_SourceUrl;
    boost::urls::url_view m_Url;

    m_SourceUrl = in_url_str;
    auto result = boost::urls::parse_uri(m_SourceUrl);
    if (!result)
    {
        return { m_Request, result.error() };
    }
    m_Url = *result;
    m_Request.method(http::verb::post);
    m_Request.target(m_Url.encoded_path());
    m_Request.set(http::field::host, m_Url.encoded_host());
    m_Request.set(http::field::user_agent, m_UserAgent);
    m_Request.set(http::field::content_type, "application/json; charset=utf-8");

    custom_resource mr;
    boost::json::object j(&mr);
    if (m_RefreshTokenValid)
    {
        j["grant_type"] = "refresh";
        j["portal_access_token"] = m_RefreshToken;
    }
    else
    {
        if (m_SICCredentials.m_UseCredentialsAsClientId)
        {
            j["grant_type"] = "client_credentials";
            j["include_refresh"] = true;
            rallyhere::memory_buffer buffer;
            basic_with_base64_data(buffer, m_SICCredentials.m_APIClientId, m_SICCredentials.m_APISecret);
            m_Request.set(http::field::authorization, std::string_view{ buffer.data(), buffer.size() });
        }
        else
        {
            j["grant_type"] = "basic";
            j["include_refresh"] = true;
            rallyhere::memory_buffer buffer;
            fmt::format_to(std::back_inserter(buffer), "{}:{}", m_SICCredentials.m_APIUserName, m_SICCredentials.m_APIPassword);
            j["portal_access_token"] = std::string_view{ buffer.data(), buffer.size() };
            j["accept_eula"] = true;
            j["accept_tos"] = true;
            j["accept_privacy_policy"] = true;
        }
    }

    rallyhere::stringstream sstr;
    boost::json::serializer sr(&mr);
    sr.reset(&j);
    do
    {
        char buf[256];
        sstr << sr.read(buf);
    } while (!sr.done());
    auto s = sstr.str();
    m_Request.body() = s;
    m_Request.prepare_payload();

    return { m_Request, {} };
}

std::pair<http::request<string_body>, boost::system::error_code> GameInstanceAdapter::BuildRegistrationRequest(const rallyhere::string& in_url_str)
{
    http::request<string_body> request;
    rallyhere::string source_url;
    boost::urls::url_view url;

    source_url = in_url_str;
    try
    {
        auto result = boost::urls::parse_uri(source_url);
        if (!result)
        {
            log().log(RH_LOG_LEVEL_ERROR, "BuildRegistrationRequest: parse_uri failed: {} source {}", result.error(), source_url);
            return { request, result.error() };
        }
        url = *result;
    }
    catch (const std::length_error& e)
    {
        log().log(RH_LOG_LEVEL_ERROR, "BuildRegistrationRequest: parse_uri failed: url is too long source {}", source_url);
        return { request, boost::system::error_code{ boost::system::errc::invalid_argument, boost::system::generic_category() } };
    }
    request.method(http::verb::post);
    request.target(url.encoded_path());
    request.set(http::field::host, url.encoded_host());
    request.set(http::field::user_agent, m_UserAgent);
    request.set(http::field::content_type, "application/json; charset=utf-8");

    rallyhere::memory_buffer buffer;
    fmt::format_to(std::back_inserter(buffer), "Bearer {}", m_AccessToken);
    request.set(http::field::authorization, std::string_view{ buffer.data(), buffer.size() });

    custom_resource mr;
    SICRegisterRequest rr;
    rr.id = m_SicId;
    rr.group_tags = m_SicGroupTags;
    rr.public_host = m_SicPublicHost;
    try
    {
        rr.public_port = boost::lexical_cast<int>(m_PublicPort);
    }
    catch (const boost::bad_lexical_cast& e)
    {
        return { request, boost::system::error_code{ boost::system::errc::invalid_argument, boost::system::generic_category() } };
    }
    SICRegisterRequestModel rrm;
    rrm.items.push_back(rr);
    try
    {
        boost::json::value j = boost::json::value_from(rrm, &mr);

        rallyhere::stringstream sstr;
        boost::json::serializer sr(&mr);
        sr.reset(&j);
        do
        {
            char buf[256];
            sstr << sr.read(buf);
        } while (!sr.done());
        auto s = sstr.str();
        request.body() = s;
        request.prepare_payload();

        return { request, {} };
    }
    catch (const std::exception& e)
    {
        return { request, boost::system::error_code{ boost::system::errc::invalid_argument, boost::system::generic_category() } };
    }
}

std::pair<http::request<string_body>, boost::system::error_code> GameInstanceAdapter::BuildPollRequest(const rallyhere::string& in_url_str)
{
    http::request<string_body> m_Request;
    rallyhere::string m_SourceUrl;
    boost::urls::url_view m_Url;

    m_SourceUrl = in_url_str;
    try
    {
        auto result = boost::urls::parse_uri(m_SourceUrl);
        if (!result)
        {
            log().log(RH_LOG_LEVEL_ERROR, "BuildPollRequest: parse_uri failed: {} source {}", result.error(), m_SourceUrl);
            return { m_Request, result.error() };
        }
        m_Url = *result;
    }
    catch (const std::length_error& e)
    {
        log().log(RH_LOG_LEVEL_ERROR, "BuildPollRequest: parse_uri failed: url is too long source {}", m_SourceUrl);
        return { m_Request, boost::system::error_code{ boost::system::errc::invalid_argument, boost::system::generic_category() } };
    }
    m_Request.method(http::verb::post);
    m_Request.target(m_Url.encoded_path());
    m_Request.set(http::field::host, m_Url.encoded_host());
    m_Request.set(http::field::user_agent, m_UserAgent);
    m_Request.set(http::field::content_type, "application/json; charset=utf-8");

    rallyhere::memory_buffer buffer;
    fmt::format_to(std::back_inserter(buffer), "Bearer {}", m_AccessToken);
    m_Request.set(http::field::authorization, std::string_view{ buffer.data(), buffer.size() });

    custom_resource mr;
    boost::json::object j(&mr);
    j["items"] = { {{"id", m_SicId}} };

    try
    {
        rallyhere::stringstream sstr;
        boost::json::serializer sr(&mr);
        sr.reset(&j);
        do
        {
            char buf[256];
            sstr << sr.read(buf);
        } while (!sr.done());
        auto s = sstr.str();
        m_Request.body() = s;
        m_Request.prepare_payload();

        return { m_Request, {} };
    }
    catch (const std::exception& e)
    {
        return { m_Request, boost::system::error_code{ boost::system::errc::invalid_argument, boost::system::generic_category() } };
    }
}

} // namespace rallyhere
