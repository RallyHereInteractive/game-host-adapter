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
#include "test_multiplay.h"
#include "c_api.h"
#include "rh_allocator.h"

#include "boost/scope_exit.hpp"

#include "shared_test_data.h"

using namespace test::multiplay;

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

template<typename StartT>
void tick_adapter(lest::env &lest_env, RallyHereGameInstanceAdapterPtr adapter, const StartT &start)
{
    EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
    auto ongoing = std::chrono::steady_clock::now();
    auto elapsed = ongoing - start;
    EXPECT(elapsed < DEFAULT_WAIT);
}

//@formatter:off
// clang-format off
static const lest::test module[] = {
    CASE("Multiplay fails to ready if no server json exists")
    {
        TestArguments<rallyhere::string> arguments;
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{};
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
            tick_adapter(lest_env, adapter, start);
        }
        EXPECT(data.ready_result == RH_STATUS_NO_SERVER_JSON);
    },
    CASE("Multiplay fails if server json is bad")
    {
        TestArguments<rallyhere::string> arguments;
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{};
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

        {
            ServerJson<rallyhere::string> sj{arguments.serverjsonpath};
            sj.write("Definitely not valid json");
            sj.write("{\"elocution_id\":\"aoeuaoeu\"}");
        }

        rallyhere_on_allocated_callback(adapter, on_allocated_callback, &data);
        auto start_reserve = std::chrono::steady_clock::now();
        rallyhere_ready(adapter, on_ready_callback, &data);

        start = std::chrono::steady_clock::now();
        while (!data.ready_called)
        {
            tick_adapter(lest_env, adapter, start);
        }
        EXPECT(data.ready_result == RH_STATUS_OK);

        start = std::chrono::steady_clock::now();
        while (!data.allocated_called)
        {
            tick_adapter(lest_env, adapter, start);
        }
        EXPECT(data.allocated_result == RH_STATUS_COULD_NOT_PARSE_SERVER_JSON);
    },
    CASE("Multiplay C interface reserve works")
    {
        TestArguments<rallyhere::string> arguments;
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{};
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

        rallyhere_reserve(adapter, 0, on_reserve_callback, &data, on_ready_callback, &data);

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
    CASE("Multiplay C interface reserve becomes ready after timeout")
    {
        TestArguments<rallyhere::string> arguments;
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{};
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

        {
            ServerJson<rallyhere::string> sj{arguments.serverjsonpath};
            sj.write("Definitely not valid json");
            sj.write("{\"elocution_id\":\"aoeuaoeu\"}");
        }

        {
            ServerJson<rallyhere::string> sj{arguments.serverjsonpath};
            sj.write("{\"allocation_id\":\"nonuniqueidfrommp\"}");
        }

        start = std::chrono::steady_clock::now();
        while (!data.ready_called)
        {
            tick_adapter(lest_env, adapter, start);
            {
                ServerJson<rallyhere::string> sj{arguments.serverjsonpath};
                sj.write("{\"allocation_id\":\"nonuniqueidfrommp\"}");
            }
        }
        EXPECT(data.ready_result == RH_STATUS_OK);
        EXPECT(data.ready_called == true);
    },
    CASE("Multiplay C interface ready to allocated works allocation and session")
    {
        TestArguments<rallyhere::string> arguments;
        RallyHereGameInstanceAdapterPtr adapter;
        rallyhere_global_init();
        auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
        EXPECT(rallyhere_is_error(result) == false);
        TestCCodeData data{};
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

        {
            ServerJson<rallyhere::string> sj{arguments.serverjsonpath};
            sj.write("{\"AllocationUUID\":\"hey there its some test data\"}\n");
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
        }
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

        for (size_t i = 0; i < 2; ++i)
        {
            ServerJson<rallyhere::string> sj{arguments.serverjsonpath};
            sj.write("{"
                     "\"AllocationUUID\":\"hey there its some test data\","
                     "\"SessionId\":\"never done a session before\","
                     "\"ConnectionIP\":\"thegreatest.game.com\","
                     "\"ConnectionPort\":\"6767\"}\n");
        }
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
        EXPECT(data.session_id == "never done a session before");
        EXPECT(data.public_host == "thegreatest.game.com");
        EXPECT(data.public_port == "6767");
    },
    CASE("Multiplay properly cancels callbacks on destroy")
    {
        TestArguments<rallyhere::string> arguments;
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
    }
};
//@formatter:on
// clang-format off

extern lest::tests& specification();

MODULE( specification(), module );