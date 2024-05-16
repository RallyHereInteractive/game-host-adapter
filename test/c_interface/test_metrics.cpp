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
#include "test_sic.h"
#include "c_api.h"

#include <map>

#include "rh_vector.h"
#include "rh_string.h"
#include "rh_allocator.h"
#include "rh_status.h"

#include "boost/scope_exit.hpp"
#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/url.hpp"

#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/ssl.hpp"
#include "boost/beast/version.hpp"
#include "boost/asio.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/strand.hpp"

#include "shared_test_data.h"
#include "shared_definitions.hpp"

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

#include "sdk_formatters.hpp"

template<typename T>
static std::vector<std::string> read_each_line_of_file_into_vector(const T& body)
{
    std::vector<std::string> result;
    std::stringstream body_stream(body.c_str());
    std::string line;
    while (std::getline(body_stream, line))
    {
        result.push_back(line);
    }
    return result;
}

std::vector<std::string> split(const std::string& list, char token)
{
    std::vector<std::string> fields;
    boost::split(fields, list, boost::is_any_of(std::string{token}), boost::token_compress_on);
    return fields;
}

struct FakeData
{
    char data[156];
};

struct TestCancelCallbacks
{
    RallyHereGameInstanceAdapterPtr adapter;
    FakeData *fake_data;
    bool allocated_called;
    RallyHereStatusCode allocated_result;
};

static void on_allocated_callback_cancel(RallyHereStringMapPtr allocation_info, const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCancelCallbacks*>(user_data);
    assert(data->allocated_called == false);
    data->allocated_result = code;
    data->allocated_called = true;
    rallyhere::allocator::destroy(data->fake_data);
    rallyhere_string_map_destroy(allocation_info);
}

static void on_ready_callback_cancel(const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCancelCallbacks*>(user_data);
    assert(data->allocated_called == false);
    data->allocated_result = code;
    data->allocated_called = true;
    rallyhere::allocator::destroy(data->fake_data);
}

class logger {
public:
    logger() = default;
    logger(const logger &) = default;
    logger &operator=(const logger &) = default;
    void log(RallyHereLogLevel level, std::string_view v)
    {
        if (level == RH_LOG_LEVEL_ERROR)
        {
            std::cerr << v << std::endl;
        }
        else
        {
            std::cout << v << std::endl;
        }
    }

    template<typename... T>
    void log(RallyHereLogLevel level, fmt::format_string<T...> fmt, T&&... args)
    {
        rallyhere::memory_buffer buffer;
        fmt::format_to(std::back_inserter(buffer), fmt, std::forward<T>(args)...);
        std::string_view v{ buffer.data(), buffer.size() };
        log(level, v);
    }
};

logger log()
{
    return logger();
}

class session : public std::enable_shared_from_this<session>
{
public:
    using callback_function_t = std::function<void(session&)>;

public:
    explicit session(net::any_io_executor ex) : m_Resolver(ex), m_Stream(ex), m_Cancelled{false}
    {
    }

    ~session()
    {
    }

    void cancel_handler(boost::asio::cancellation_type_t cancellation_type)
    {
        m_Cancelled = true;
        m_Resolver.cancel();
        beast::get_lowest_layer(m_Stream).cancel();
    }

