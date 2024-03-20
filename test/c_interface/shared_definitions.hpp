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
#pragma once

#include "configuration.h"

template<typename T>
auto demo_get_default_arguments()
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
        "rhsicappendlabels=testit:withvalue",
        "rhsicappendadditionalinfolabels=addinfo:nothanks"
    };
    return arguments;
}

// Helper function to join a vector of strings into a single string.
inline rallyhere::string join(const rallyhere::vector<rallyhere::string>& strings, const rallyhere::string& separator)
{
    rallyhere::string result;
    for (auto& s : strings)
    {
        if (!result.empty())
        {
            result += separator;
        }
        result += s;
    }
    return result;
}

#define SETUP_TEST_ADAPTER auto arguments = join(arguments_source, " "); \
RallyHereGameInstanceAdapterPtr adapter; \
rallyhere_global_init(); \
auto result = rallyhere_create_game_instance_adaptern(&adapter, arguments.c_str(), arguments.size()); \
EXPECT(rallyhere_is_error(result) == false); \
TestCCodeData data{}; \
BOOST_SCOPE_EXIT_ALL(adapter) { \
    rallyhere_destroy_game_instance_adapter(adapter); \
}; \
data.adapter = adapter;


#define ADAPTER_CONNECT rallyhere_connect(adapter, on_connect_callback, &data); \
auto start = std::chrono::steady_clock::now(); \
while (!data.connect_called) \
{ \
    EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK); \
    auto ongoing = std::chrono::steady_clock::now(); \
    auto elapsed = ongoing - start; \
    EXPECT(elapsed < DEFAULT_WAIT); \
} \
EXPECT(data.connect_result == RH_STATUS_OK);

#define ADAPTER_READY rallyhere_on_allocated_callback(adapter, on_allocated_callback, &data); \
auto start_reserve = std::chrono::steady_clock::now(); \
rallyhere_ready(adapter, on_ready_callback, &data); \
start = std::chrono::steady_clock::now(); \
while (!data.ready_called) \
{ \
    EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK); \
    auto ongoing = std::chrono::steady_clock::now(); \
    auto elapsed = ongoing - start; \
    EXPECT(elapsed < DEFAULT_WAIT); \
} \
EXPECT(data.ready_result == RH_STATUS_OK);

#define ADAPTER_HEALTHY EXPECT(rallyhere_healthy(adapter) == RH_STATUS_OK);
#define ADAPTER_TICK EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);

#define ADAPTER_TICK_FOR_SECONDS(secs) { \
    auto start = std::chrono::steady_clock::now(); \
    while (std::chrono::steady_clock::now() - start < std::chrono::seconds(secs)) \
    { \
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK); \
    } \
}
