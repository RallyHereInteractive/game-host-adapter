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
#include "configuration.h"

#include <mutex>
#include <thread>
#include <filesystem>

#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"

static LaunchParameters launch_parameters{.use_user_pass = true,
                                          .use_clientid_secret = false,
                                          .generate_cred_file = false,
                                          .rallyhereenv = "Dev",
                                          .profiles_path = "../.test_data.json"};
static std::optional<Profile> profile{};
static std::once_flag rally_here_url, credentials_file_path, rh_credentials_as;

static void set_profile(const Profile& p)
{
    profile = p;
}

static void set_launch_parameters(const LaunchParameters& lp)
{
    launch_parameters = lp;
}

void handle_launch_parameters(int argc, char** argv)
{
    using namespace std;
    using namespace boost::program_options;

    LaunchParameters lp{};

    options_description desc("Allowed options");
    desc.add_options()
        ("generate-cred-file", bool_switch(&lp.generate_cred_file), "Should use the given profile to generate a credentials file and load that")
        ("use-user-pass", bool_switch(&lp.use_user_pass), "Use the username and password to log in or generate the cred file")
        ("use-clientid-secret", bool_switch(&lp.use_clientid_secret), "Use the client id and secret to log in or generate the cred file")
        ("rallyhereenv", value<std::string>(&lp.rallyhereenv)->default_value("Dev"), "Which environment to pick from available profiles")
        ("profiles-path", value<std::string>(&lp.profiles_path), "The path to the profiles.json file to use");
    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    Profiles profiles = load_profiles(lp.profiles_path);
    Profile profile = *std::find_if(profiles.profiles.begin(), profiles.profiles.end(), [&](Profile const& p) { return p.environment_id == lp.rallyhereenv; });
    set_profile(profile);

    if (!lp.use_user_pass && !lp.use_clientid_secret)
    {
        lp.use_user_pass = true;
    }
    set_launch_parameters(lp);

    if (lp.generate_cred_file)
    {
        auto cred_file_path = get_credentials_file_path();
        auto cred_file = std::ofstream(cred_file_path, std::ios::out | std::ios::trunc);
        if (lp.use_user_pass)
        {
            cred_file << profile.username << std::endl << profile.password;
        }
        else if (lp.use_clientid_secret)
        {
            cred_file << profile.client_id << std::endl << profile.client_secret;
        }
        cred_file.close();
    }
}

std::string get_rally_here_url()
{
    auto env_rally_here_url = getenv("RH_URL");
    if (env_rally_here_url)
    {
        return env_rally_here_url;
    }
    if (profile)
    {
        if (!profile->base_url.empty())
        {
            return profile->base_url;
        }
    }
    return "https://demo.rally-here.io";
}

const char* get_rally_here_url_arg()
{
    static std::string rally_here_url_value{};
    std::call_once(rally_here_url, []() {
        rally_here_url_value = std::string{"rallyhereurl="} + get_rally_here_url();
    });
    return rally_here_url_value.c_str();
}

std::string get_credentials_file_path()
{
    auto env_credentials_path = getenv("RH_CREDENTIALS_FILE");
    if (env_credentials_path)
    {
        return env_credentials_path;
    }
    return (boost::filesystem::temp_directory_path() / "credentials.json").generic_string();
}

const char* get_credentials_file_path_arg()
{
    static std::string credentials_file_path_value{};
    std::call_once(credentials_file_path, []() {
        credentials_file_path_value = std::string{"rhcredentialsfile="} + get_credentials_file_path();
    });
    return credentials_file_path_value.c_str();
}

const char* get_rh_credentials_as_arg()
{
    static std::string rh_credentials_as_value{};
    std::call_once(rh_credentials_as, []() {
        if (launch_parameters.use_user_pass)
        {
            rh_credentials_as_value = "rhcredentialsas=userpass";
        }
        else if (launch_parameters.use_clientid_secret)
        {
            rh_credentials_as_value = "rhcredentialsas=clientid";
        }
    });
    return rh_credentials_as_value.c_str();
}