    void run(const rallyhere::string& in_url_str, http::request<rallyhere::string_body>&& request, callback_function_t callback, logger logger, bool log_request)
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
                boost::asio::bind_allocator(rallyhere::allocator::allocator<int>{}, boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session::on_resolve, shared_from_this()))));
    }

    template<typename Rep, typename Period>
    void run(const rallyhere::string& in_url_str, http::request<rallyhere::string_body>&& request, callback_function_t callback, logger logger, bool log_request, std::chrono::duration<Rep, Period> timeout)
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
        beast::get_lowest_layer(m_Stream).expires_after(std::chrono::seconds(30));

        // Make the connection on the IP address we get from a lookup
        beast::get_lowest_layer(m_Stream).async_connect(
                results,
                boost::asio::bind_allocator(rallyhere::allocator::allocator<int>{}, boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session::on_connect, shared_from_this()))));
    }

    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
    {
        if (check_cancelled()) return;
        if (ec)
        {
            failure(ec, "connect");
            return;
        }

        // Set a timeout on the operation
        beast::get_lowest_layer(m_Stream).expires_after(std::chrono::seconds(30));
        if (m_LogRequest)
            m_Logger.log(RH_LOG_LEVEL_TRACE, "{}", m_Request);
        http::async_write(
                m_Stream,
                m_Request,
                boost::asio::bind_allocator(rallyhere::allocator::allocator<int>{}, boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session::on_write, shared_from_this()))));
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
                boost::asio::bind_allocator(rallyhere::allocator::allocator<int>{}, boost::asio::bind_cancellation_slot(m_CancelSignal.slot(), beast::bind_front_handler(&session::on_read, shared_from_this()))));
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
        m_Stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        if (ec && ec != beast::errc::not_connected)
            return failure(ec, "shutdown");
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
    beast::basic_flat_buffer<rallyhere::allocator::allocator<char>> m_Buffer;
    http::request<rallyhere::string_body> m_Request;
    http::response<rallyhere::string_body> m_Response;
    rallyhere::Status m_Status{ RH_STATUS_OK };
    boost::system::error_code m_SystemErrorCode{};
    rallyhere::string m_SourceUrl;
    boost::urls::url_view m_Url;
    callback_function_t m_Callback;
    logger m_Logger{};
    bool m_Cancelled{ false };
    boost::asio::cancellation_slot m_CancellationSlot;
    boost::asio::cancellation_signal m_CancelSignal;
    std::chrono::milliseconds m_Timeout{ 30000 };
    bool m_LogRequest{ false };
};

std::pair<http::request<rallyhere::string_body>, boost::system::error_code> BuildMetricsRequest(const rallyhere::string& in_url_str)
{
    http::request<rallyhere::string_body> m_Request;
    rallyhere::string m_SourceUrl;
    boost::urls::url_view m_Url;

    m_SourceUrl = in_url_str;
    try
    {
        auto result = boost::urls::parse_uri(m_SourceUrl);
        if (!result)
        {
            log().log(RH_LOG_LEVEL_ERROR, "BuildRawStatsRequest: parse_uri failed: {} source {}", result.error(), m_SourceUrl);
            return { m_Request, result.error() };
        }
        m_Url = *result;
    }
    catch (const std::length_error& e)
    {
        log().log(RH_LOG_LEVEL_ERROR, "BuildRawStatsRequest: parse_uri failed: url is too long source {}", m_SourceUrl);
        return { m_Request, boost::system::error_code{ boost::system::errc::invalid_argument, boost::system::generic_category() } };
    }
    m_Request.method(http::verb::get);
    m_Request.target(m_Url.encoded_path());
    m_Request.set(http::field::host, m_Url.encoded_host());
    m_Request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    return { m_Request, {} };
}

void get_raw_metrics(lest::env & lest_env, RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr metrics)
{
    boost::asio::io_context m_IoContext{};
    auto session_ptr = std::make_shared<session>(net::make_strand(m_IoContext));
    auto url = "http://127.0.0.1:23890/metrics";
    auto request_pair = BuildMetricsRequest(url);
    EXPECT(!request_pair.second);

    bool metrics_ready = false;
    auto wrapper = [&](session& session) {
        EXPECT(session.m_Status);
        metrics_ready = true;
        auto lines = read_each_line_of_file_into_vector(session.m_Response.body());
        for (auto&& line : lines)
        {
            if (!boost::algorithm::starts_with(line, "instance_info"))
                continue;
            // Parse a line of text that looks like instance_info{label1="value1",label2="A longer value to",folder="",game="",game_mode="koth",map="",name="",version=""} 1
            auto pos = line.find_first_of('{');
            if (pos == rallyhere::string::npos)
                continue;
            auto pos2 = line.find_first_of('}', pos);
            if (pos2 == rallyhere::string::npos)
                continue;
            auto labels = line.substr(pos + 1, pos2 - pos - 1);
            auto label_pairs = split(labels, ',');
            for (auto&& label_pair : label_pairs)
            {
                auto label = split(label_pair, '=');
                if (label.size() != 2)
                    continue;
                auto key = label[0];
                auto value = label[1];
                if (value.size() > 2 && value[0] == '"' && value[value.size() - 1] == '"')
                {
                    value = value.substr(1, value.size() - 2);
                }
                rallyhere_string_map_set(metrics, key.c_str(), value.c_str());
            }
        }
    };
    session_ptr->run(url, std::move(request_pair.first), wrapper, log(), true, DEFAULT_WAIT);

    auto start = std::chrono::steady_clock::now();
    while (!metrics_ready)
    {
        if (m_IoContext.stopped())
            m_IoContext.restart();
        m_IoContext.poll();
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
        auto ongoing = std::chrono::steady_clock::now();
        auto elapsed = ongoing - start;
        EXPECT(elapsed < DEFAULT_WAIT);
    }
}

