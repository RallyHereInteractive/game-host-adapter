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
#include "sdk.h"

namespace rallyhere
{
#ifndef DEFAULT_LOG_LEVEL
#define DEFAULT_LOG_LEVEL RH_LOG_LEVEL_ERROR
#endif

RallyHereLogLevel logger::s_LogLevel = DEFAULT_LOG_LEVEL;

void logger::log(RallyHereLogLevel level, std::string_view v)
{
    if (m_pAdapter && m_pAdapter->m_LogCallback)
    {
        m_pAdapter->m_LogCallback(level, v);
    }
    else
    {
        if (level > s_LogLevel)
            return;
        if (level == RH_LOG_LEVEL_ERROR)
        {
            std::cerr << v << std::endl;
        }
        else
        {
            std::cout << v << std::endl;
        }
    }
}

}