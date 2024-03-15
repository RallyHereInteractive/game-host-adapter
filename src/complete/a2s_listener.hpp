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

#include "a2s.hpp"

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
#include "stats.hpp"
#include <random>
#include "boost/circular_buffer.hpp"


namespace rallyhere
{

template<class T>
using circular_buffer = boost::circular_buffer<T, i3d::one::StandardAllocator<T>>;

class a2s_listener : public std::enable_shared_from_this<a2s_listener>
{
    using udp = boost::asio::ip::udp; // from <boost/asio/ip/udp.hpp>
  public:
    a2s_listener(boost::asio::any_io_executor ex, short port, logger logger, bool challenge ) :
        m_Socket(ex, udp::endpoint(udp::v4(), port)), m_Logger{ logger }, m_ShouldChallenge{ challenge }
    {
    }

    void run()
    {
        do_receive();
    }

    void do_receive()
    {
        m_Socket.async_receive_from(
            boost::asio::buffer(m_Buffer),
            m_SenderEndpoint,
            boost::asio::bind_allocator(i3d::one::StandardAllocator<int>{},
                                        boost::beast::bind_front_handler(&a2s_listener::handle_receive_from, this->shared_from_this())));
    }

    void do_send(std::size_t length)
    {
        m_Socket.async_send_to(boost::asio::buffer(m_Buffer, length),
                               m_SenderEndpoint,
                               boost::asio::bind_allocator(i3d::one::StandardAllocator<int>{},
                                                           boost::beast::bind_front_handler(&a2s_listener::handle_send, this->shared_from_this())));
    }

    void process(a2s_simple_response* resp, size_t bytes_recvd)
    {
        resp->header = boost::endian::little_to_native(resp->header);
        if (resp->header != -1)
        {
            warn(boost::system::errc::invalid_argument, "invalid header", resp->header);
            return;
        }
        switch (static_cast<a2s_query>(resp->type))
        {
            case a2s_query::info:
                if (!handle_info(bytes_recvd))
                    return;
                break;
            default:
                warn(boost::system::errc::operation_not_supported, "unhandled request", resp->type);
                return;
                break;
        }
    }

    void handle_receive_from(const boost::system::error_code& ec, size_t bytes_recvd)
    {
        if (ec)
        {
            fatal(ec, "handle receive from");
            return;
        }
        if (ec)
        {
            fatal(ec, "read");
            return;
        }
        if (bytes_recvd >= sizeof(a2s_simple_response))
        {
            a2s_simple_response* resp = reinterpret_cast<a2s_simple_response*>(m_Buffer.data());
            process(resp, bytes_recvd);
        }
        else
        {
            warn(boost::system::errc::operation_not_supported, "unknown datagram size");
        }
        if (!m_Cancelled)
            do_receive();
    }

    bool handle_info(size_t len)
    {
        const auto A2S_CHALLENGE_FIELD_SIZE = 4;
        static_assert (A2S_CHALLENGE_FIELD_SIZE == sizeof(int32_t), "A2S_CHALLENGE_FIELD_SIZE must be 4");
        if (len == sizeof(a2s_simple_response))
            return warn(boost::system::errc::invalid_argument, "payload too small");
        auto begin = m_Buffer.begin() + sizeof(a2s_simple_response);
        auto it = begin;
        while (it != m_Buffer.end() && *it != '\0')
            ++it;
        ++it;
        size_t total_len = it - m_Buffer.begin();
        if (m_ShouldChallenge && total_len == len)
        {
            // Send back this request with a challenge
            A2SDatagram datagram(m_Buffer);
            m_ServerInfo.ResponseHeader = -1;
            m_ServerInfo.Header = 0x41;
            auto current_challenge = m_ChallengeDistribution(m_RandomEngine);
            datagram << m_ServerInfo.ResponseHeader << m_ServerInfo.Header << current_challenge;
            m_RecentChallenges.push_back(current_challenge);
            do_send(datagram.size());
            return true;
        }
        if (m_ShouldChallenge && total_len != len)
        {
            // Check that the challenge is valid
            A2SDatagram datagram(&(*it), len - total_len);
            if ((len - total_len) != A2S_CHALLENGE_FIELD_SIZE)
                return warn(boost::system::errc::invalid_argument, "invalid challenge size");
            int32_t current_challenge;
            datagram >> current_challenge;
            auto existing_challenge = std::find(m_RecentChallenges.begin(), m_RecentChallenges.end(), current_challenge);
            if (existing_challenge == m_RecentChallenges.end())
                return warn(boost::system::errc::invalid_argument, "invalid challenge", current_challenge);
            m_RecentChallenges.erase(existing_challenge);
        }
        if (!m_ShouldChallenge && total_len != len)
            return warn(boost::system::errc::invalid_argument, "challenges not supported");
        // Send back A2S_INFO response
        A2SDatagram datagram(m_Buffer);
        m_ServerInfo.ResponseHeader = -1;
        m_ServerInfo.Header = 'I';
        m_ServerInfo.EDF = 0;
        datagram << m_ServerInfo.ResponseHeader << m_ServerInfo.Header << m_ServerInfo.Protocol << m_ServerInfo.Name << m_ServerInfo.Map
                 << m_ServerInfo.Folder << m_ServerInfo.GameName << m_ServerInfo.ID << m_ServerInfo.Players << m_ServerInfo.MaxPlayers
                 << m_ServerInfo.Bots << m_ServerInfo.ServerType << m_ServerInfo.Environment << m_ServerInfo.Visibility << m_ServerInfo.VAC
                 << m_ServerInfo.Version << m_ServerInfo.EDF;
        if (datagram.overflowed)
            return warn(boost::system::errc::invalid_argument, "data too large for single datagram");
        do_send(datagram.size());
        return true;
    }

