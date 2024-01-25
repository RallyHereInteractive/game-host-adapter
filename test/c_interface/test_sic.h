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

#include "lest/lest.hpp"
#include "configuration.h"
#include <list>

#include "rh_string.h"
#include "rh_vector.h"


/** @file test_sic.h
 * Tests GameInstanceAdapter in SIC mode
 */

namespace test
{
namespace sic
{

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

    template<typename T>
    struct TestArguments
    {
        rallyhere::vector<T> arguments;
        rallyhere::string arguments_str;

        TestArguments()
        {
            get_default_arguments();
        }

        void get_default_arguments()
        {
            arguments = {
                "rhbootstrapmode=SIC",
                get_credentials_file_path_arg(),
                get_rally_here_url_arg(),
                get_rh_credentials_as_arg(),
                "A2SPort=23891",
                "rhsicprofileid=unknownprofileid",
                "rhsicserverid=unknownserverid",
                "rhsichostname=unknownhostname",
                "PoRt=9000",
                "MULTIHOME=0.0.0.0",
                "rhsicid=sic_unknownserverid_9000",
                "rhsicgrouptags=profile_id:unknownprofileid,server_id:unknownserverid,hostname:unknownhostname",
                "rhsicprometheusbind=0.0.0.0:23890"
            };
            arguments_str = join(arguments, " ");
        }

        const char* c_str()
        {
            return arguments_str.c_str();
        }

        size_t size()
        {
            return arguments_str.size();
        }
    };
} // namespace sic
} // namespace test
