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
        "rhsicregistrationurl=http://127.0.0.1:8080/sic_registration/v3/register",
        "rhsicpollurl=http://127.0.0.1:8080/sic_registration/v1/state",
        "rhsicappendgrouptag=application_instance_id:387148274"
    };
    return arguments;
}

//@formatter:off
// clang-format off
static const lest::test module[] = {
    CASE("allocation local only [hide]")
    {
        auto arguments_source = get_default_arguments<rallyhere::string>();
        SETUP_TEST_ADAPTER;
        ADAPTER_CONNECT;
        rallyhere_on_allocated_callback(adapter, on_allocated_callback, &data);
        ADAPTER_READY;
        for (size_t i = 0; i < 10; ++i)
        {
            ADAPTER_HEALTHY;
            ADAPTER_TICK;
        }
    },
};
//@formatter:on
// clang-format off

extern lest::tests& specification();

MODULE( specification(), module );
