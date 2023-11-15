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

#include "boost/filesystem.hpp"

static std::once_flag rally_here_url, credentials_file_path;

static std::string get_rally_here_url_impl()
{
    auto env_rally_here_url = getenv("RH_URL");
    if (env_rally_here_url)
    {
        return env_rally_here_url;
    }
    return "https://demo.rally-here.io";
}

const char* get_rally_here_url_arg()
{
    static std::string rally_here_url_value{};
    std::call_once(rally_here_url, []() {
        rally_here_url_value = std::string{"rallyhereurl="} + get_rally_here_url_impl();
    });
    return rally_here_url_value.c_str();
}

static std::string get_credentials_file_path_impl()
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
        credentials_file_path_value = std::string{"rhcredentialsfile="} + get_credentials_file_path_impl();
    });
    return credentials_file_path_value.c_str();
}