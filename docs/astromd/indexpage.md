---
title: Rally Here Game Host Adapter
---

An SDK for handling multiple different types of game hosting systems in one set of calls
# Introduction <a id="index_1intro_sec"></a>

This library as meant to be used as part of integrating game instance servers with RallyHere. The source code for the entire system is provided, but one should only target and use the C API directly. This will enable RallyHere to update your game servers to be able to support different game hosting providers without the need for the code in the game server to change.
# Getting Started <a id="index_1getting_started"></a>

The main usage of this library is to set up your memory allocator, initialize the sdk, instantiate a game instance adapter, and then tick that either asserting or waiting for allocations to come in with additional details.


#### See also

[rallyhere_global_init()](/game-host-adapter/c__api_8h/#c__api_8h_1a70be86817854bcd19b3f10b31fdf19c8)
#### See also

[rallyhere_allocator_set_alloc()](/game-host-adapter/c__api_8h/#c__api_8h_1a4432d2d7215904ab70ae8fa09ceee234)
#### See also

[RallyHereGameInstanceAdapter](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter)