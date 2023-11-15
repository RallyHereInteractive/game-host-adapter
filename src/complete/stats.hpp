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

#include "rh_string.h"

namespace rallyhere
{
struct StatsBase
{
    /// Name of the server
    rallyhere::string name;
    /// Map the server is running
    rallyhere::string map;
    /// Folder containing the game files
    rallyhere::string folder;
    /// Name of the game
    rallyhere::string game;
    /// Numeric ID of the game. Potentially the Steam App ID
    int32_t id;
    /// Number of players currently on the server
    uint8_t players;
    /// Maximum number of players the server can hold
    uint8_t max_players;
    /// Number of bots on the server
    uint8_t bots;
    /// Type of server. Currently 'd' for dedicated, 'l' for listen, 'p' for SourceTV
    uint8_t server_type;
    /// Environment the server is running in. Currently 'w' for Windows, 'l' for Linux, 'm' for Mac
    uint8_t environment;
    /// Visibility of the server. Currently '0' for public and '1' for private
    uint8_t visibility;
    /// Anti-cheat running on the server. Currently '0' for unsecured, '1' for secured
    uint8_t anticheat;
    /// Version of the game installed on the server
    rallyhere::string version;
};
} // namespace rallyhere