    void handle_send(const boost::system::error_code& ec, std::size_t bytes_sent)
    {
        if (ec)
        {
            fatal(ec, "send");
            return;
        }
        do_receive();
    }

    void cancel()
    {
        m_Cancelled = true;
        m_Socket.cancel();
    }

    void cancel_later()
    {
        m_Cancelled = true;
        boost::asio::post(m_Socket.get_executor(), [this]() {
            cancel();
        });
    }

    bool warn(boost::system::errc::errc_t errc, const char* what)
    {
        return warn({ errc, boost::system::generic_category() }, what);
    }

    bool warn(boost::system::error_code ec, const char* what)
    {
        m_Logger.log(RH_LOG_LEVEL_ERROR, "a2s: {}: {}", what, ec.message());
        m_Error = ec;
        return false;
    }

    template<typename T>
    bool warn(boost::system::errc::errc_t errc, const char* what, T&& value)
    {
        return warn({ errc, boost::system::generic_category() }, what, value);
    }

    template<typename T>
    bool warn(boost::system::error_code ec, const char* what, T&& value)
    {
        m_Logger.log(RH_LOG_LEVEL_ERROR, "a2s: {}: {} {}", what, ec.message(), value);
        m_Error = ec;
        return false;
    }

    bool fatal(boost::system::errc::errc_t errc, const char* what)
    {
        return fatal({ errc, boost::system::generic_category() }, what);
    }

    bool fatal(boost::system::error_code ec, const char* what)
    {
        m_Logger.log(RH_LOG_LEVEL_ERROR, "a2s: {}: {}", what, ec.message());
        m_Error = ec;
        return false;
    }

    void update_server_info(server_info info)
    {
        auto self = this->shared_from_this();
        boost::asio::post(m_Socket.get_executor(), [self, info = std::move(info)]() {
            self->handle_update_server_info(std::move(info));
        });
    }

    void update_server_info(rallyhere::StatsBase stats, RallyHereStatsBaseProvided provided, std::function<void()> callback)
    {
        auto self = this->shared_from_this();
        boost::asio::post(m_Socket.get_executor(), [self, stats = std::move(stats), provided = std::move(provided), callback = std::move(callback)]() {
            self->handle_update_server_info(std::move(stats), std::move(provided));
            callback();
        });
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

  protected:
    void handle_update_server_info(server_info info)
    {
        m_ServerInfo = info;
    }

    void handle_update_server_info(rallyhere::StatsBase stats, RallyHereStatsBaseProvided provided)
    {
        if (provided.set_name)
            m_ServerInfo.Name = stats.name;
        if (provided.set_map)
            m_ServerInfo.Map = stats.map;
        if (provided.set_folder)
            m_ServerInfo.Folder = stats.folder;
        if (provided.set_game)
            m_ServerInfo.GameName = stats.game;
        if (provided.set_id)
        {
            if (stats.id > std::numeric_limits<decltype(m_ServerInfo.ID)>::max())
            {
                // RAS FIXME Use the EDF to set the GameID
            }
            m_ServerInfo.ID = stats.id;
        }
        if (provided.set_players)
            m_ServerInfo.Players = stats.players;
        if (provided.set_max_players)
            m_ServerInfo.MaxPlayers = stats.max_players;
        if (provided.set_bots)
            m_ServerInfo.Bots = stats.bots;
        if (provided.set_server_type)
            m_ServerInfo.ServerType = stats.server_type;
        if (provided.set_environment)
            m_ServerInfo.Environment = stats.environment;
        if (provided.set_visibility)
            m_ServerInfo.Visibility = stats.visibility;
        if (provided.set_anticheat)
            m_ServerInfo.VAC = stats.anticheat;
        if (provided.set_version)
            m_ServerInfo.Version = stats.version;
    }

  private:
    udp::socket m_Socket;
    udp::endpoint m_SenderEndpoint;
    std::array<uint8_t, A2S_MAX_DATA_SIZE + UDP_HEADER_SIZE> m_Buffer{};
    bool m_Cancelled{ false };
    boost::system::error_code m_Error{};
    server_info m_ServerInfo;
    circular_buffer<int32_t> m_RecentChallenges{20};
    logger m_Logger{};
    std::random_device m_RandomDevice{};
    std::default_random_engine m_RandomEngine{ m_RandomDevice() };
    std::uniform_int_distribution<int32_t> m_ChallengeDistribution{1};
    rallyhere::vector<int32_t> m_Challenges{};
    bool m_ShouldChallenge{true};
};

} // namespace rallyhere