struct MetricEntry
{
    std::string m_Name;
    std::string m_Value;
    std::map<std::string, std::string> m_Labels;
};

auto get_field_value_and_labels_handler(lest::env & lest_env, RallyHereGameInstanceAdapterPtr adapter, bool& executed, std::map<std::string, MetricEntry>& entries)
{
    return [&](session& session) {
        EXPECT(session.m_Status);
        executed = true;
        auto lines = read_each_line_of_file_into_vector(session.m_Response.body());
        for (auto&& line : lines)
        {
            MetricEntry entry{};
            // Parse a line of text that looks like instance_info{label1="value1",label2="A longer value to",folder="",game="",game_mode="koth",map="",name="",version=""} 1
            auto pos = line.find_first_of('{');
            if (pos == rallyhere::string::npos)
                continue;
            entry.m_Name = line.substr(0, pos);
            auto pos2 = line.find_first_of('}', pos);
            if (pos2 == rallyhere::string::npos)
                continue;
            auto labels = line.substr(pos + 1, pos2 - pos - 1);
            auto label_pairs = split(labels, ',');
            for (auto&& label_pair : label_pairs)
            {
                auto label = split(label_pair, '=');
                if (label.size() != 2)
                    continue;
                auto key = label[0];
                auto value = label[1];
                if (value.size() > 2 && value[0] == '"' && value[value.size() - 1] == '"')
                {
                    value = value.substr(1, value.size() - 2);
                }
                else if (value == "\"\"")
                {
                    value = "";
                }
                entry.m_Labels[key] = value;
            }
            if (++pos2 == rallyhere::string::npos)
                continue;
            if (++pos2 == rallyhere::string::npos)
                continue;
            entry.m_Value = line.substr(pos2);
            entries[entry.m_Name] = entry;
        }
    };
}

auto get_metric_entries(lest::env & lest_env, RallyHereGameInstanceAdapterPtr adapter)
{
    boost::asio::io_context m_IoContext{};
    auto session_ptr = std::make_shared<session>(net::make_strand(m_IoContext));
    auto url = "http://127.0.0.1:23890/metrics";
    auto request_pair = BuildMetricsRequest(url);
    EXPECT(!request_pair.second);

    bool metrics_ready = false;
    std::map<std::string, MetricEntry> entries;
    auto wrapper = get_field_value_and_labels_handler(lest_env, adapter, metrics_ready, entries);
    session_ptr->run(url, std::move(request_pair.first), wrapper, log(), true, DEFAULT_WAIT);

    auto start = std::chrono::steady_clock::now();
    while (!metrics_ready)
    {
        if (m_IoContext.stopped())
            m_IoContext.restart();
        m_IoContext.poll();
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
        auto ongoing = std::chrono::steady_clock::now();
        auto elapsed = ongoing - start;
        EXPECT(elapsed < DEFAULT_WAIT);
    }
    return entries;
}

void get_instance_info_labels(lest::env & lest_env, RallyHereGameInstanceAdapterPtr adapter, std::map<std::string, std::string>& metric_labels)
{
    auto entries = get_metric_entries(lest_env, adapter);
    MetricEntry instance_info{};
    EXPECT_NO_THROW(instance_info = entries.at("instance_info"));
    metric_labels = instance_info.m_Labels;
}

