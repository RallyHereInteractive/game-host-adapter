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

#include "rh_vector.h"
#include "rh_string.h"
#include "rh_allocator.h"

#include "boost/scope_exit.hpp"
#include "boost/filesystem.hpp"

#include "shared_test_data.h"
#include "configuration.h"

#include "shared_definitions.hpp"

#include "boost/asio.hpp"

template<typename T>
static auto get_just_profileid_arguments()
{
    rallyhere::vector<T> arguments = {
        "-rhbootstrapmode=SIC",
        get_credentials_file_path_arg(),
        get_rally_here_url_arg(),
        get_rh_credentials_as_arg(),
        "rhsicprofileid=unknownprofileid",
    };
    return arguments;
}

template<typename T>
static auto get_default_arguments()
{
    rallyhere::vector<T> arguments = {
        "-rhbootstrapmode=SIC",
        get_credentials_file_path_arg(),
        get_rally_here_url_arg(),
        get_rh_credentials_as_arg(),
        "rhsicprofileid=unknownprofileid",
        "rhsicserverid=unknownserverid",
        "rhsichostname=unknownhostname",
        "PoRt=9000",
        "MULTIHOME=0.0.0.0",
        "rhsicid=sic_unknownserverid_9000",
        "rhsicgrouptags=profile_id:unknownprofileid,server_id:unknownserverid,hostname:unknownhostname",
        "rhsicprometheusbind=0.0.0.0:23890",
    };
    return arguments;
}

