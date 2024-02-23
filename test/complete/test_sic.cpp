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
#include "allocator.h"
#include "sdk.h"
#include "c_api.h"

#include "rh_vector.h"
#include "rh_string.h"

#include "boost/scope_exit.hpp"
#include "boost/filesystem.hpp"

#include "shared_test_data.h"
#include "configuration.h"

template<typename T>
auto get_just_profileid_arguments()
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
auto get_default_arguments()
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
        "rhsicprometheusbind=0.0.0.0:23890"
    };
    return arguments;
}

// Helper function to join a vector of strings into a single string.
static rallyhere::string join(const rallyhere::vector<rallyhere::string>& strings, const rallyhere::string& separator)
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
    i3d::one::allocator::destroy(data->fake_data);
    rallyhere_string_map_destroy(allocation_info);
}

static void on_ready_callback_cancel(const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCancelCallbacks*>(user_data);
    assert(data->allocated_called == false);
    data->allocated_result = code;
    data->allocated_called = true;
    i3d::one::allocator::destroy(data->fake_data);
}

//@formatter:off
// clang-format off
static const lest::test module[] = {
    CASE("SIC mode works")
    {
        namespace one = i3d::one;
        auto arguments = get_default_arguments<const char *>();
        auto gia = one::allocator::create<rallyhere::GameInstanceAdapter>(arguments.size() - 2, arguments.data());
        EXPECT(gia->status().code() == RH_STATUS_OK);
        EXPECT(gia->good() == true);
        auto mode_name = gia->GetModeName();
        EXPECT(mode_name == "SIC");
        EXPECT(gia->GetSicId() == "sic_unknownserverid_9000");
        EXPECT(gia->GetSicGroupTags() == "profile_id:unknownprofileid,server_id:unknownserverid,hostname:unknownhostname");
        bool got_connect_result = false;
        rallyhere::Status connect_result;
        gia->Connect([&](const RallyHereStatusCode& status, void* user_data) {
            got_connect_result = true;
            connect_result = status;
        }, nullptr);
        auto start = std::chrono::steady_clock::now();
        while (!got_connect_result)
        {
            EXPECT(gia->Tick().ok() == true);
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < DEFAULT_WAIT);
        }
        EXPECT(connect_result.code() == RH_STATUS_OK);
        EXPECT(connect_result.ok() == true);
        one::allocator::destroy(gia);
    },
    CASE("SIC fails when no bootstrap specified")
    {
        namespace one = i3d::one;
        auto arguments = get_default_arguments<const char *>();
        auto gia = one::allocator::create<rallyhere::GameInstanceAdapter>(arguments.size() - 1, arguments.data() + 1);
        EXPECT(gia->good() == false);
        EXPECT(gia->status().code() == RH_STATUS_NO_BOOTSTRAP_MODE_PROVIDED);
        one::allocator::destroy(gia);
    }
};
//@formatter:on
// clang-format off

extern lest::tests& specification();

MODULE( specification(), module );