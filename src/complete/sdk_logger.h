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

#include "c_platform.h"
#include "c_api.h"
#include "status.h"
#include "rh_string.h"
#include "rh_vector.h"
#include "rh_map.h"
#include "rh_format.h"
#include "custom_resource.h"
#include <iostream>

namespace rallyhere
{

class GameInstanceAdapter;

class logger
{
  public:
    logger() = default;
    logger(const logger&) = default;
    logger& operator=(const logger&) = default;
    logger(GameInstanceAdapter& adapter) : m_pAdapter(&adapter)
    {
    }

    void log(RallyHereLogLevel level, std::string_view v);

    template<typename... T>
    void log(RallyHereLogLevel level, fmt::format_string<T...> fmt, T&&... args)
    {
        rallyhere::memory_buffer buffer;
        fmt::format_to(std::back_inserter(buffer), fmt, std::forward<T>(args)...);
        std::string_view v{ buffer.data(), buffer.size() };
        log(level, v);
    }

    static void SetDefaultLogLevel(RallyHereLogLevel level)
    {
        s_LogLevel = level;
    }

  private:
    GameInstanceAdapter* m_pAdapter{};
    static RallyHereLogLevel s_LogLevel;
};

}