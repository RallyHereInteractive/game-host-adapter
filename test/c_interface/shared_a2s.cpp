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
#include "shared_a2s.h"
#include "c_api.h"

#include "boost/scope_exit.hpp"

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "a2s.hpp"
#include "shared_test_data.h"
#include "shared_a2s.h"

using boost::asio::ip::udp;

void validate_server_info(lest::env& lest_env, const rallyhere::server_info& info)
{
    // Only currently support the simple response format
    EXPECT(info.ResponseHeader == -1);
    EXPECT(info.Header == 'I');
    EXPECT(info.EDF == 0);
}

struct a2s_response_handler
{
    lest::env& lest_env;
    boost::array<uint8_t, rallyhere::A2S_MAX_DATA_SIZE + rallyhere::UDP_HEADER_SIZE> recv_buf{};
    bool received{ false };
    rallyhere::a2s_simple_response response{};
    rallyhere::a2s_challenge_response challenge{};
    rallyhere::server_info info{};

    void operator()(const boost::system::error_code& ec, size_t len)
    {
        received = true;
        rallyhere::A2SDatagram datagram{ recv_buf.data(), len };
        datagram >> response;
        datagram.seek(0);
        if (response.type == rallyhere::a2s_response_type::challenge)
        {
            datagram >> challenge;
        }
        else if (response.type == rallyhere::a2s_response_type::info)
        {
            datagram >> info;
        }
        else
        {
            EXPECT(response.type == rallyhere::a2s_response_type::info);
        }
    }
};

rallyhere::server_info get_stats(lest::env& lest_env, RallyHereGameInstanceAdapterPtr adapter, TestCCodeData& data)
{
    boost::asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "localhost", "23891").begin();

    udp::socket socket(io_context);
    socket.open(udp::v4());

    std::string_view send_buf = { "\xff\xff\xff\xff\x54Source Engine Query\0", 25 };
    socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

    a2s_response_handler handler{ lest_env };
    udp::endpoint sender_endpoint;
    socket.async_receive_from(boost::asio::buffer(handler.recv_buf), sender_endpoint, std::ref(handler));

    auto start = std::chrono::steady_clock::now();
    while (!handler.received)
    {
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
        io_context.poll();
        auto ongoing = std::chrono::steady_clock::now();
        auto elapsed = ongoing - start;
        EXPECT(elapsed < std::chrono::seconds(10 + 2));
    }

    if (handler.challenge.challenge != 0)
    {
        rallyhere::string send_buf = "\xff\xff\xff\xff\x54Source Engine Query\0\xff\xff\xff\xff";
        rallyhere::A2SDatagram datagram{ send_buf };
        datagram.seek(25);
        datagram << handler.challenge.challenge;

        udp::endpoint sender_endpoint;
        socket.async_receive_from(boost::asio::buffer(handler.recv_buf), sender_endpoint, std::ref(handler));

        start = std::chrono::steady_clock::now();
        while (!handler.received)
        {
            EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
            io_context.poll();
            auto ongoing = std::chrono::steady_clock::now();
            auto elapsed = ongoing - start;
            EXPECT(elapsed < std::chrono::seconds(10 + 2));
        }
    }

    validate_server_info(lest_env, handler.info);
    return handler.info;
}
