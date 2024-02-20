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

#include "boost/scope_exit.hpp"

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "a2s.hpp"
#include "shared_test_data.h"
#include "shared_a2s.h"

using boost::asio::ip::udp;
using namespace test::sic;

static RallyHereStatsBase get_stats_base()
{
    return {
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
        .visibility = '1',
        .anticheat = '0',
        .version = "0.0.1",
    };
}

static void compare(lest::env& lest_env, const RallyHereStatsBase& stats_base, const rallyhere::server_info& info)
{
    EXPECT(stats_base.name == info.Name);
    EXPECT(stats_base.map == info.Map);
    EXPECT(stats_base.folder == info.Folder);
    EXPECT(stats_base.game == info.GameName);
    EXPECT(stats_base.id == info.ID);
    EXPECT(stats_base.players == info.Players);
    EXPECT(stats_base.max_players == info.MaxPlayers);
    EXPECT(stats_base.bots == info.Bots);
    EXPECT(stats_base.server_type == info.ServerType);
    EXPECT(stats_base.environment == info.Environment);
    EXPECT(stats_base.visibility == info.Visibility);
    EXPECT(stats_base.anticheat == info.VAC);
    EXPECT(stats_base.version == info.Version);
}

static void get_ready(lest::env& lest_env, RallyHereGameInstanceAdapterPtr& adapter, TestCCodeData& data)
{
    TestArguments<rallyhere::string> arguments;
    rallyhere_global_init();
    auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size());
    EXPECT(rallyhere_is_error(result) == false);

    data.adapter = adapter;

    rallyhere_connect(adapter, on_connect_callback, &data);

    auto start = std::chrono::steady_clock::now();
    while (!data.connect_called)
    {
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
        auto ongoing = std::chrono::steady_clock::now();
        auto elapsed = ongoing - start;
        EXPECT(elapsed < std::chrono::seconds(10));
    }
    EXPECT(data.connect_result == RH_STATUS_OK);

    {
        RallyHereStatsBase base = get_stats_base();
        RallyHereStatsBaseProvided provided{};
        memset(&provided, 0xff, sizeof(provided));
        EXPECT(rallyhere_stats_base(adapter, &base, &provided, on_set_base_stats_callback, &data) == RH_STATUS_OK);
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
        EXPECT(elapsed < std::chrono::seconds(10));
    }
    EXPECT(data.ready_result == RH_STATUS_OK);

    EXPECT(rallyhere_healthy(adapter) == RH_STATUS_OK);
    EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);

    start = std::chrono::steady_clock::now();
    while (!data.set_base_stats_called)
    {
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
        auto ongoing = std::chrono::steady_clock::now();
        auto elapsed = ongoing - start;
        EXPECT(elapsed < std::chrono::seconds(10 + 2));
    }
    EXPECT(data.set_base_stats_called == true);
    EXPECT(data.set_base_stats_result == RH_STATUS_OK);
}

static void write_new_stats(lest::env& lest_env,
                            RallyHereGameInstanceAdapterPtr adapter,
                            TestCCodeData& data,
                            RallyHereStatsBase& base,
                            RallyHereStatsBaseProvided& provided)
{
    EXPECT(rallyhere_stats_base(adapter, &base, &provided, on_set_base_stats_callback, &data) == RH_STATUS_OK);
    data.set_base_stats_called = false;

    auto start = std::chrono::steady_clock::now();
    while (!data.set_base_stats_called)
    {
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
        auto ongoing = std::chrono::steady_clock::now();
        auto elapsed = ongoing - start;
        EXPECT(elapsed < std::chrono::seconds(10 + 2));
    }
    EXPECT(data.set_base_stats_called == true);
    EXPECT(data.set_base_stats_result == RH_STATUS_OK);
}

static void make_bad_request(lest::env& lest_env,
                             RallyHereGameInstanceAdapterPtr adapter,
                             TestCCodeData& data)
{
    boost::asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "localhost", "23891").begin();

    udp::socket socket(io_context);
    socket.open(udp::v4());

    std::string_view send_buf = { "\xff\xff\xff\x54Source Engine Query\0", 25 };
    socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
}

//@formatter:off
// clang-format off
static const lest::test module[] = {
    CASE("A2S starts with previously set information")
    {
        RallyHereGameInstanceAdapterPtr adapter{nullptr};
        TestCCodeData data{};
        get_ready(lest_env, adapter, data);
        BOOST_SCOPE_EXIT_ALL(adapter) {
            rallyhere_destroy_game_instance_adapter(adapter);
        };

        rallyhere::server_info info = get_stats(lest_env, adapter, data);
        compare(lest_env, get_stats_base(), info);
    },
    CASE("A2S updates after ready")
    {
        RallyHereGameInstanceAdapterPtr adapter{nullptr};
        TestCCodeData data{};
        get_ready(lest_env, adapter, data);
        BOOST_SCOPE_EXIT_ALL(adapter) {
            rallyhere_destroy_game_instance_adapter(adapter);
        };

        RallyHereStatsBase base = get_stats_base();
        base.version = "hoo ha";
        {
            RallyHereStatsBaseProvided provided{};
            memset(&provided, 0xff, sizeof(provided));
            write_new_stats(lest_env, adapter, data, base, provided);
        }

        rallyhere::server_info info = get_stats(lest_env, adapter, data);
        compare(lest_env, base, info);
    },
    CASE("A2S updates only the values specified")
    {
        RallyHereGameInstanceAdapterPtr adapter{nullptr};
        TestCCodeData data{};
        get_ready(lest_env, adapter, data);
        BOOST_SCOPE_EXIT_ALL(adapter) {
            rallyhere_destroy_game_instance_adapter(adapter);
        };

        RallyHereStatsBase prev = get_stats_base();
        RallyHereStatsBase current = get_stats_base();
        {
            RallyHereStatsBaseProvided provided{};
            memset(&provided, 0xff, sizeof(provided));
            write_new_stats(lest_env, adapter, data, prev, provided);
        }

        rallyhere::server_info info = get_stats(lest_env, adapter, data);
        compare(lest_env, prev, info);

        current.name = "hoo ha";
        current.map = "alllo";
        {
            RallyHereStatsBaseProvided provided{};
            provided.set_name = true;
            write_new_stats(lest_env, adapter, data, current, provided);
        }
        info = get_stats(lest_env, adapter, data);
        EXPECT(info.Name == current.name);
        EXPECT(info.Map == prev.map);
    },
    CASE("A2S survives bad requests")
    {
        RallyHereGameInstanceAdapterPtr adapter{nullptr};
        TestCCodeData data{};
        get_ready(lest_env, adapter, data);
        BOOST_SCOPE_EXIT_ALL(adapter) {
            rallyhere_destroy_game_instance_adapter(adapter);
        };

        make_bad_request(lest_env, adapter, data);
        rallyhere::server_info info = get_stats(lest_env, adapter, data);
        compare(lest_env, get_stats_base(), info);
    }
};
//@formatter:on
// clang-format off

extern lest::tests& specification();

MODULE( specification(), module );