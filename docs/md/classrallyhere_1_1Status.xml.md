# rallyhere::Status Class



## Summary
| Kind | View | Description |
|------|------|-------------|
|[RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[m_Code](classrallyhere_1_1Status.xml.md#classrallyhere_1_1Status_1ad310138441f6a57ce6e026dedc70134c)||
|[string](namespacerallyhere.xml.md#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e)|[m_Message](classrallyhere_1_1Status.xml.md#classrallyhere_1_1Status_1a0e46aec2426f5c2ff85e62dda8f87ef0)||
||[Status](classrallyhere_1_1Status.xml.md#classrallyhere_1_1Status_1aa3fd8a836a74be4497179fc788c02521)()||
||[Status](classrallyhere_1_1Status.xml.md#classrallyhere_1_1Status_1a537c8dd791bd3a6fdd59796dff75fac8)([RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) code)||
||[Status](classrallyhere_1_1Status.xml.md#classrallyhere_1_1Status_1a7ad71b78f8e8422d7c5dc4b463eccdaf)([RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) code, const [string](namespacerallyhere.xml.md#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e) & message)||
|[RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[code](classrallyhere_1_1Status.xml.md#classrallyhere_1_1Status_1ad0de2e63b84f8da0d827c6e77b039a7d)()||
|std::string_view|[code_message](classrallyhere_1_1Status.xml.md#classrallyhere_1_1Status_1a308114c9416a03fc066cab687947e10a)()||
|std::string_view|[message](classrallyhere_1_1Status.xml.md#classrallyhere_1_1Status_1aa7f557c4cfb13ebe562cfe6f3d3a44cc)()||
|bool|[ok](classrallyhere_1_1Status.xml.md#classrallyhere_1_1Status_1a2da5027faf42d9ac874787c25c7afc7e)()||
||[operator bool](classrallyhere_1_1Status.xml.md#classrallyhere_1_1Status_1a772c946fd1d0e06a189c1f68b1c838fc)()||
## Private-attrib



### `m_Code` {#classrallyhere_1_1Status_1ad310138441f6a57ce6e026dedc70134c}

`RallyHereStatusCode rallyhere::Status::m_Code`






### `m_Message` {#classrallyhere_1_1Status_1a0e46aec2426f5c2ff85e62dda8f87ef0}

`string rallyhere::Status::m_Message`







## Public-func



### `Status` {#classrallyhere_1_1Status_1aa3fd8a836a74be4497179fc788c02521}

 Status()

#### Parameters

| Type | Name |
|------|------|

#### Description






### `Status` {#classrallyhere_1_1Status_1a537c8dd791bd3a6fdd59796dff75fac8}

 Status([RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) code)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|code|

#### Description






### `Status` {#classrallyhere_1_1Status_1a7ad71b78f8e8422d7c5dc4b463eccdaf}

 Status([RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) code, const [string](namespacerallyhere.xml.md#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e) & message)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|code|
|const [string](namespacerallyhere.xml.md#namespacerallyhere_1a06e017abf8a0212e162b377dc793078e) &|message|

#### Description






### `code` {#classrallyhere_1_1Status_1ad0de2e63b84f8da0d827c6e77b039a7d}

[RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) code()

#### Parameters

| Type | Name |
|------|------|

#### Description






### `code_message` {#classrallyhere_1_1Status_1a308114c9416a03fc066cab687947e10a}

std::string_view code_message()

#### Parameters

| Type | Name |
|------|------|

#### Description






### `message` {#classrallyhere_1_1Status_1aa7f557c4cfb13ebe562cfe6f3d3a44cc}

std::string_view message()

#### Parameters

| Type | Name |
|------|------|

#### Description






### `ok` {#classrallyhere_1_1Status_1a2da5027faf42d9ac874787c25c7afc7e}

bool ok()

#### Parameters

| Type | Name |
|------|------|

#### Description






### `operator bool` {#classrallyhere_1_1Status_1a772c946fd1d0e06a189c1f68b1c838fc}

 operator bool()

#### Parameters

| Type | Name |
|------|------|

#### Description