template<typename T>
static auto get_soft_stop_default_arguments()
{
    rallyhere::vector<T> arguments = {
        "-rhbootstrapmode=SIC",
        get_credentials_file_path_arg(),
        get_rally_here_url_arg(),
        get_rh_credentials_as_arg(),
        "rhsicprofileid=unknownprofileid",
        "rhsicserverid=unknownserverid",
        "rhsichostname=unknownhostname",
        "PoRt=9000",
        "MULTIHOME=0.0.0.0",
        "rhsicid=sic_unknownserverid_9000",
        "rhsicgrouptags=profile_id:unknownprofileid,server_id:unknownserverid,hostname:unknownhostname",
        "rhsicprometheusbind=0.0.0.0:23890",
        "rhdefaultsoftstoptimeout=700"
    };
    return arguments;
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

//@formatter:off
// clang-format off
static const lest::test module[] = {
    CASE("SIC C interface reserve works")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{ lest_env };
        BOOST_SCOPE_EXIT_ALL(adapter) {
            rallyhere_destroy_game_instance_adapter(adapter);
        };

        data.adapter = adapter;

        rallyhere_connect(adapter, on_connect_callback, &data);

        auto start = std::chrono::steady_clock::now();
        while (!data.connect_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.connect_result == RH_STATUS_OK);

        rallyhere_reserve(adapter, 5, on_reserve_callback, &data, on_ready_callback, &data);

        start = std::chrono::steady_clock::now();
        while (!data.reserve_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.reserve_result == RH_STATUS_OK);

        EXPECT(rallyhere_healthy(adapter) == RH_STATUS_OK);
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);

        // Now something happens and we decide it's time to be allocated!
        EXPECT(data.ready_called == false);

        rallyhere_allocate(adapter, on_allocate_callback, &data);

        start = std::chrono::steady_clock::now();
        while (!data.allocate_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.allocate_result == RH_STATUS_OK);

        start = std::chrono::steady_clock::now();
        while (!data.ready_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            EXPECT(data.ready_called == false);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            if (elapsed < DEFAULT_WAIT)
            {
                break;
            }
        }
        EXPECT(data.ready_called == false);
    },
    CASE("SIC C interface reserve becomes ready after timeout")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{ lest_env };
        BOOST_SCOPE_EXIT_ALL(adapter) {
            rallyhere_destroy_game_instance_adapter(adapter);
        };

        data.adapter = adapter;

        rallyhere_connect(adapter, on_connect_callback, &data);

        auto start = std::chrono::steady_clock::now();
        while (!data.connect_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.connect_result == RH_STATUS_OK);

        // If we become ready we must have an on_allocated_callback in order to handle any immediate allocations
        rallyhere_on_allocated_callback(adapter, on_allocated_callback, &data);
        auto start_reserve = std::chrono::steady_clock::now();
        rallyhere_reserve(adapter, 5, on_reserve_callback, &data, on_ready_callback, &data);

        start = std::chrono::steady_clock::now();
        while (!data.reserve_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.reserve_result == RH_STATUS_OK);

        EXPECT(rallyhere_healthy(adapter) == RH_STATUS_OK);
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);

        start = std::chrono::steady_clock::now();
        while (!data.ready_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.ready_result == RH_STATUS_OK);
        EXPECT(data.ready_called == true);
    },
    CASE("SIC C interface ready to allocated works only allocation id")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{ lest_env };
        BOOST_SCOPE_EXIT_ALL(adapter) {
            rallyhere_destroy_game_instance_adapter(adapter);
        };
        data.adapter = adapter;

        rallyhere_connect(adapter, on_connect_callback, &data);

        auto start = std::chrono::steady_clock::now();
        while (!data.connect_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.connect_result == RH_STATUS_OK);

        RallyHereStringMapPtr labels;
        EXPECT(rallyhere_string_map_create(&labels) == RH_STATUS_OK);
        BOOST_SCOPE_EXIT_ALL(labels) {
             rallyhere_string_map_destroy(labels);
        };

        EXPECT(rallyhere_string_map_set(labels, "label1", "value1") == RH_STATUS_OK);
        const char *longer_value = "A longer value to use";
        std::string_view labels_str{"A longer value to use", strlen(longer_value) - 4};
        EXPECT(rallyhere_string_map_setn(labels, "label2", labels_str.data(), labels_str.size()) == RH_STATUS_OK);

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

        rallyhere_on_allocated_callback(adapter, on_allocated_callback, &data);
        auto start_reserve = std::chrono::steady_clock::now();
        rallyhere_ready(adapter, on_ready_callback, &data);

        start = std::chrono::steady_clock::now();
        while (!data.ready_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.ready_result == RH_STATUS_OK);

        EXPECT(rallyhere_healthy(adapter) == RH_STATUS_OK);
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);

        data.set_labels_called = false;
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
        EXPECT(rallyhere_string_map_set(additional_info, "game_mode", "koth") == RH_STATUS_OK);
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

        rallyhere_test_fake_allocation_response(
                adapter,
                "{\"items\":[{\"success\":true,\"error_code\":0,\"error_message\":null,\"state\":\"allocated\",\"allocation_id\":\"hey there its some test data\"}]}");
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
        EXPECT(data.allocation_id == "hey there its some test data");
        EXPECT(data.session_id == "");
        EXPECT(data.public_host == "unknownhostname");
        EXPECT(data.public_port == "9000");
    },
    CASE("SIC C interface ready to allocated works allocation and session")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{ lest_env };
        BOOST_SCOPE_EXIT_ALL(adapter) {
            rallyhere_destroy_game_instance_adapter(adapter);
        };
        data.adapter = adapter;

        rallyhere_connect(adapter, on_connect_callback, &data);

        auto start = std::chrono::steady_clock::now();
        while (!data.connect_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.connect_result == RH_STATUS_OK);

        rallyhere_on_allocated_callback(adapter, on_allocated_callback, &data);
        auto start_reserve = std::chrono::steady_clock::now();
        rallyhere_ready(adapter, on_ready_callback, &data);

        start = std::chrono::steady_clock::now();
        while (!data.ready_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.ready_result == RH_STATUS_OK);

        EXPECT(rallyhere_healthy(adapter) == RH_STATUS_OK);
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);

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
        EXPECT(data.allocation_id == "hey there its some test data");
        EXPECT(data.session_id == "never \u2603\uFE0F\U0001F37B done a session before");
        EXPECT(data.public_host == "unknownhostname");
        EXPECT(data.public_port == "9000");
    },
    CASE("SIC properly cancels callbacks on destroy")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);

        TestCancelCallbacks data{};
        data.adapter = adapter;
        data.fake_data = rallyhere::allocator::create<FakeData>();
        rallyhere_on_allocated_callback(adapter, on_allocated_callback_cancel, &data);
        rallyhere_destroy_game_instance_adapter(adapter);
        EXPECT(data.allocated_called == true);
        EXPECT(data.allocated_result == RH_STATUS_CANCELLED);
    },
    CASE("SIC cancels callbacks only in tick on destroy")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);

        TestCancelCallbacks data{};
        data.adapter = adapter;
        data.fake_data = rallyhere::allocator::create<FakeData>();
        rallyhere_ready(adapter, on_ready_callback_cancel, &data);
        rallyhere_destroy_game_instance_adapter(adapter);
        EXPECT(data.allocated_called == true);
        EXPECT(data.allocated_result == RH_STATUS_CANCELLED);
    },
    CASE("Processes different command line options")
    {
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adapter(&adapter, "rhbootstrapmode= rallyhereurl=");
        EXPECT(result == RH_STATUS_NO_RALLYHERE_URL_PROVIDED);
        result = rallyhere_create_game_instance_adapter(&adapter, "rhbootstrapmode= rallyhereurl=http://localhost:8080");
        EXPECT(result == RH_STATUS_NO_BOOTSTRAP_MODE_PROVIDED);
        result = rallyhere_create_game_instance_adapter(&adapter, "rhbootstrapmode= -rallyhereurl=http://localhost:8080");
        EXPECT(result == RH_STATUS_NO_BOOTSTRAP_MODE_PROVIDED);
        result = rallyhere_create_game_instance_adapter(&adapter, "rhbootstrapmode= --rallyhereurl=http://localhost:8080");
        EXPECT(result == RH_STATUS_NO_BOOTSTRAP_MODE_PROVIDED);
    },
    CASE("SIC handles only profile id")
    {
        auto arguments_source = get_just_profileid_arguments<rallyhere::string>();
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(result == RH_STATUS_OK);
        EXPECT(rallyhere_is_error(result) == false);
        rallyhere_destroy_game_instance_adapter(adapter);
    },
    CASE("SIC soft stop external triggers on tick")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        rallyhere_on_soft_stop_callback(adapter, on_soft_stop_callback, &data);
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        EXPECT(data.soft_stop_called_count == 0);

        rallyhere_external_soft_stop_requested(adapter);
        EXPECT(data.soft_stop_called_count == 0);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
    },
    CASE("SIC soft stop external triggers only once per request")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        rallyhere_on_soft_stop_callback(adapter, on_soft_stop_callback, &data);
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        EXPECT(data.soft_stop_called_count == 0);

        rallyhere_external_soft_stop_requested(adapter);
        EXPECT(data.soft_stop_called_count == 0);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
    },
    CASE("SIC soft stop external v2 triggers v1 on tick")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        rallyhere_on_soft_stop_callback(adapter, on_soft_stop_callback, &data);
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        EXPECT(data.soft_stop_called_count == 0);

        rallyhere_external_soft_stop_requested_v2(adapter, 100);
        EXPECT(data.soft_stop_called_count == 0);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
    },
    CASE("SIC soft stop external v2 triggers v1 only once per request")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        rallyhere_on_soft_stop_callback(adapter, on_soft_stop_callback, &data);
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        EXPECT(data.soft_stop_called_count == 0);

        rallyhere_external_soft_stop_requested_v2(adapter, 125);
        EXPECT(data.soft_stop_called_count == 0);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
    },
    CASE("SIC soft stop external v2 triggers v2 on tick")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        rallyhere_on_soft_stop_callback_v2(adapter, on_soft_stop_callback_v2, &data);
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        EXPECT(data.soft_stop_called_count == 0);

        rallyhere_external_soft_stop_requested_v2(adapter, 100);
        EXPECT(data.soft_stop_called_count == 0);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
        EXPECT(data.soft_stop_timeout == 100);
    },
    CASE("SIC soft stop external v2 triggers v2 only once per request")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        rallyhere_on_soft_stop_callback_v2(adapter, on_soft_stop_callback_v2, &data);
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        EXPECT(data.soft_stop_called_count == 0);

        rallyhere_external_soft_stop_requested_v2(adapter, 125);
        EXPECT(data.soft_stop_called_count == 0);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
        EXPECT(data.soft_stop_timeout == 125);
    },
    CASE("SIC soft stop external v1 triggers v2 on tick")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        rallyhere_on_soft_stop_callback_v2(adapter, on_soft_stop_callback_v2, &data);
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        EXPECT(data.soft_stop_called_count == 0);

        rallyhere_external_soft_stop_requested(adapter);
        EXPECT(data.soft_stop_called_count == 0);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
        EXPECT(data.soft_stop_timeout == -1);
    },
    CASE("SIC soft stop external v1 triggers v2 only once per request")
    {
        auto arguments_source = get_soft_stop_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        rallyhere_on_soft_stop_callback_v2(adapter, on_soft_stop_callback_v2, &data);
        ADAPTER_CONNECT;
        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        EXPECT(data.soft_stop_called_count == 0);

        rallyhere_external_soft_stop_requested(adapter);
        EXPECT(data.soft_stop_called_count == 0);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
        ADAPTER_TICK;
        EXPECT(data.soft_stop_called_count == 1);
        EXPECT(data.soft_stop_timeout == 700);
    },
    CASE("SIC prometheus binding fails early")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        auto arguments = join(arguments_source, " ");
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{ lest_env };
        BOOST_SCOPE_EXIT_ALL(adapter) {
                                          rallyhere_destroy_game_instance_adapter(adapter);
                                      };
        data.adapter = adapter;
        rallyhere_connect(adapter, on_connect_callback, &data);

        auto start = std::chrono::steady_clock::now();
        while (!data.connect_called)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(data.connect_result == RH_STATUS_OK);

        boost::asio::io_context io_context;
        boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 23890));
        io_context.run_for(std::chrono::seconds(1));

        rallyhere_on_allocated_callback(adapter, on_allocated_callback, &data);
        auto start_reserve = std::chrono::steady_clock::now();
        rallyhere_ready(adapter, on_ready_callback, &data);
        EXPECT(data.ready_result == RH_STATUS_PROMETHEUS_COULD_NOT_START);
    },
};
//@formatter:on
// clang-format off

extern lest::tests& specification();

MODULE( specification(), module );
