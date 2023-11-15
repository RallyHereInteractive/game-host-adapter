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
#include "file_watcher.hpp"
#include "sdk_logger.h"

namespace rallyhere
{
class ServerJsonWatcher : public FileWatcher<1024>
{
  public:
    ServerJsonWatcher(boost::asio::any_io_executor ex, const rallyhere::string& path, std::function<void()> fileChangedCallback, logger logger) :
        FileWatcher<1024>(ex), m_Timer(ex), m_Logger{ logger }, m_FileChangedCallback{ fileChangedCallback }
    {
        add_file(path);
        if (good())
            start_fallback_timer();
    }

    void start_fallback_timer()
    {
        m_Timer.expires_from_now(boost::posix_time::seconds(1));
        m_Timer.async_wait([this](const boost::system::error_code& ec) {
            if (ec)
                return;
            if (good())
            {
                m_FileChangedCallback();
                start_fallback_timer();
            }
        });
    }

    void failure(boost::system::error_code ec, const char* what) override
    {
        m_Logger.log(RH_LOG_LEVEL_ERROR, "{}: {}", what, ec.message());
        m_Error = ec;
    }

#if PLATFORM_LINUX
    void debug_handle(const inotify_event& event)
    {
        rallyhere::memory_buffer buffer;
        fmt::format_to(std::back_inserter(buffer), "{}: ", event.mask);
        if (event.mask & IN_OPEN)
            fmt::format_to(std::back_inserter(buffer), "IN_OPEN: ");
        if (event.mask & IN_CLOSE_NOWRITE)
            fmt::format_to(std::back_inserter(buffer), "IN_CLOSE_NOWRITE: ");
        if (event.mask & IN_CLOSE_WRITE)
            fmt::format_to(std::back_inserter(buffer), "IN_CLOSE_WRITE: ");

        for (auto&& p : m_WatchMap)
        {
            if (p.first == event.wd)
            {
                fmt::format_to(std::back_inserter(buffer), "{}/", p.second);
                break;
            }
        }

        if (event.len)
            fmt::format_to(std::back_inserter(buffer), "{}", event.name);

        if (event.mask & IN_ISDIR)
            fmt::format_to(std::back_inserter(buffer), " [directory]");
        else
            fmt::format_to(std::back_inserter(buffer), " [file]");
        m_Logger.log(RH_LOG_LEVEL_INFO, { buffer.data(), buffer.size() });
    }

    void handle(const inotify_event& event) override
    {
        if (event.mask & IN_CLOSE_WRITE)
            m_FileChangedCallback();
    }
#endif

    void cancel() override
    {
        m_Timer.cancel();
        FileWatcher<1024>::cancel();
    }

  protected:
    boost::asio::deadline_timer m_Timer;
    logger m_Logger{};
    std::function<void()> m_FileChangedCallback{};
};

}