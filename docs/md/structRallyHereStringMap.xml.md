# RallyHereStringMap Struct



Opaque type and handle to a Rally Here String Map.
This struct is intentionally empty because its true interface is only defined internally. Any methods which take this as the first argument can be thought of as methods on this struct. 
## Summary
| Kind | View | Description |
|------|------|-------------|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_string_map_create](structRallyHereStringMap.xml.md#structRallyHereStringMap_1a024ea30d47a943cb48161bf6b0a2498d)([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_string_map_destroy](structRallyHereStringMap.xml.md#structRallyHereStringMap_1a17aed95832b03004f2b54c41458e6490)([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_string_map_copy](structRallyHereStringMap.xml.md#structRallyHereStringMap_1ae7e305441ea3ae8ceb24d373b45e1e18)([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) dest, const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) src)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_string_map_clear](structRallyHereStringMap.xml.md#structRallyHereStringMap_1af1314ea38f79e701dd6980abb48253b7)([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_string_map_get](structRallyHereStringMap.xml.md#structRallyHereStringMap_1a8aa120697cdba80f092f1488c7ae62cd)([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, const char * key, const char ** value, unsigned int * value_size)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_string_map_set](structRallyHereStringMap.xml.md#structRallyHereStringMap_1a9e8929bdfc2a69879837a992acb011cf)([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, const char * key, const char * value)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_string_map_setn](structRallyHereStringMap.xml.md#structRallyHereStringMap_1adc0a5e10afb2c923e02f50dd7e58e2ee)([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, const char * key, const char * value, unsigned int value_size)||
## String map

A string map structure using the assigned allocator meant to help simplify passing generic, string key-value data to and from the adapter. For those calling this from C++ there is an example wrapper which helps to manage string map lifetimes. 

#### See also

[RallyHereStringMapWrapper](structRallyHereStringMapWrapper.xml.md#structRallyHereStringMapWrapper)

### `rallyhere_string_map_create` {#structRallyHereStringMap_1a024ea30d47a943cb48161bf6b0a2498d}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_string_map_create([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) *|map|

#### Description



Create a map of string to a string. The strings may not be null terminated in order to store binary data 



### `rallyhere_string_map_destroy` {#structRallyHereStringMap_1a17aed95832b03004f2b54c41458e6490}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_string_map_destroy([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|map|

#### Description



Destroy a map of string to a string 



### `rallyhere_string_map_copy` {#structRallyHereStringMap_1ae7e305441ea3ae8ceb24d373b45e1e18}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_string_map_copy([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) dest, const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) src)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|dest|
|const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|src|

#### Description



Copy a 
[RallyHereStringMap](structRallyHereStringMap.xml.md#structRallyHereStringMap)


### `rallyhere_string_map_clear` {#structRallyHereStringMap_1af1314ea38f79e701dd6980abb48253b7}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_string_map_clear([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|map|

#### Description



Clear the contents of a 
[RallyHereStringMap](structRallyHereStringMap.xml.md#structRallyHereStringMap)


### `rallyhere_string_map_get` {#structRallyHereStringMap_1a8aa120697cdba80f092f1488c7ae62cd}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_string_map_get([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, const char * key, const char ** value, unsigned int * value_size)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|map|
|const char *|key|
|const char **|value|
|unsigned int *|value_size|

#### Description



Get a value form 
[RallyHereStringMap](structRallyHereStringMap.xml.md#structRallyHereStringMap)


### `rallyhere_string_map_set` {#structRallyHereStringMap_1a9e8929bdfc2a69879837a992acb011cf}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_string_map_set([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, const char * key, const char * value)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|map|
|const char *|key|
|const char *|value|

#### Description



Set a value in 
[RallyHereStringMap](structRallyHereStringMap.xml.md#structRallyHereStringMap)


### `rallyhere_string_map_setn` {#structRallyHereStringMap_1adc0a5e10afb2c923e02f50dd7e58e2ee}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_string_map_setn([RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, const char * key, const char * value, unsigned int value_size)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|map|
|const char *|key|
|const char *|value|
|unsigned int|value_size|

#### Description



Set a value in 
[RallyHereStringMap](structRallyHereStringMap.xml.md#structRallyHereStringMap)with a size



