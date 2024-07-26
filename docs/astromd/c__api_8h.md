---
title: c_api.h File Reference
---An SDK for handling multiple different types of game hosting systems in one set of calls.

## Summary
| Kind | View | Description |
|------|------|-------------|
|struct|[RallyHereStatsBase](/game-host-adapter/structrallyherestatsbase/#structRallyHereStatsBase)||
|struct|[RallyHereStatsBaseProvided](/game-host-adapter/structrallyherestatsbaseprovided/#structRallyHereStatsBaseProvided)||
|enum|[RallyHereLogLevel](/game-host-adapter/c__api_8h/#c__api_8h_1acea7ed85e1829a9b981399a844ae5120)||
|typedef|[RallyHereLogLevel](/game-host-adapter/c__api_8h/#c__api_8h_1ade4228dcd8f4d1f8c1d42bd5f40bcca3)||
|typedef|[RallyHereLogCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a64a7679e581842654beca7ae63664a26)|Log callback function.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_set_default_log_level](/game-host-adapter/c__api_8h/#c__api_8h_1a4b0adaca38324c8ac2ebf6d2429b3d7a)([RallyHereLogLevel](/game-host-adapter/c__api_8h/#c__api_8h_1acea7ed85e1829a9b981399a844ae5120) level)|Sets the default log level for the SDK and any adapters created. This is only used if a log callback is not set.|
|typedef|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|Opaque type and handle to a Rally Here Game Instance Adapter.|
|typedef|[RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|Opaque type and handle to a Rally Here String Map.|
|typedef|[RallyHereOnAllocatedCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a8664a168347b6152d05be2870fff5113)|Callback to be sent when an allocation is received from the game host adapter.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_allocator_set_alloc](/game-host-adapter/c__api_8h/#c__api_8h_1a4432d2d7215904ab70ae8fa09ceee234)(void *(*)(unsigned int size) callback)|Optional custom memory alloc override.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_allocator_set_free](/game-host-adapter/c__api_8h/#c__api_8h_1aff419acc09d05afbc49c6b671d9b053f)(void(*)(void *) callback)|Optional custom memory free override.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_allocator_set_realloc](/game-host-adapter/c__api_8h/#c__api_8h_1a7eeeadbe1fc805105e3ab945cb246720)(void *(*)(void *, unsigned int size) callback)|Optional custom memory realloc override.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_global_init](/game-host-adapter/c__api_8h/#c__api_8h_1a70be86817854bcd19b3f10b31fdf19c8)()|Initialize the Rally Here SDK.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_global_cleanup](/game-host-adapter/c__api_8h/#c__api_8h_1af11502e19d260c2a68285b7eb9e635c1)()|Cleanup the Rally Here SDK.|
## Logging

Logging levels that will passed to the optional log callback function 



### `RallyHereLogLevel` <a id="c__api_8h_1acea7ed85e1829a9b981399a844ae5120"></a>




| Enumerator | Initializer|
|------------|------------|
|`RH_LOG_LEVEL_ERROR`|``|
|`RH_LOG_LEVEL_INFO`|``|
|`RH_LOG_LEVEL_DEBUG`|``|
|`RH_LOG_LEVEL_TRACE`|``|


### `RallyHereLogLevel` <a id="c__api_8h_1ade4228dcd8f4d1f8c1d42bd5f40bcca3"></a>

typedef enum [RallyHereLogLevel](/game-host-adapter/c__api_8h/#c__api_8h_1acea7ed85e1829a9b981399a844ae5120) RallyHereLogLevel




### `RallyHereLogCallback` <a id="c__api_8h_1a64a7679e581842654beca7ae63664a26"></a>

typedef void(* RallyHereLogCallback)(RallyHereLogLevel level, const char *message, size_t messagen, void *user_data)Log callback function.




### `rallyhere_set_default_log_level` <a id="c__api_8h_1a4b0adaca38324c8ac2ebf6d2429b3d7a"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_set_default_log_level([RallyHereLogLevel](/game-host-adapter/c__api_8h/#c__api_8h_1acea7ed85e1829a9b981399a844ae5120) level)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereLogLevel](/game-host-adapter/c__api_8h/#c__api_8h_1acea7ed85e1829a9b981399a844ae5120)|level|

#### Description

Sets the default log level for the SDK and any adapters created. This is only used if a log callback is not set.





## Opaque types

The API uses the pointer handles to represent internal objects. 


### `RallyHereGameInstanceAdapterPtr` <a id="c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b"></a>

typedef [RallyHereGameInstanceAdapter](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter) * RallyHereGameInstanceAdapterPtrOpaque type and handle to a Rally Here Game Instance Adapter.




### `RallyHereStringMapPtr` <a id="c__api_8h_1a8901377cd48d4831c6380593a2a60360"></a>

typedef [RallyHereStringMap](/game-host-adapter/structrallyherestringmap/#structRallyHereStringMap) * RallyHereStringMapPtrOpaque type and handle to a Rally Here String Map.





## State request and callbacks



### `RallyHereOnAllocatedCallback` <a id="c__api_8h_1a8664a168347b6152d05be2870fff5113"></a>

typedef void(* RallyHereOnAllocatedCallback)(RallyHereStringMapPtr allocation_info, const RallyHereStatusCode &code, void *user_data)Callback to be sent when an allocation is received from the game host adapter.

The callback is responsible for freeing the allocation_info string map 




## Environment



### `rallyhere_allocator_set_alloc` <a id="c__api_8h_1a4432d2d7215904ab70ae8fa09ceee234"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_allocator_set_alloc(void *(*)(unsigned int size) callback)

#### Parameters

| Type | Name |
|------|------|
|void *(*)(unsigned int size)|callback|

#### Description

Optional custom memory alloc override.

If set, must be set at init time, before using any other APIs. If this is called, then rallyhere_allocator_set_free must also be called. If this is called, then rallyhere_allocator_set_free and rallyhere_allocator_set_realloc must also be called. 
#### Parameters

callback
: A function that takes a size in bytes of memory to allocate. 

#### See also

[rallyhere_allocator_set_free](/game-host-adapter/c__api_8h/#c__api_8h_1aff419acc09d05afbc49c6b671d9b053f)
#### See also

[rallyhere_allocator_set_realloc](/game-host-adapter/c__api_8h/#c__api_8h_1a7eeeadbe1fc805105e3ab945cb246720)


### `rallyhere_allocator_set_free` <a id="c__api_8h_1aff419acc09d05afbc49c6b671d9b053f"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_allocator_set_free(void(*)(void *) callback)

#### Parameters

| Type | Name |
|------|------|
|void(*)(void *)|callback|

#### Description

Optional custom memory free override.

If set, must be set at init time, before using any other APIs. 
#### Parameters

callback
: An existing pointer p to free. 



### `rallyhere_allocator_set_realloc` <a id="c__api_8h_1a7eeeadbe1fc805105e3ab945cb246720"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_allocator_set_realloc(void *(*)(void *, unsigned int size) callback)

#### Parameters

| Type | Name |
|------|------|
|void *(*)(void *, unsigned int size)|callback|

#### Description

Optional custom memory realloc override.

If set, must be set at init time, before using any other APIs. 
#### Parameters

callback
: A function taking an existing pointer and a new size. See the standard c realloc requirements for behavior. 




## Global

Bookend calls for using this SDK. Only the Environment methods may be called before or after these SDK calls. 


### `rallyhere_global_init` <a id="c__api_8h_1a70be86817854bcd19b3f10b31fdf19c8"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_global_init()

#### Parameters

| Type | Name |
|------|------|

#### Description

Initialize the Rally Here SDK.

Custom memory allocation must be made before this call, and no other SDK calls may be made until after this call. 



### `rallyhere_global_cleanup` <a id="c__api_8h_1af11502e19d260c2a68285b7eb9e635c1"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_global_cleanup()

#### Parameters

| Type | Name |
|------|------|

#### Description

Cleanup the Rally Here SDK.

No other SDK calls may be made after this call. 