void get_max_allowed_players(lest::env & lest_env, RallyHereGameInstanceAdapterPtr adapter, MetricEntry& max)
{
    auto entries = get_metric_entries(lest_env, adapter);
    EXPECT_NO_THROW(max = entries.at("max_allowed_players_total"));
}

void get_connected_clients(lest::env & lest_env, RallyHereGameInstanceAdapterPtr adapter, MetricEntry& max)
{
    auto entries = get_metric_entries(lest_env, adapter);
    EXPECT_NO_THROW(max = entries.at("connected_clients_total"));
}


//@formatter:off
// clang-format off
static const lest::test module[] = {
    CASE("Test instance info labels set directly as labels")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;

        RallyHereStringMapPtr labels;
        EXPECT(rallyhere_string_map_create(&labels) == RH_STATUS_OK);
        BOOST_SCOPE_EXIT_ALL(labels) {
             rallyhere_string_map_destroy(labels);
        };

        std::map<std::string, std::string> expected_instance_info;
        expected_instance_info["sic_id"] = "sic_unknownserverid_9000";
        expected_instance_info["sic_state"] = "Deallocated";
        EXPECT(rallyhere_string_map_set(labels, "label1", "value1") == RH_STATUS_OK);
        expected_instance_info["label1"] = "value1";
        const char *longer_value = "A longer value to use";
        std::string_view labels_str{"A longer value to use", strlen(longer_value) - 4};
        EXPECT(rallyhere_string_map_setn(labels, "label2", labels_str.data(), labels_str.size()) == RH_STATUS_OK);
        expected_instance_info["label2"] = labels_str;

        const char* value_to_check = nullptr;
        unsigned int value_size = 0;
        EXPECT(rallyhere_string_map_get(labels, "label1", &value_to_check, &value_size) == RH_STATUS_OK);
        EXPECT(0 == strcmp("value1", value_to_check));
        EXPECT(rallyhere_string_map_get(labels, "label2", &value_to_check, &value_size) == RH_STATUS_OK);
        EXPECT(value_size == labels_str.size());
        EXPECT(value_size == strlen(longer_value) - 4);
        EXPECT(0 == memcmp(labels_str.data(), value_to_check, value_size));

        rallyhere_set_labels(adapter, labels, on_set_labels_callback, &data);

        start = std::chrono::steady_clock::now();
        while (!data.set_labels_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.set_labels_result == RH_STATUS_OK);

        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        rallyhere_set_labels(adapter, labels, on_set_labels_callback, &data);

        start = std::chrono::steady_clock::now();
        while (!data.set_labels_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.set_labels_result == RH_STATUS_PROMETHEUS_ALREADY_STARTED);

        RallyHereStringMapPtr additional_info;
        EXPECT(rallyhere_string_map_create(&additional_info) == RH_STATUS_OK);
        BOOST_SCOPE_EXIT_ALL(additional_info) {
            rallyhere_string_map_destroy(additional_info);
        };
        EXPECT(rallyhere_string_map_set(additional_info, "map", "thecoolestmap") == RH_STATUS_OK);
        // Map is handled by the stats base labels now, so this should be ignored essentially
        expected_instance_info["map"] = "";
        EXPECT(rallyhere_string_map_set(additional_info, "map_override", "thecoolestmap") == RH_STATUS_OK);
        expected_instance_info["map_override"] = "thecoolestmap";
        EXPECT(rallyhere_string_map_set(additional_info, "game_mode", "koth") == RH_STATUS_OK);
        expected_instance_info["game_mode"] = "koth";
        rallyhere_set_additional_info(adapter, additional_info, on_set_additional_info_callback, &data);

        start = std::chrono::steady_clock::now();
        while (!data.set_additional_info_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.set_additional_info_result == RH_STATUS_OK);

        std::map<std::string, std::string> labels_map;
        get_instance_info_labels(lest_env, adapter, labels_map);
        for (auto&& p : expected_instance_info)
        {
            EXPECT(labels_map.find(p.first) != labels_map.end());
            EXPECT(labels_map[p.first] == p.second);
        }
    },
    CASE("Test instance info labels coming from stats base")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;

        RallyHereStringMapPtr labels;
        EXPECT(rallyhere_string_map_create(&labels) == RH_STATUS_OK);
        BOOST_SCOPE_EXIT_ALL(labels) {
             rallyhere_string_map_destroy(labels);
        };

        std::map<std::string, std::string> expected_instance_info;
        RallyHereStatsBase base{
            .name = "the best test server",
            .map = "test map",
            .folder = "/test/folder",
            .game = "Annihilation",
            .id = 3899,
            .players = 0,
            .max_players = 100,
            .bots = 100,
            .server_type = 'd',
            .environment = 'l',
            .visibility = 1,
            .anticheat = 0,
            .version = "0.0.1",
        };
        expected_instance_info["name"] = "the best test server";
        expected_instance_info["map"] = "test map";
        expected_instance_info["folder"] = "/test/folder";
        expected_instance_info["game"] = "Annihilation";
        RallyHereStatsBaseProvided provided{};
        memset(&provided, 0xff, sizeof(provided));
        EXPECT(rallyhere_stats_base(adapter, &base, &provided, on_set_base_stats_callback, &data) == RH_STATUS_OK);

        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        std::map<std::string, std::string> labels_map;
        get_instance_info_labels(lest_env, adapter, labels_map);
        for (auto&& p : expected_instance_info)
        {
            EXPECT(labels_map.find(p.first) != labels_map.end());
            EXPECT(labels_map[p.first] == p.second);
        }
    },
    CASE("Test instance info labels coming from stats base not clobbered by other labels")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;

        RallyHereStringMapPtr labels;
        EXPECT(rallyhere_string_map_create(&labels) == RH_STATUS_OK);
        BOOST_SCOPE_EXIT_ALL(labels) {
             rallyhere_string_map_destroy(labels);
        };

        std::map<std::string, std::string> expected_instance_info;
        RallyHereStatsBase base{
            .name = "the best test server",
            .map = "test map",
            .folder = "/test/folder",
            .game = "Annihilation",
            .id = 3899,
            .players = 0,
            .max_players = 100,
            .bots = 100,
            .server_type = 'd',
            .environment = 'l',
            .visibility = 1,
            .anticheat = 0,
            .version = "0.0.1",
        };
        expected_instance_info["name"] = "the best test server";
        expected_instance_info["map"] = "test map";
        expected_instance_info["folder"] = "/test/folder";
        expected_instance_info["game"] = "Annihilation";
        RallyHereStatsBaseProvided provided{};
        memset(&provided, 0xff, sizeof(provided));
        EXPECT(rallyhere_stats_base(adapter, &base, &provided, on_set_base_stats_callback, &data) == RH_STATUS_OK);

        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        std::map<std::string, std::string> labels_map;
        get_instance_info_labels(lest_env, adapter, labels_map);
        for (auto&& p : expected_instance_info)
        {
            EXPECT(labels_map.find(p.first) != labels_map.end());
            EXPECT(labels_map[p.first] == p.second);
        }

        RallyHereStringMapPtr additional_info;
        EXPECT(rallyhere_string_map_create(&additional_info) == RH_STATUS_OK);
        BOOST_SCOPE_EXIT_ALL(additional_info) {
            rallyhere_string_map_destroy(additional_info);
        };
        EXPECT(rallyhere_string_map_set(additional_info, "map", "thecoolestmap") == RH_STATUS_OK);
        EXPECT(rallyhere_string_map_set(additional_info, "game", "koth") == RH_STATUS_OK);
        rallyhere_set_additional_info(adapter, additional_info, on_set_additional_info_callback, &data);

        start = std::chrono::steady_clock::now();
        while (!data.set_additional_info_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.set_additional_info_result == RH_STATUS_OK);

        labels_map.clear();
        get_instance_info_labels(lest_env, adapter, labels_map);
        for (auto&& p : expected_instance_info)
        {
            EXPECT(labels_map.find(p.first) != labels_map.end());
            EXPECT(labels_map[p.first] == p.second);
        }
    },
    CASE("Test instance info labels coming environment variables")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        EXPECT(0 == putenv("SIC_PROMETHEUS_ADDITIONAL_LABELS=site_id:10,game_name:thetest,profile_id:thetest_10_sic,hostname:here.and.now.local,dstack:5"));
        EXPECT(0 == putenv("SIC_PROMETHEUS_ADDITIONAL_INFO_LABELS=server_id:a8u7sth3sthaus8sth"));
        std::map<std::string, std::string> expected_instance_info;
        expected_instance_info["site_id"] = "10";
        expected_instance_info["game_name"] = "thetest";
        expected_instance_info["profile_id"] = "thetest_10_sic";
        expected_instance_info["hostname"] = "here.and.now.local";
        expected_instance_info["dstack"] = "5";
        expected_instance_info["server_id"] = "a8u7sth3sthaus8sth";
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{};
        BOOST_SCOPE_EXIT_ALL(adapter) {
            rallyhere_destroy_game_instance_adapter(adapter);
        };
        data.adapter = adapter;

        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        std::map<std::string, std::string> labels_map;
        get_instance_info_labels(lest_env, adapter, labels_map);
        for (auto&& p : expected_instance_info)
        {
            EXPECT(labels_map.find(p.first) != labels_map.end());
            EXPECT(labels_map[p.first] == p.second);
        }

