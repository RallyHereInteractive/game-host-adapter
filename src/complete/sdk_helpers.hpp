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
#include "boost/algorithm/string/find.hpp"

template<typename ArgumentsT, typename CmdLineT>
bool ParseCommandLineArgs(ArgumentsT& args, const CmdLineT& pwzCmdLine)
{
    std::basic_stringstream<char, std::char_traits<char>, i3d::one::StandardAllocator<char>> ss(pwzCmdLine.data());
    rallyhere::string buf;

    while (ss >> std::quoted(buf))
    {
        args.emplace_back(buf);
    }

    return true;
}

template<typename CharT, typename StringT>
bool ParseArgument(const CharT* option, const StringT& in_arg, StringT& value)
{
    auto n = std::char_traits<CharT>::length(option);
    auto arg_begin = in_arg.begin();
    while (*arg_begin == '-' || *arg_begin == ' ' || *arg_begin == '/')
        ++arg_begin;
    std::string_view arg{ arg_begin, in_arg.end() };
    auto needle = boost::ifind_first(arg, option);
    if (needle.begin() != arg.begin())
        return false;
    auto value_pos = std::distance(arg.begin(), needle.end());
    if (value_pos > arg.size())
        return false;
    value = arg.substr(value_pos);
    return true;
}

inline rallyhere::vector<rallyhere::string> LoadFileToStringArray(const rallyhere::string& filename)
{
    rallyhere::vector<rallyhere::string> result;
    std::ifstream file(filename.data());
    if (file.is_open())
    {
        rallyhere::string line;
        while (std::getline(file, line))
        {
            if (!line.empty())
                if (line.back() == '\r')
                    line.pop_back();
            result.push_back(line);
        }
        file.close();
    }
    return result;
}

// A function that loads a file into a rallyhere::string
inline rallyhere::string LoadFileToString(const rallyhere::string& path)
{
    std::ifstream file(path.c_str());
    rallyhere::stringstream sstr;
    sstr << file.rdbuf();
    return sstr.str();
}