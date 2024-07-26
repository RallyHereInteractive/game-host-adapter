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


namespace rallyhere
{

struct SimulatorGame
{
    int startup_lag{};
    int game_length{};
    int players;
    int max_players;
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, SimulatorGame const& req)
{
    jv = {
        { "startup_lag", req.startup_lag },
        { "game_length", req.game_length },
        { "players", req.players },
        { "max_players", req.max_players },
    };
}

SimulatorGame tag_invoke(boost::json::value_to_tag<SimulatorGame>, boost::json::value const& jv)
{
    SimulatorGame req{};
    req.startup_lag = jv.at("startup_lag").as_int64();
    req.game_length = jv.at("game_length").as_int64();
    req.players = jv.at("players").as_int64();
    req.max_players = jv.at("max_players").as_int64();
    return req;
}

class session_http : public std::enable_shared_from_this<session_http>
{
  public:
    using callback_function_t = std::function<void(session_http&)>;

  public:
    explicit session_http(net::any_io_executor ex, boost::asio::cancellation_slot cancellation_slot) : m_Resolver(ex), m_Stream(ex), m_Cancelled{false}, m_CancellationSlot{cancellation_slot}
    {
        m_CancellationSlot.assign([this](boost::asio::cancellation_type_t cancellation_type) { cancel_handler(cancellation_type); });
    }

    ~session_http()
    {
        m_CancellationSlot.clear();
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

        std::string_view scheme = "http";
        if (m_Url.has_scheme())
        {
            scheme = m_Url.scheme();
        }
        std::string_view service = "http";
        if (m_Url.has_port())
        {
            service = m_Url.port();
        }
        m_Resolver.async_resolve(
            m_Url.encoded_host(),
            service,
            boost::asio::bind_allocator(i3d::one::StandardAllocator<int>{}, boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session_http::on_resolve, shared_from_this()))));
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
            boost::asio::bind_allocator(i3d::one::StandardAllocator<int>{}, boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session_http::on_connect, shared_from_this()))));
    }

    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
    {
        if (check_cancelled()) return;
        if (ec)
        {
            failure(ec, "connect");
            return;
        }
        http::async_write(
            m_Stream,
            m_Request,
            boost::asio::bind_allocator(i3d::one::StandardAllocator<int>{}, boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session_http::on_write, shared_from_this()))));
    }

    void on_write(beast::error_code ec, std::size_t bytes_transferred)
    {
        if (check_cancelled()) return;
        boost::ignore_unused(bytes_transferred);
        if (ec)
            return failure(ec, "write");
        http::async_read(
            m_Stream,
            m_Buffer,
            m_Response,
            boost::asio::bind_allocator(i3d::one::StandardAllocator<int>{}, boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session_http::on_read, shared_from_this()))));
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
        beast::get_lowest_layer(m_Stream).socket().shutdown(tcp::socket::shutdown_both, ec);
        if (ec && ec != beast::errc::not_connected)
            return failure(ec, "shutdown");
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
        if (m_Callback)
        {
            m_Callback(*this);
        }
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
    beast::tcp_stream m_Stream;
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
};

std::pair<http::request<string_body>, boost::system::error_code> GameInstanceAdapter::BuildSimulateGameRequest(const rallyhere::string& in_url_str)
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
    m_Request.method(http::verb::get);
    m_Request.target(m_Url.encoded_path());
    m_Request.set(http::field::host, m_Url.encoded_host());
    m_Request.set(http::field::user_agent, m_UserAgent);
    m_Request.set(http::field::content_type, "application/json; charset=utf-8");

    m_Request.prepare_payload();

    return { m_Request, {} };
}

void GameInstanceAdapter::SimulateGame()
{
    if (m_SimulatorUrl.empty())
    {
        m_NextSimulatedGame = std::chrono::steady_clock::now() + std::chrono::days(1000);
        return;
    }
    log().log(RH_LOG_LEVEL_INFO, "Attempting to get the next simulated game");
    CallAfterAuthValidation([=]() {
        auto alloc = i3d::one::StandardAllocator<session_http>{};
        auto session_ptr = std::allocate_shared<session_http>(alloc, net::make_strand(m_IoContext), m_CancelSignal.slot());
        auto url = m_SimulatorUrl;
        auto request_pair = BuildSimulateGameRequest(url);
        if (request_pair.second)
        {
            log().log(RH_LOG_LEVEL_ERROR, "simulate: {}", request_pair.second.message());
            m_NextSimulatedGame = std::chrono::steady_clock::now() + std::chrono::seconds(30);
            return;
        }
        auto wrapper = [this](session_http& session) {
            boost::json::error_code ec;
            auto jv = boost::json::parse(session.m_Response.body(), ec, &m_CustomResource);
            if (ec)
            {
                session.m_Logger.log(RH_LOG_LEVEL_ERROR, "simulate game response: {} :{}", ec.message(), session.m_Response.body());
                m_NextSimulatedGame = std::chrono::steady_clock::now() + std::chrono::seconds(30);
                return;
            }
            try
            {
                auto game = boost::json::value_to<SimulatorGame>(jv);
                session.m_Logger.log(RH_LOG_LEVEL_INFO, "Simulated game: startup_lag: {}, game_length: {}, players: {}, max_players: {}", game.startup_lag, game.game_length, game.players, game.max_players);
                auto last_time_point = std::chrono::steady_clock::now();
                // Start with the max players
                TimedStatsChange init{};
                init.time = last_time_point;
                init.base.max_players = game.max_players;
                init.provided.set_max_players = true;
                m_FakeStatChanges.push_back(init);
                // Have the players join when the game starts
                TimedStatsChange start{};
                start.time = last_time_point + std::chrono::seconds(game.startup_lag);
                start.base.players = game.players;
                start.base.max_players = game.max_players;
                start.provided.set_players = true;
                start.provided.set_max_players = true;
                m_FakeStatChanges.push_back(start);
                // Have everybody leave when the game is over
                TimedStatsChange end{};
                end.time = start.time + std::chrono::seconds(game.game_length);
                end.base.players = 0;
                end.base.max_players = game.max_players;
                end.provided.set_players = true;
                end.provided.set_max_players = true;
                m_FakeStatChanges.push_back(end);
                // The next game starts when the last one ends
                if (game.game_length < 5)
                    m_NextSimulatedGame = std::chrono::steady_clock::now() + std::chrono::seconds(5);
                else
                    m_NextSimulatedGame = end.time;
            }
            catch (const std::exception& e)
            {
                session.m_Logger.log(RH_LOG_LEVEL_ERROR, "simulate game response: {} :{}", e.what(), session.m_Response.body());
                m_NextSimulatedGame = std::chrono::steady_clock::now() + std::chrono::seconds(30);
                return;
            }
        };
        session_ptr->run(url, std::move(request_pair.first), wrapper, log(), false, m_SicRequestTimeout);
    });
}

} // namespace rallyhere