#if PLATFORM_LINUX
        unsetenv("SIC_PROMETHEUS_ADDITIONAL_LABELS");
        unsetenv("SIC_PROMETHEUS_ADDITIONAL_INFO_LABELS");
#else
        EXPECT(0 == putenv("SIC_PROMETHEUS_ADDITIONAL_LABELS="));
        EXPECT(0 == putenv("SIC_PROMETHEUS_ADDITIONAL_INFO_LABELS="));
#endif
    },
    CASE("Test instance info labels correct after allocation")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;

        RallyHereStringMapPtr labels;
        EXPECT(rallyhere_string_map_create(&labels) == RH_STATUS_OK);
        BOOST_SCOPE_EXIT_ALL(labels) {
             rallyhere_string_map_destroy(labels);
        };

        std::map<std::string, std::string> expected_instance_info;
        RallyHereStatsBase base{
            .name = "the best test server",
            .map = "test map",
            .folder = "/test/folder",
            .game = "Annihilation",
            .id = 3899,
            .players = 0,
            .max_players = 100,
            .bots = 100,
            .server_type = 'd',
            .environment = 'l',
            .visibility = 1,
            .anticheat = 0,
            .version = "0.0.1",
        };
        expected_instance_info["name"] = "the best test server";
        expected_instance_info["map"] = "test map";
        expected_instance_info["folder"] = "/test/folder";
        expected_instance_info["game"] = "Annihilation";
        RallyHereStatsBaseProvided provided{};
        memset(&provided, 0xff, sizeof(provided));
        EXPECT(rallyhere_stats_base(adapter, &base, &provided, on_set_base_stats_callback, &data) == RH_STATUS_OK);

        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        std::map<std::string, std::string> labels_map;
        get_instance_info_labels(lest_env, adapter, labels_map);
        for (auto&& p : expected_instance_info)
        {
            EXPECT(labels_map.find(p.first) != labels_map.end());
            EXPECT(labels_map[p.first] == p.second);
        }

        RallyHereStringMapPtr additional_info;
        EXPECT(rallyhere_string_map_create(&additional_info) == RH_STATUS_OK);
        BOOST_SCOPE_EXIT_ALL(additional_info) {
            rallyhere_string_map_destroy(additional_info);
        };
        EXPECT(rallyhere_string_map_set(additional_info, "map", "thecoolestmap") == RH_STATUS_OK);
        EXPECT(rallyhere_string_map_set(additional_info, "game", "koth") == RH_STATUS_OK);
        rallyhere_set_additional_info(adapter, additional_info, on_set_additional_info_callback, &data);

        start = std::chrono::steady_clock::now();
        while (!data.set_additional_info_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.set_additional_info_result == RH_STATUS_OK);

        labels_map.clear();
        get_instance_info_labels(lest_env, adapter, labels_map);
        for (auto&& p : expected_instance_info)
        {
            EXPECT(labels_map.find(p.first) != labels_map.end());
            EXPECT(labels_map[p.first] == p.second);
        }

        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        rallyhere_test_fake_allocation_response(
                adapter,
                "{\"items\":[{\"success\":true,\"error_code\":0,\"error_message\":null,\"state\":\"allocated\",\"allocation_id\":\"hey there its some test data\",\"session_id\":\"never \u2603\uFE0F\U0001F37B done a session before\"}]}");
        start = std::chrono::steady_clock::now();
        while (!data.allocated_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.allocated_called == true);
        EXPECT(data.allocated_result == RH_STATUS_OK);

        expected_instance_info["sic_state"] = "Allocated";
        labels_map.clear();
        get_instance_info_labels(lest_env, adapter, labels_map);
        for (auto&& p : expected_instance_info)
        {
            EXPECT(labels_map.find(p.first) != labels_map.end());
            EXPECT(labels_map[p.first] == p.second);
        }
    },
    CASE("Max players starts at zero")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        MetricEntry max;
        get_max_allowed_players(lest_env, adapter, max);
        EXPECT(max.m_Value == "0");
    },
    CASE("Max players starts at the default")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        arguments_source.push_back("rhdefaultreportmaxplayers=5");
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        MetricEntry max;
        get_max_allowed_players(lest_env, adapter, max);
        EXPECT(max.m_Value == "5");
    },
    CASE("Max players starts at the default and is updated")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        arguments_source.push_back("rhdefaultreportmaxplayers=5");
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        MetricEntry max;
        get_max_allowed_players(lest_env, adapter, max);
        EXPECT(max.m_Value == "5");

        RallyHereStatsBase base{.max_players = 10};
        RallyHereStatsBaseProvided provided{.set_max_players = true};
        EXPECT(rallyhere_stats_base(adapter, &base, &provided, on_set_base_stats_callback, &data) == RH_STATUS_OK);

        get_max_allowed_players(lest_env, adapter, max);
        EXPECT(max.m_Value == "10");
    },
    CASE("Max players starts at the forced value")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        arguments_source.push_back("rhforcereportmaxplayers=6");
        arguments_source.push_back("rhdefaultreportmaxplayers=3");
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        MetricEntry max;
        get_max_allowed_players(lest_env, adapter, max);
        EXPECT(max.m_Value == "6");
    },
    CASE("Max players starts at the forced value and can't be changed")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        arguments_source.push_back("rhforcereportmaxplayers=6");
        arguments_source.push_back("rhdefaultreportmaxplayers=3");
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        MetricEntry max;
        get_max_allowed_players(lest_env, adapter, max);
        EXPECT(max.m_Value == "6");

        RallyHereStatsBase base{.max_players = 10};
        RallyHereStatsBaseProvided provided{.set_max_players = true};
        EXPECT(rallyhere_stats_base(adapter, &base, &provided, on_set_base_stats_callback, &data) == RH_STATUS_OK);

        get_max_allowed_players(lest_env, adapter, max);
        EXPECT(max.m_Value == "6");
    },
    CASE("Current players starts at zero")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        arguments_source.push_back("rhforcereportmaxplayers=6");
        arguments_source.push_back("rhdefaultreportmaxplayers=3");
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        MetricEntry max;
        get_connected_clients(lest_env, adapter, max);
        EXPECT(max.m_Value == "0");
    },
    CASE("Current players can be updated")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        arguments_source.push_back("rhforcereportmaxplayers=6");
        arguments_source.push_back("rhdefaultreportmaxplayers=3");
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        MetricEntry max;
        get_connected_clients(lest_env, adapter, max);
        EXPECT(max.m_Value == "0");

        RallyHereStatsBase base{.players = 10};
        RallyHereStatsBaseProvided provided{.set_players = true};
        EXPECT(rallyhere_stats_base(adapter, &base, &provided, on_set_base_stats_callback, &data) == RH_STATUS_OK);

        get_connected_clients(lest_env, adapter, max);
        EXPECT(max.m_Value == "10");
    },
    CASE("Simulate player count changes over time")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        arguments_source.push_back("rhsimulationlock=players,max_players");
        arguments_source.push_back("rhsimulatecurrentplayersat=5,5");
        arguments_source.push_back("rhsimulatecurrentplayersat=10,6");
        arguments_source.push_back("rhsimulatecurrentplayersat=15,10");
        arguments_source.push_back("rhsimulatemaxplayersat=5,6");
        arguments_source.push_back("rhsimulatemaxplayersat=10,7");
        arguments_source.push_back("rhsimulatemaxplayersat=15,12");
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        MetricEntry max;
        get_connected_clients(lest_env, adapter, max);
        EXPECT(max.m_Value == "0");
        get_max_allowed_players(lest_env, adapter, max);
        EXPECT(max.m_Value == "0");

        ADAPTER_TICK_FOR_SECONDS(5);

        get_connected_clients(lest_env, adapter, max);
        EXPECT(max.m_Value == "5");
        get_max_allowed_players(lest_env, adapter, max);
        EXPECT(max.m_Value == "6");

        ADAPTER_TICK_FOR_SECONDS(5);

        get_connected_clients(lest_env, adapter, max);
        EXPECT(max.m_Value == "6");
        get_max_allowed_players(lest_env, adapter, max);
        EXPECT(max.m_Value == "7");

        ADAPTER_TICK_FOR_SECONDS(5);

        get_connected_clients(lest_env, adapter, max);
        EXPECT(max.m_Value == "10");
        get_max_allowed_players(lest_env, adapter, max);
        EXPECT(max.m_Value == "12");
    },
    CASE("Test hostname can be queried from the system")
    {
        auto arguments_source = demo_get_nohostname_arguments<rallyhere::string>();
        arguments_source.push_back("rhsichostnamequerylocal=yes");
        arguments_source.push_back("rhsicappendhostnametolabels=yes");
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        std::map<std::string, std::string> expected_instance_info;
        auto host_name = boost::asio::ip::host_name();
        expected_instance_info["hostname"] = host_name.c_str();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{};
        BOOST_SCOPE_EXIT_ALL(adapter) {
            rallyhere_destroy_game_instance_adapter(adapter);
        };
        data.adapter = adapter;

        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        std::map<std::string, std::string> labels_map;
        get_instance_info_labels(lest_env, adapter, labels_map);
        for (auto&& p : expected_instance_info)
        {
            EXPECT(labels_map.find(p.first) != labels_map.end());
            EXPECT(labels_map[p.first] == p.second);
        }
    },
    CASE("Test hostname can be set directly")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        arguments_source.push_back("rhsicappendhostnametolabels=yes");
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        std::map<std::string, std::string> expected_instance_info;
        expected_instance_info["hostname"] = "unknownhostname";
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{};
        BOOST_SCOPE_EXIT_ALL(adapter) {
            rallyhere_destroy_game_instance_adapter(adapter);
        };
        data.adapter = adapter;

        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        std::map<std::string, std::string> labels_map;
        get_instance_info_labels(lest_env, adapter, labels_map);
        for (auto&& p : expected_instance_info)
        {
            EXPECT(labels_map.find(p.first) != labels_map.end());
            EXPECT(labels_map[p.first] == p.second);
        }
    },
    CASE("Test that additional info labels works")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        std::map<std::string, std::string> expected_instance_info;
        expected_instance_info["addinfo"] = "nothanks";
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{};
        BOOST_SCOPE_EXIT_ALL(adapter) {
                                          rallyhere_destroy_game_instance_adapter(adapter);
                                      };
        data.adapter = adapter;

        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        std::map<std::string, std::string> labels_map;
        get_instance_info_labels(lest_env, adapter, labels_map);
        for (auto&& p : expected_instance_info)
        {
            EXPECT(labels_map.find(p.first) != labels_map.end());
            EXPECT(labels_map[p.first] == p.second);
        }
    }
};
//@formatter:on
// clang-format off

extern lest::tests& specification();

MODULE( specification(), module );