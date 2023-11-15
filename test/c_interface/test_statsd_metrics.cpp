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
#include "c_metrics.h"

#include "rh_vector.h"
#include "rh_string.h"

#include "boost/scope_exit.hpp"
#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"

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

//@formatter:off
// clang-format off
static const lest::test module[] = {
    CASE("Test immediate mode counter")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;

        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        RallyHereMetricDefinition definition{};
        definition.type = RALLY_HERE_METRIC_TYPE_COUNTER;
        definition.name = "TickCount";

        RallyHereMetricAction action{};
        action.type = RALLY_HERE_METRIC_ACTION_INCREMENT;
        action.action.increment.change = 1.0f;
        result = rallyhere_metrics_immediate_action(adapter, &definition, &action, RALLY_HERE_METRIC_FLUSH);
        EXPECT(result == RH_STATUS_OK);

        const char *output = nullptr;
        size_t output_size = 0;
        rallyhere_metrics_statsd_output(adapter, &output, &output_size);
        EXPECT(0 == strcmp(output, "TickCount:1|c\n"));
        rallyhere_metrics_graphite_output(adapter, &output, &output_size);
        EXPECT(0 == output_size);
    },
    CASE("Test immediate mode counter with definition and value labels")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;

        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        RallyHereMetricDefinition definition{};
        definition.type = RALLY_HERE_METRIC_TYPE_COUNTER;
        definition.name = "TickCount";
        definition.label_names = "game\0map\0\0";
        definition.label_values = "rallyheredemo\0thebestmap\0\0";

        RallyHereMetricAction action{};
        action.type = RALLY_HERE_METRIC_ACTION_INCREMENT;
        action.action.increment.change = 1.0f;
        action.label_names = "mode\0\0";
        action.label_values = "slow\0\0";
        rallyhere_metrics_immediate_action(adapter, &definition, &action, RALLY_HERE_METRIC_FLUSH);
        EXPECT(result == RH_STATUS_OK);

        const char *output = nullptr;
        size_t output_size = 0;
        rallyhere_metrics_statsd_output(adapter, &output, &output_size);
        EXPECT(0 == strcmp(output, "TickCount:1|c\n"));
        rallyhere_metrics_graphite_output(adapter, &output, &output_size);
        EXPECT(0 == output_size);
    },
    CASE("Test immediate mode gauge with labels ")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;

        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        RallyHereMetricDefinition definition{};
        definition.type = RALLY_HERE_METRIC_TYPE_GAUGE;
        definition.name = "MaxFrameTimeMs";
        definition.label_names = "game\0map\0\0";
        definition.label_values = "rallyheredemo\0thebestmap\0\0";

        RallyHereMetricAction action{};
        action.type = RALLY_HERE_METRIC_ACTION_SET;
        action.action.increment.change = 100.0f;
        action.label_names = "mode\0\0";
        action.label_values = "slow\0\0";
        rallyhere_metrics_immediate_action(adapter, &definition, &action, RALLY_HERE_METRIC_FLUSH);
        EXPECT(result == RH_STATUS_OK);

        const char *output = nullptr;
        size_t output_size = 0;
        rallyhere_metrics_statsd_output(adapter, &output, &output_size);
        EXPECT(0 == strcmp(output, "MaxFrameTimeMs:100|g\n"));
        rallyhere_metrics_graphite_output(adapter, &output, &output_size);
        EXPECT(0 == strcmp(output, "MaxFrameTimeMs;game=rallyheredemo;map=thebestmap;mode=slow 100 -1\n"));
    },
    CASE("Test immediate mode gauge")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;

        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        RallyHereMetricDefinition definition{};
        definition.type = RALLY_HERE_METRIC_TYPE_GAUGE;
        definition.name = "MaxFrameTimeMs";

        RallyHereMetricAction action{};
        action.type = RALLY_HERE_METRIC_ACTION_SET;
        action.action.increment.change = 100.0f;
        rallyhere_metrics_immediate_action(adapter, &definition, &action, RALLY_HERE_METRIC_FLUSH);
        EXPECT(result == RH_STATUS_OK);

        const char *output = nullptr;
        size_t output_size = 0;
        rallyhere_metrics_statsd_output(adapter, &output, &output_size);
        EXPECT(0 == strcmp(output, "MaxFrameTimeMs:100|g\n"));
        rallyhere_metrics_graphite_output(adapter, &output, &output_size);
        EXPECT(0 == strcmp(output, "MaxFrameTimeMs 100 -1\n"));
    },
    CASE("Test immediate mode gauge with time")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;

        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        RallyHereMetricDefinition definition{};
        definition.type = RALLY_HERE_METRIC_TYPE_GAUGE;
        definition.name = "MaxFrameTimeMs";

        RallyHereMetricAction action{};
        action.type = RALLY_HERE_METRIC_ACTION_SET;
        action.action.increment.change = 100.0f;
        auto original = 12341234;
        auto timestamp = static_cast<double>(original);
        timestamp += 0.25f;
        rallyhere_metrics_immediate_action_with_time(adapter, &definition, &action, timestamp, RALLY_HERE_METRIC_FLUSH);
        EXPECT(result == RH_STATUS_OK);

        const char *output = nullptr;
        size_t output_size = 0;
        rallyhere_metrics_statsd_output(adapter, &output, &output_size);
        EXPECT(0 == strcmp(output, "MaxFrameTimeMs:100|g\n"));
        rallyhere_metrics_graphite_output(adapter, &output, &output_size);
        EXPECT(0 == strcmp(output, "MaxFrameTimeMs 100 12341234\n"));
    },
    CASE("Test immediate mode gauges that wait for a flush")
    {
        auto arguments_source = demo_get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;

        ADAPTER_READY;
        ADAPTER_HEALTHY;
        ADAPTER_TICK;

        RallyHereMetricDefinition definition{};
        definition.type = RALLY_HERE_METRIC_TYPE_GAUGE;
        definition.name = "MaxFrameTimeMs";

        RallyHereMetricAction action{};
        action.type = RALLY_HERE_METRIC_ACTION_SET;
        action.action.increment.change = 100.0f;
        rallyhere_metrics_immediate_action(adapter, &definition, &action, RALLY_HERE_METRIC_NO_FLUSH);
        EXPECT(result == RH_STATUS_OK);

        const char *output = nullptr;
        size_t output_size = 0;
        rallyhere_metrics_statsd_output(adapter, &output, &output_size);
        EXPECT(0 == output_size);
        rallyhere_metrics_graphite_output(adapter, &output, &output_size);
        EXPECT(0 == output_size);

        definition.type = RALLY_HERE_METRIC_TYPE_TIMER;
        definition.name = "OnlyStatsDTime";

        action.type = RALLY_HERE_METRIC_ACTION_SET;
        action.action.increment.change = 100.0f;
        rallyhere_metrics_immediate_action(adapter, &definition, &action, RALLY_HERE_METRIC_NO_FLUSH);
        EXPECT(result == RH_STATUS_OK);

        rallyhere_metrics_statsd_output(adapter, &output, &output_size);
        EXPECT(0 == output_size);
        rallyhere_metrics_graphite_output(adapter, &output, &output_size);
        EXPECT(0 == output_size);

        definition.type = RALLY_HERE_METRIC_TYPE_GAUGE;
        definition.name = "NetworkDrift";

        action.type = RALLY_HERE_METRIC_ACTION_SET;
        action.action.increment.change = 100.53f;
        rallyhere_metrics_immediate_action(adapter, &definition, &action, RALLY_HERE_METRIC_FLUSH);
        EXPECT(result == RH_STATUS_OK);

        rallyhere_metrics_statsd_output(adapter, &output, &output_size);
        EXPECT(0 == strcmp(output, "MaxFrameTimeMs:100|g\nOnlyStatsDTime:100|ms\nNetworkDrift:100.52999877929688|g\n"));
        rallyhere_metrics_graphite_output(adapter, &output, &output_size);
        EXPECT(0 == strcmp(output, "MaxFrameTimeMs 100 -1\nNetworkDrift 100.52999877929688 -1\n"));
    },
};
//@formatter:on
// clang-format off

extern lest::tests& specification();

MODULE( specification(), module );