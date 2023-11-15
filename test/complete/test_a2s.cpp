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
#include "test_a2s.h"

#include <cstring>
#include "boost/core/span.hpp"
#include "rh_string.h"
#include "a2s.hpp"

using namespace rallyhere;

#define SAFE_MEMCPY(DEST, DESTSZ, SRC, SRCSZ, OUTPTR)                                                                                                         \
        if (DESTSZ >= SRCSZ)                                                                                                                                  \
        {                                                                                                                                                     \
            OUTPTR = static_cast<uint8_t*>(memcpy(DEST, SRC, SRCSZ)) + SRCSZ;                                                                                   \
        }                                                                                                                                                     \
        else                                                                                                                                                  \
        {                                                                                                                                                     \
            return ptr - data;                               \
        }

static constexpr unsigned int A2S_MAX_PACKET_SIZE = 1400;

struct FServerInfo
{
#pragma pack(push, 1)
    struct
    {
        uint32_t ResponseHeader;
        uint8_t Header;
        uint8_t Protocol;
    } binFirst;
#pragma pack(pop)

    std::string Name;
    std::string Map;
    std::string Folder;
    std::string GameName;

#pragma pack(push, 1)
    struct
    {
        uint16_t ID; //This probably would need to be placed in a specific byte order, but we don't put anything meaningful into it anyhow
        uint8_t Players;
        uint8_t MaxPlayers;
        uint8_t Bots;
        uint8_t ServerType;
        uint8_t Environment;
        uint8_t Visibility;
        uint8_t VAC;
    } binSecond;
#pragma pack(pop)

    std::string Version;

    uint8_t EDF;
};

template<size_t N>
size_t populate_old_style(lest::env & lest_env, const FServerInfo& info, uint8_t (&data)[N])
{
	uint8_t* ptr = data;
	//Serialize to network bytes
    uint8_t* end = std::end(data);
    SAFE_MEMCPY(ptr, static_cast<std::size_t>(end - ptr), &info.binFirst, sizeof(info.binFirst), ptr);

    SAFE_MEMCPY(ptr, static_cast<std::size_t>(end - ptr), info.Name.c_str(), info.Name.size() + 1, ptr);
    SAFE_MEMCPY(ptr, static_cast<std::size_t>(end - ptr), info.Map.c_str(), info.Map.size() + 1, ptr);
    SAFE_MEMCPY(ptr, static_cast<std::size_t>(end - ptr), info.Folder.c_str(), info.Folder.size() + 1, ptr);
    SAFE_MEMCPY(ptr, static_cast<std::size_t>(end - ptr), info.GameName.c_str(), info.GameName.size() + 1, ptr);

    SAFE_MEMCPY(ptr, static_cast<std::size_t>(end - ptr), &info.binSecond, sizeof(info.binSecond), ptr);

    SAFE_MEMCPY(ptr, static_cast<std::size_t>(end - ptr), info.Version.c_str(), info.Version.size() + 1, ptr);

    SAFE_MEMCPY(ptr, static_cast<std::size_t>(end - ptr), &info.EDF, sizeof(info.EDF), ptr);
    return ptr - data;
}


template<size_t SIZE>
struct A2SPacket
{
	void put(const uint8_t *data, size_t len)
	{
		if (overflowed)
			return;
		if (next + len > std::end(Data))
		{
			overflowed = true;
			return;
		}
		next = static_cast<uint8_t*>(memcpy(next, data, len)) + len;
	}
	template<typename T>
	A2SPacket& operator<<(const T& data)
	{
		put(reinterpret_cast<const uint8_t*>(&data), sizeof(data));
		return *this;
	}
	A2SPacket& operator<<(const std::string& data)
	{
		put(reinterpret_cast<const uint8_t*>(data.c_str()), data.size() + 1);
		return *this;
	}
	uint8_t Data[SIZE];
	uint8_t *next{Data};
	bool overflowed{false};
};

template<size_t N>
size_t populate_new_style(lest::env & lest_env, const FServerInfo& info, A2SPacket<N>& packet)
{
    packet << info.binFirst << info.Name << info.Map << info.Folder << info.GameName
           << info.binSecond << info.Version << info.EDF;
    return packet.next - packet.Data;
}

size_t populate_new_style(lest::env & lest_env, const server_info& info, A2SDatagram& packet)
{
    packet
        << info.ResponseHeader
        << info.Header
        << info.Protocol
        << info.Name
        << info.Map
        << info.Folder
        << info.GameName
        << info.ID
        << info.Players
        << info.MaxPlayers
        << info.Bots
        << info.ServerType
        << info.Environment
        << info.Visibility
        << info.VAC
        << info.Version
        << info.EDF;
    return packet.size();
}

