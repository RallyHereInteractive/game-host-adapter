# rallyhere::A2SDatagram Struct



## Summary
| Kind | View | Description |
|------|------|-------------|
|boost::span< uint8_t >|[Data](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a9c73db3ef61f5eb10bf27aefb39b7262)||
|boost::span< uint8_t >::iterator|[next](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a0ee3cc43224f77e140c87c5036101a3a)||
|bool|[overflowed](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1aad00977149cc81d3963f521a2464daf3)||
||[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a4bac929dddade7f7843a9de8fc3b4749)(uint8_t(&) data)||
||[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1aa5f0396e77cd77738ef437a4c230eb80)(std::array< Element, N > & data)||
||[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a70adb04f9e749c935eb3d9dd0cb8b658)(Element * data, size_t len)||
|void|[put](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a10116e2a2d97418b540a209b216ad4ff)(const uint8_t * data, size_t len)||
|[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) &|[operator<<](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a1606755a90627bd00a74321bd02be2e9)(const [rallyhere::string](namespacerallyhere.xml.md#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e) & data)||
|[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) &|[operator<<](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a42e8ab242adebd5c18c9842ee349311a)(const uint8_t & data)||
|[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) &|[operator<<](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a9725748bf2b5f3f941b2f57af72d2edd)(const int16_t & data)||
|[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) &|[operator<<](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a711070759068065b89522e3a34c911b0)(const int32_t & data)||
|[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) &|[operator<<](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a7076938d076d90eb8abc1738a36144ff)(const [server_info](structrallyhere_1_1server__info.xml.md#structrallyhere_1_1server__info) & info)||
|[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) &|[operator>>](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a907cc503694552fb7f0adc02642f58a4)([rallyhere::string](namespacerallyhere.xml.md#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e) & data)||
|[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) &|[operator>>](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1acb762317e15cc47549dbcc163b5cee90)(uint8_t & data)||
|void|[get](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1ac2409ea27ef627a7c57fbd3ef2ac2d22)(uint8_t * data, size_t len)||
|[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) &|[operator>>](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1aeed62009f3e17be72cc76dee941f7427)(int16_t & data)||
|[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) &|[operator>>](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a1e6b73015c9f39f0074dbf9f18a6a63f)(int32_t & data)||
|[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) &|[operator>>](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1a31ba12d0497ce35d895778beccf92316)([server_info](structrallyhere_1_1server__info.xml.md#structrallyhere_1_1server__info) & info)||
|size_t|[size](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram_1ab6cae52b4c701996121e08d860cedc69)()||
## Public-attrib



### `Data` {#structrallyhere_1_1A2SDatagram_1a9c73db3ef61f5eb10bf27aefb39b7262}

`boost::span<uint8_t> rallyhere::A2SDatagram::Data`






### `next` {#structrallyhere_1_1A2SDatagram_1a0ee3cc43224f77e140c87c5036101a3a}

`boost::span<uint8_t>::iterator rallyhere::A2SDatagram::next`






### `overflowed` {#structrallyhere_1_1A2SDatagram_1aad00977149cc81d3963f521a2464daf3}

`bool rallyhere::A2SDatagram::overflowed`







## Public-func



### `A2SDatagram` {#structrallyhere_1_1A2SDatagram_1a4bac929dddade7f7843a9de8fc3b4749}

 A2SDatagram(uint8_t(&) data)

#### Parameters

| Type | Name |
|------|------|
|uint8_t(&)|data|

#### Description






### `A2SDatagram` {#structrallyhere_1_1A2SDatagram_1aa5f0396e77cd77738ef437a4c230eb80}

 A2SDatagram(std::array< Element, N > & data)

#### Parameters

| Type | Name |
|------|------|
|std::array< Element, N > &|data|

#### Description






### `A2SDatagram` {#structrallyhere_1_1A2SDatagram_1a70adb04f9e749c935eb3d9dd0cb8b658}

 A2SDatagram(Element * data, size_t len)

#### Parameters

| Type | Name |
|------|------|
|Element *|data|
|size_t|len|

#### Description






### `put` {#structrallyhere_1_1A2SDatagram_1a10116e2a2d97418b540a209b216ad4ff}

void put(const uint8_t * data, size_t len)

#### Parameters

| Type | Name |
|------|------|
|const uint8_t *|data|
|size_t|len|

#### Description






### `operator<<` {#structrallyhere_1_1A2SDatagram_1a1606755a90627bd00a74321bd02be2e9}

[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) & operator<<(const [rallyhere::string](namespacerallyhere.xml.md#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e) & data)

#### Parameters

| Type | Name |
|------|------|
|const [rallyhere::string](namespacerallyhere.xml.md#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e) &|data|

#### Description






### `operator<<` {#structrallyhere_1_1A2SDatagram_1a42e8ab242adebd5c18c9842ee349311a}

[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) & operator<<(const uint8_t & data)

#### Parameters

| Type | Name |
|------|------|
|const uint8_t &|data|

#### Description






### `operator<<` {#structrallyhere_1_1A2SDatagram_1a9725748bf2b5f3f941b2f57af72d2edd}

[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) & operator<<(const int16_t & data)

#### Parameters

| Type | Name |
|------|------|
|const int16_t &|data|

#### Description






### `operator<<` {#structrallyhere_1_1A2SDatagram_1a711070759068065b89522e3a34c911b0}

[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) & operator<<(const int32_t & data)

#### Parameters

| Type | Name |
|------|------|
|const int32_t &|data|

#### Description






### `operator<<` {#structrallyhere_1_1A2SDatagram_1a7076938d076d90eb8abc1738a36144ff}

[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) & operator<<(const [server_info](structrallyhere_1_1server__info.xml.md#structrallyhere_1_1server__info) & info)

#### Parameters

| Type | Name |
|------|------|
|const [server_info](structrallyhere_1_1server__info.xml.md#structrallyhere_1_1server__info) &|info|

#### Description






### `operator>>` {#structrallyhere_1_1A2SDatagram_1a907cc503694552fb7f0adc02642f58a4}

[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) & operator>>([rallyhere::string](namespacerallyhere.xml.md#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e) & data)

#### Parameters

| Type | Name |
|------|------|
|[rallyhere::string](namespacerallyhere.xml.md#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e) &|data|

#### Description






### `operator>>` {#structrallyhere_1_1A2SDatagram_1acb762317e15cc47549dbcc163b5cee90}

[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) & operator>>(uint8_t & data)

#### Parameters

| Type | Name |
|------|------|
|uint8_t &|data|

#### Description






### `get` {#structrallyhere_1_1A2SDatagram_1ac2409ea27ef627a7c57fbd3ef2ac2d22}

void get(uint8_t * data, size_t len)

#### Parameters

| Type | Name |
|------|------|
|uint8_t *|data|
|size_t|len|

#### Description






### `operator>>` {#structrallyhere_1_1A2SDatagram_1aeed62009f3e17be72cc76dee941f7427}

[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) & operator>>(int16_t & data)

#### Parameters

| Type | Name |
|------|------|
|int16_t &|data|

#### Description






### `operator>>` {#structrallyhere_1_1A2SDatagram_1a1e6b73015c9f39f0074dbf9f18a6a63f}

[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) & operator>>(int32_t & data)

#### Parameters

| Type | Name |
|------|------|
|int32_t &|data|

#### Description






### `operator>>` {#structrallyhere_1_1A2SDatagram_1a31ba12d0497ce35d895778beccf92316}

[A2SDatagram](structrallyhere_1_1A2SDatagram.xml.md#structrallyhere_1_1A2SDatagram) & operator>>([server_info](structrallyhere_1_1server__info.xml.md#structrallyhere_1_1server__info) & info)

#### Parameters

| Type | Name |
|------|------|
|[server_info](structrallyhere_1_1server__info.xml.md#structrallyhere_1_1server__info) &|info|

#### Description






### `size` {#structrallyhere_1_1A2SDatagram_1ab6cae52b4c701996121e08d860cedc69}

size_t size()

#### Parameters

| Type | Name |
|------|------|

#### Description







