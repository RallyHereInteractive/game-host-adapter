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

#include <sstream>
#include <iomanip>
#include <fstream>

#if PLATFORM_LINUX
#include <unistd.h>
#include <sys/inotify.h>
#endif

#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "boost/asio.hpp"
#include "boost/url.hpp"
#include "boost/beast/core.hpp"
#include "boost/filesystem.hpp"

template<size_t N>
class FileWatcher : public std::enable_shared_from_this<FileWatcher<N>>
{
  public:
    FileWatcher(boost::asio::any_io_executor ex)
    : m_Executor{ex}
    {
#if PLATFORM_LINUX
        m_InotifyFd = inotify_init1(IN_NONBLOCK);
        if (m_InotifyFd < 0)
        {
            failure(boost::system::error_code(errno, boost::asio::error::get_system_category()), "init inotify");
            return;
        }
        auto alloc = i3d::one::StandardAllocator<boost::asio::posix::stream_descriptor>{};
        m_Stream = std::allocate_shared<boost::asio::posix::stream_descriptor>(alloc, ex, m_InotifyFd);
#endif
    }

    virtual ~FileWatcher()
    {
        cancel();
    }

    bool add_file(const rallyhere::string& path)
    {
#if PLATFORM_LINUX
        auto wd = inotify_add_watch(m_InotifyFd, path.c_str(), IN_ALL_EVENTS);
        if (wd < 0)
        {
            failure(boost::system::error_code(errno, boost::asio::error::get_system_category()), "add file");
            return false;
        }
        m_WatchMap[wd] = path;
        return true;
#else
        if (!boost::filesystem::exists(path.c_str()))
        {
            failure(boost::system::error_code(boost::system::errc::no_such_file_or_directory, boost::asio::error::get_system_category()), "add file");
            return false;
        }
        return true;
#endif
    }

    void run()
    {
#if PLATFORM_LINUX
        m_Stream->async_read_some(
            boost::asio::buffer(m_Buffer),
            boost::asio::bind_allocator(i3d::one::StandardAllocator<int>{}, boost::beast::bind_front_handler(&FileWatcher::handle_read, this->shared_from_this())));
#endif
    }

    void handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred)
    {
#if PLATFORM_LINUX
        if (ec)
        {
            return failure(ec, "read");
        }
        m_Str.append(m_Buffer.data(), m_Buffer.data() + bytes_transferred);
        while (m_Str.size() > sizeof(inotify_event))
        {
            const inotify_event *ievent = reinterpret_cast<const inotify_event*>(m_Str.data());
            handle(*ievent);
            m_Str.erase(0, sizeof(inotify_event) + ievent->len);
        }
        if (!m_Cancelled)
            run();
#endif
    }

    virtual void cancel()
    {
        m_Cancelled = true;
#if PLATFORM_LINUX
        if (m_Stream)
            m_Stream->cancel();
#endif
    }

    void cancel_later()
    {
        m_Cancelled = true;
        boost::asio::post(m_Executor, [this]() { cancel(); });
    }

    virtual void failure(boost::system::error_code ec, const char* what)
    {
        m_Error = ec;
    }

    /// @name State Functions
    /// @{
    bool good() const
    {
        return !m_Error;
    }
    bool fail() const
    {
        return m_Error.failed();
    }
    bool operator!() const
    {
        return fail();
    }
    explicit operator bool() const
    {
        return !fail();
    }
    auto error_code() const
    {
        return m_Error;
    }
    /// @}

#if PLATFORM_LINUX
    virtual void handle(const inotify_event& event) = 0;
#endif

  protected:
    boost::system::error_code m_Error{};
    boost::asio::any_io_executor m_Executor{};
#if PLATFORM_LINUX
    rallyhere::map<int, rallyhere::string> m_WatchMap{};
    int m_InotifyFd{-1};
    std::shared_ptr<boost::asio::posix::stream_descriptor> m_Stream{};
#endif
    std::array<char, N> m_Buffer{};
    rallyhere::string m_Str{};
    bool m_Cancelled{false};
};