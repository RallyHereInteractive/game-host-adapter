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

#include "profile_reader.hpp"

struct LaunchParameters
{
    bool use_user_pass{false};
    bool use_clientid_secret{false};
    bool generate_cred_file{false};
    std::string rallyhereenv{"Dev"};
    std::string profiles_path{"../.test_data.json"};
};

void handle_launch_parameters(int argc, char** argv);

std::string get_rally_here_url();
const char* get_rally_here_url_arg();
std::string get_credentials_file_path();
const char* get_credentials_file_path_arg();
const char* get_rh_credentials_as_arg();