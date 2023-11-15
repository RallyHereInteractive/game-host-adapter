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

#include "lest/lest.hpp"
#include "boost/algorithm/string/find.hpp"
#include <iomanip>

template<typename ArgumentsT, typename CmdLineT>
bool ParseCommandLineArgs(ArgumentsT& args, const CmdLineT& pwzCmdLine)
{
    std::stringstream ss(pwzCmdLine.data());
    std::string buf;

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

//@formatter:off
// clang-format off
static const lest::test module[] = {
    CASE("Test overlapping command line arguments")
    {
        std::string source = "-port=2222 -sicprometheusport=1111";
        std::string prometheusport = "0000";
        std::string port = "9999";
        std::vector<std::string> m_Arguments{};
        ParseCommandLineArgs(m_Arguments, source);
        for (auto&& arg : m_Arguments)
        {
            if (ParseArgument("port=", arg, port))
            {
                continue;
            }
            if (ParseArgument("sicprometheusport=", arg, prometheusport))
            {
                continue;
            }
        }
        EXPECT(prometheusport == "1111");
        EXPECT(port == "2222");
    }
};
//@formatter:on
// clang-format off

extern lest::tests& specification();

MODULE( specification(), module );