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
auto get_default_arguments(rallyhere::string bootstrap, rallyhere::string provider)
{
    std::stringstream sstr;
    sstr << get_credentials_file_path_arg() << " " << get_rally_here_url_arg() << " " << get_rh_credentials_as_arg();
    if (!bootstrap.empty())
    {
        sstr << " -rhbootstrapmode=" << bootstrap;
    }
    if (!provider.empty())
    {
        sstr << " -ghaprovider=" << provider;
    }
    return sstr.str();
}

//@formatter:off
// clang-format off
static const lest::test module[] = {
    CASE("Bootstrap SIC is correct")
    {
        auto arguments = get_default_arguments<const char *>("SIC", "");
        rallyhere::GameInstanceAdapter adapter{arguments.c_str()};
        auto mode_name = adapter.GetModeName();
        EXPECT(mode_name == "SIC");
    },
    CASE("Provider SIC is correct")
    {
        auto arguments = get_default_arguments<const char *>("AutoCreate", "SIC");
        rallyhere::GameInstanceAdapter adapter{arguments.c_str()};
        auto mode_name = adapter.GetModeName();
        EXPECT(mode_name == "SIC");
    },
    CASE("Provider SIC is correct if bootstrap is multiplay")
    {
        auto arguments = get_default_arguments<const char *>("Multiplay", "SIC");
        rallyhere::GameInstanceAdapter adapter{arguments.c_str()};
        auto mode_name = adapter.GetModeName();
        EXPECT(mode_name == "SIC");
    },
    CASE("Provider SIC is correct if bootstrap is set to unknown")
    {
        auto arguments = get_default_arguments<const char *>("WallaWalla", "SIC");
        rallyhere::GameInstanceAdapter adapter{arguments.c_str()};
        auto mode_name = adapter.GetModeName();
        EXPECT(mode_name == "SIC");
    },
};
//@formatter:on
// clang-format off

extern lest::tests& specification();

MODULE( specification(), module );
