# rallyhere namespace



## Summary
| Kind | View | Description |
|------|------|-------------|
|namespace|[rallyhere::allocator](namespacerallyhere_1_1allocator.xml.md#namespacerallyhere_1_1allocator)||
|struct|[rallyhere::a2s_simple_response](structrallyhere_1_1a2s__simple__response.xml.md#structrallyhere_1_1a2s__simple__response)||
|struct|[rallyhere::server_info](structrallyhere_1_1server__info.xml.md#structrallyhere_1_1server__info)||
|struct|[rallyhere::A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram)||
|class|[rallyhere::Status](classrallyhere_1_1Status.xml.md#classrallyhere_1_1Status)||
|enum|[a2s_query](namespacerallyhere.xml.md#namespacerallyhere_1a0afcd5afb55b621622fd5e2f7b4a0566)||
|typedef|[memory_buffer](namespacerallyhere.xml.md#namespacerallyhere_1a7f5299480c8b27fb1ecb8a48090074be)||
|typedef|[string](namespacerallyhere.xml.md#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e)||
|typedef|[stringstream](namespacerallyhere.xml.md#namespacerallyhere_1ae6c398b5cead92743640822e3a687302)||
|typedef|[vector](namespacerallyhere.xml.md#namespacerallyhere_1a9822d3fb78edd3730ef97afb9b2c06d9)||
|typedef|[string_body](namespacerallyhere.xml.md#namespacerallyhere_1a6a86fdf96ef7e4a9876d443a6bba29ec)||
|constexpr unsigned int|[A2S_MAX_DATA_SIZE](namespacerallyhere.xml.md#namespacerallyhere_1a17f51994891fa6aade759ea62f75fbdd)||
|constexpr unsigned int|[UDP_HEADER_SIZE](namespacerallyhere.xml.md#namespacerallyhere_1a1b4c37b3daf7e45441669ac1b5a251f5)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) const char *|[status_text](namespacerallyhere.xml.md#namespacerallyhere_1a01f61b99849af1519d941b913b1fb746)([RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) )||
|constexpr bool|[is_error](namespacerallyhere.xml.md#namespacerallyhere_1aaebe4b7543e4994038781a51d57e9bae)([RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) err)||
|constexpr bool|[is_cancelled](namespacerallyhere.xml.md#namespacerallyhere_1a32e5c5dcdf2d843bff452a896dc45d48)([RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) err)||
## Enum




### `a2s_query` {#namespacerallyhere_1a0afcd5afb55b621622fd5e2f7b4a0566}




| Enumerator | Initializer|
|------------|------------|
|`info`|`= 0x54`|
|`players`|`= 0x55`|
|`rules`|`= 0x56`|



## Typedef



### `memory_buffer` {#namespacerallyhere_1a7f5299480c8b27fb1ecb8a48090074be}

`using rallyhere::memory_buffer = typedef fmt::basic_memory_buffer<char, fmt::inline_buffer_size, rallyhere::allocator::allocator<char> >`




### `string` {#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e}

`using rallyhere::string = typedef std::basic_string<char, std::char_traits<char>, rallyhere::allocator::allocator<char> >`




### `stringstream` {#namespacerallyhere_1ae6c398b5cead92743640822e3a687302}

`using rallyhere::stringstream = typedef std::basic_stringstream<char, std::char_traits<char>, rallyhere::allocator::allocator<char> >`




### `vector` {#namespacerallyhere_1a9822d3fb78edd3730ef97afb9b2c06d9}

`using rallyhere::vector = typedef std::vector<T, rallyhere::allocator::allocator<T> >`




### `string_body` {#namespacerallyhere_1a6a86fdf96ef7e4a9876d443a6bba29ec}

`using rallyhere::string_body = typedef boost::beast::http::basic_string_body<char, std::char_traits<char>, rallyhere::allocator::allocator<char> >`





## Var



### `A2S_MAX_DATA_SIZE` {#namespacerallyhere_1a17f51994891fa6aade759ea62f75fbdd}

`constexpr unsigned int rallyhere::A2S_MAX_DATA_SIZE`






### `UDP_HEADER_SIZE` {#namespacerallyhere_1a1b4c37b3daf7e45441669ac1b5a251f5}

`constexpr unsigned int rallyhere::UDP_HEADER_SIZE`







## Func



### `status_text` {#namespacerallyhere_1a01f61b99849af1519d941b913b1fb746}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) const char * status_text([RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) )

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)||

#### Description






### `is_error` {#namespacerallyhere_1aaebe4b7543e4994038781a51d57e9bae}

constexpr bool is_error([RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) err)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|err|

#### Description






### `is_cancelled` {#namespacerallyhere_1a32e5c5dcdf2d843bff452a896dc45d48}

constexpr bool is_cancelled([RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) err)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|err|

#### Description







