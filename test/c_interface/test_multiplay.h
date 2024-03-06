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
#include <fstream>

/** @file test_multiplay.h
 * Tests GameInstanceAdapter in Multiplay mode
 */

namespace test
{
namespace multiplay
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
    rallyhere::string serverjsonpath;

    TestArguments()
    {
        serverjsonpath = std::tmpnam(nullptr);
        get_default_arguments();
    }

    void get_default_arguments()
    {
        arguments = {
            "rhbootstrapmode=Multiplay",
            get_rally_here_url_arg(),
            "A2SPort=23891",
            "rhmultiplayfile=" + serverjsonpath,
        };
        arguments_str = join(arguments, " ");
    }

    const char *c_str()
    {
        return arguments_str.c_str();
    }

    size_t size()
    {
        return arguments_str.size();
    }

    void add_argument(const char* arg)
    {
        this->arguments.push_back(arg);
        this->arguments_str = join(this->arguments, " ");
    }
};

template<typename T>
struct TestArgumentsNoChallenge : public TestArguments<T>
{
    TestArgumentsNoChallenge() : TestArguments<T>()
    {
        this->arguments.push_back("A2SChallenge=0");
        this->arguments_str = join(this->arguments, " ");
    }
};

template<typename T>
struct ServerJson
{
    rallyhere::string path;
    std::ofstream ofile;

    ServerJson(const rallyhere::string& path_) : path(path_), ofile{path_.c_str()}
    {

    }

    ~ServerJson()
    {
        ofile.close();
    }

    void write(const T& data)
    {
        ofile << data;
    }
};


}
}