void compare_new_and_old(lest::env & lest_env, const FServerInfo& info, bool skip_extended = false)
{
    uint8_t old_data[A2S_MAX_PACKET_SIZE];
    memset(old_data, 0, sizeof(old_data));
    auto old_written = populate_old_style(lest_env, info, old_data);

    A2SPacket<A2S_MAX_PACKET_SIZE> new_data;
    memset(new_data.Data, 0, sizeof(new_data.Data));
    auto new_written = populate_new_style(lest_env, info, new_data);
    EXPECT(old_written == new_written);
    for (auto oldbegin = old_data, newbegin = new_data.Data; oldbegin != old_data + old_written; ++oldbegin, ++newbegin)
    {
        EXPECT(*oldbegin == *newbegin);
    }

    // Note that this test will only be correct on le little endian machines
    uint8_t datagram_data[A2S_MAX_PACKET_SIZE];
    memset(datagram_data, 0, sizeof(datagram_data));
    A2SDatagram datagram(datagram_data);
    server_info new_info_style{};
    new_info_style.ResponseHeader = info.binFirst.ResponseHeader;
    new_info_style.Header = info.binFirst.Header;
    new_info_style.Protocol = info.binFirst.Protocol;
    new_info_style.Name = info.Name;
    new_info_style.Map = info.Map;
    new_info_style.Folder = info.Folder;
    new_info_style.GameName = info.GameName;
    new_info_style.ID = info.binSecond.ID;
    new_info_style.Players = info.binSecond.Players;
    new_info_style.MaxPlayers = info.binSecond.MaxPlayers;
    new_info_style.Bots = info.binSecond.Bots;
    new_info_style.ServerType = info.binSecond.ServerType;
    new_info_style.Environment = info.binSecond.Environment;
    new_info_style.Visibility = info.binSecond.Visibility;
    new_info_style.VAC = info.binSecond.VAC;
    new_info_style.Version = info.Version;
    new_info_style.EDF = info.EDF;
    new_written = populate_new_style(lest_env, new_info_style, datagram);
    EXPECT(old_written == new_written);
    for (auto oldbegin = old_data, newbegin = new_data.Data; oldbegin != old_data + old_written; ++oldbegin, ++newbegin)
    {
        EXPECT(*oldbegin == *newbegin);
    }

    if (!skip_extended)
    {
        memset(datagram_data, 0, sizeof(datagram_data));
        A2SDatagram testagram{ datagram_data };
        testagram << new_info_style;

        server_info output{};
        A2SDatagram outagram{ datagram_data };
        outagram >> output;
        EXPECT(new_info_style == output);
    }
}


//@formatter:off
// clang-format off
static const lest::test module[] = {
    CASE("Default case")
    {
        FServerInfo info{};
        info.binFirst.ResponseHeader = -1;
        info.binFirst.Header = 0x49;
        info.binFirst.Protocol = 0x00;
        info.binSecond.ID = 0;
        info.binSecond.Players = 10;
        info.binSecond.MaxPlayers = 32;
        info.binSecond.Bots = 5;
        info.binSecond.ServerType = 'd';
        info.binSecond.Environment = 'l';
        info.binSecond.Visibility = 0;
        info.binSecond.VAC = 0;
        info.EDF = 0x00;

        compare_new_and_old(lest_env, info);
    },
    CASE("Longer case")
    {
        FServerInfo info{};
        info.binFirst.ResponseHeader = -1;
        info.binFirst.Header = 0x49;
        info.binFirst.Protocol = 0x00;
        info.binSecond.ID = 0;
        info.binSecond.Players = 10;
        info.binSecond.MaxPlayers = 32;
        info.binSecond.Bots = 5;
        info.binSecond.ServerType = 'd';
        info.binSecond.Environment = 'l';
        info.binSecond.Visibility = 0;
        info.binSecond.VAC = 0;
        info.EDF = 0x00;

        info.Name = "Rocking in a Free World 2";
        info.Folder = "/home/dev/cia/rocking2";
        info.GameName = "The Only Way To Play";
        info.Map = "de_dust2";

        compare_new_and_old(lest_env, info);
    },
    CASE("Too long case")
    {
        FServerInfo info{};
        info.binFirst.ResponseHeader = -1;
        info.binFirst.Header = 0x49;
        info.binFirst.Protocol = 0x00;
        info.binSecond.ID = 0;
        info.binSecond.Players = 10;
        info.binSecond.MaxPlayers = 32;
        info.binSecond.Bots = 5;
        info.binSecond.ServerType = 'd';
        info.binSecond.Environment = 'l';
        info.binSecond.Visibility = 0;
        info.binSecond.VAC = 0;
        info.EDF = 0x00;

        info.Name = "Rocking in a Free World 2";
        info.Folder = "/home/dev/cia/rocking2";
        info.GameName = "The Only Way To Play";
        info.Map = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam imperdiet purus nec elit porttitor, nec varius ligula pharetra. Mauris vel nisi malesuada, porta ante non, commodo orci. Etiam nec lacus sit amet risus aliquam dignissim. Vivamus egestas quis tellus in dictum. Maecenas id tincidunt diam, a porta neque. Ut convallis enim ut augue porttitor, eget mattis leo dictum. Nam vehicula, felis quis hendrerit hendrerit, lacus arcu tincidunt tellus, sagittis lobortis nisl orci eget purus. Vivamus pellentesque luctus volutpat. Vestibulum elementum lectus vel neque pharetra ullamcorper dapibus et libero.\n"
                   "\n"
                   "Nam accumsan facilisis mauris vel scelerisque. Vestibulum lacus libero, varius vitae ligula vitae, rutrum varius tortor. Donec nec tempus purus. Ut vitae ipsum sit amet massa commodo dignissim vulputate id justo. In dignissim urna id consectetur laoreet. Donec interdum sem et nulla volutpat blandit. Pellentesque varius ullamcorper lorem, ut convallis odio bibendum non. In eu magna a massa luctus dignissim in a dolor.\n"
                   "\n"
                   "Aenean est ligula, ultrices non urna sed, viverra rhoncus dui. Integer consectetur commodo arcu, sit amet tempor orci volutpat eu. Cras pretium iaculis hendrerit. Nulla sagittis elit sem, vel mattis tellus efficitur a. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Aliquam dapibus metus id sem dapibus, sit amet pulvinar nullam.de_dust2";

        compare_new_and_old(lest_env, info, true);
    }
};
//@formatter:on
// clang-format off

extern lest::tests& specification();

MODULE( specification(), module );