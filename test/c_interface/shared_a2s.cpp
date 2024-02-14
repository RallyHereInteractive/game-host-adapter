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

rallyhere::server_info get_stats(lest::env& lest_env, RallyHereGameInstanceAdapterPtr adapter, TestCCodeData& data)
{
    boost::asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "localhost", "23891").begin();

    udp::socket socket(io_context);
    socket.open(udp::v4());

    std::string_view send_buf = { "\xff\xff\xff\xff\x54Source Engine Query\0", 25 };
    socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

    boost::array<uint8_t, rallyhere::A2S_MAX_DATA_SIZE + rallyhere::UDP_HEADER_SIZE> recv_buf;
    udp::endpoint sender_endpoint;
    bool received{ false };
    rallyhere::server_info info{};
    socket.async_receive_from(
        boost::asio::buffer(recv_buf), sender_endpoint, [&received, &recv_buf, &info](const boost::system::error_code& ec, size_t len) {
            received = true;
            rallyhere::A2SDatagram datagram{ recv_buf.data(), len };
            datagram >> info;
        });

    auto start = std::chrono::steady_clock::now();
    while (!received)
    {
        EXPECT(rallyhere_tick(adapter) == RH_STATUS_OK);
        io_context.poll();
        auto ongoing = std::chrono::steady_clock::now();
        auto elapsed = ongoing - start;
        EXPECT(elapsed < std::chrono::seconds(10 + 2));
    }

    validate_server_info(lest_env, info);
    return info;
}
