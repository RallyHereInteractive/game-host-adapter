# RallyHereStatsBase Struct

Base stats to be exported by the game host adapter.

## Summary
| Kind | View | Description |
|------|------|-------------|
|const char *|[name](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1a81e0795669945fdad28d5e9c6ecb499b)|Name of the server.|
|const char *|[map](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1acf6058a69c59bedc5cc231ce79f4cc45)|Map the server is running.|
|const char *|[folder](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1aeff9a689014da8e2938eef48ab7a408d)|Folder containing the game files.|
|const char *|[game](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1a713b15b5a238ff1898cc9d05ad6429db)|Name of the game.|
|int32_t|[id](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1a2b9cfc8f3868f87a1cda51d816d8b820)|Numeric ID of the game. Potentially the Steam App ID.|
|uint8_t|[players](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1ae3fe7b9ec5d0977b2c41d0744d2fea30)|Number of players currently on the server.|
|uint8_t|[max_players](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1aff123439bb2fe7ac8f88973908a1b589)|Maximum number of players the server can hold.|
|uint8_t|[bots](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1a67565ba06401c066339374733c1f0f1f)|Number of bots on the server.|
|uint8_t|[server_type](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1a03e3b91a2874c60c24cb4ddb2f595def)|Type of server. Currently 'd' for dedicated, 'l' for listen, 'p' for SourceTV.|
|uint8_t|[environment](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1a869a930027d3997e7eda78bcec2e4bde)|Environment the server is running in. Currently 'w' for Windows, 'l' for Linux, 'm' for Mac.|
|uint8_t|[visibility](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1a47cbeb803c043765e14e998d39ed48cf)|Visibility of the server. Currently '0' for public and '1' for private.|
|uint8_t|[anticheat](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1a69f230c0fef002804fda3e1eb00409e3)|Anti-cheat running on the server. Currently '0' for unsecured, '1' for secured.|
|const char *|[version](structRallyHereStatsBase.xml.md#structRallyHereStatsBase_1a3e3e1b3b8cc6b01045fc31581663115d)|Version of the game installed on the server.|
## Public-attrib



### `name` {#structRallyHereStatsBase_1a81e0795669945fdad28d5e9c6ecb499b}

`const char* RallyHereStatsBase::name`

Name of the server.




### `map` {#structRallyHereStatsBase_1acf6058a69c59bedc5cc231ce79f4cc45}

`const char* RallyHereStatsBase::map`

Map the server is running.




### `folder` {#structRallyHereStatsBase_1aeff9a689014da8e2938eef48ab7a408d}

`const char* RallyHereStatsBase::folder`

Folder containing the game files.




### `game` {#structRallyHereStatsBase_1a713b15b5a238ff1898cc9d05ad6429db}

`const char* RallyHereStatsBase::game`

Name of the game.




### `id` {#structRallyHereStatsBase_1a2b9cfc8f3868f87a1cda51d816d8b820}

`int32_t RallyHereStatsBase::id`

Numeric ID of the game. Potentially the Steam App ID.




### `players` {#structRallyHereStatsBase_1ae3fe7b9ec5d0977b2c41d0744d2fea30}

`uint8_t RallyHereStatsBase::players`

Number of players currently on the server.




### `max_players` {#structRallyHereStatsBase_1aff123439bb2fe7ac8f88973908a1b589}

`uint8_t RallyHereStatsBase::max_players`

Maximum number of players the server can hold.




### `bots` {#structRallyHereStatsBase_1a67565ba06401c066339374733c1f0f1f}

`uint8_t RallyHereStatsBase::bots`

Number of bots on the server.




### `server_type` {#structRallyHereStatsBase_1a03e3b91a2874c60c24cb4ddb2f595def}

`uint8_t RallyHereStatsBase::server_type`

Type of server. Currently 'd' for dedicated, 'l' for listen, 'p' for SourceTV.




### `environment` {#structRallyHereStatsBase_1a869a930027d3997e7eda78bcec2e4bde}

`uint8_t RallyHereStatsBase::environment`

Environment the server is running in. Currently 'w' for Windows, 'l' for Linux, 'm' for Mac.




### `visibility` {#structRallyHereStatsBase_1a47cbeb803c043765e14e998d39ed48cf}

`uint8_t RallyHereStatsBase::visibility`

Visibility of the server. Currently '0' for public and '1' for private.




### `anticheat` {#structRallyHereStatsBase_1a69f230c0fef002804fda3e1eb00409e3}

`uint8_t RallyHereStatsBase::anticheat`

Anti-cheat running on the server. Currently '0' for unsecured, '1' for secured.




### `version` {#structRallyHereStatsBase_1a3e3e1b3b8cc6b01045fc31581663115d}

`const char* RallyHereStatsBase::version`

Version of the game installed on the server.





