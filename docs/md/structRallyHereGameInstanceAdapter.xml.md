# RallyHereGameInstanceAdapter Struct



Opaque type and handle to a Rally Here Game Instance Adapter.
This struct is intentionally empty because its true interface is only defined internally. Any methods which take this as the first argument can be thought of as methods on this struct.
Callbacks are either called immediately at the call site or when calling 
[rallyhere_tick()](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a4db3f8b5b27ab240d0edb337719dd2fd).## Summary
| Kind | View | Description |
|------|------|-------------|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_set_log_callback](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1ad1d09e6062ad3db182a66f1829c7479b)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereLogCallback](c__api_8h.xml.md#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_create_game_instance_adapter](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a6122df1cede4159d0d30be0eb7b45463)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_create_game_instance_adaptern](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1aeef6eea3f33281f1a997fcb3462effd3)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, unsigned int arguments_length)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_create_game_instance_adapter_with_logger](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a49786a749d31ba4c426b4b0059709704)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, [RallyHereLogCallback](c__api_8h.xml.md#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_create_game_instance_adaptern_with_logger](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a85db0b2f34bb7f67347ff9bbe5ff2beb)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, unsigned int arguments_length, [RallyHereLogCallback](c__api_8h.xml.md#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_destroy_game_instance_adapter](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a291d4acc324c8dbab8786391fb5557ec)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)|Destroys the adapter.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_tick](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a4db3f8b5b27ab240d0edb337719dd2fd)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)|Tick that's expected to be called every frame.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_healthy](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1af3d04dce0166b8f1ba22de46ef74da9f)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)|Mark the adapter as healthy.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_connect](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a4a665dd0eeb67b49043e7a54e0d397ed)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Begin the process of connecting to the appropriate game host.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_ready](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Tell the game host that this GameInstance can now be allocated.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_on_allocated_callback](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a3c96a6e133beb24d2f54af05b13a713c)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereOnAllocatedCallback](c__api_8h.xml.md#c__api_8h_1a8664a168347b6152d05be2870fff5113) callback, void * user_data)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_test_fake_allocation_response](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1adddb6d6ebd068feef7972c6188ebecfe)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * fake_data)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_allocate](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1afa437234d06d99d50e63479ae8ca3bd4)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Tell the game host that this GameInstance has been allocated.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_reserve](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, unsigned int timeout_seconds, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) reserve_callback, void * reserve_user_data, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) ready_callback, void * ready_user_data)|Tell the game host to reserve this game instance rather than marking it ready.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_reserve_unconditional](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1afb679e1014b59a9a246c7c5b02c1062b)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Tell the game host to reserve this game instance rather than marking it ready.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_on_soft_stop_callback](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a662073848a735b58f79f18df20196cf1)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|The callback to trigger when the game host wants to stop the game instance.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_external_soft_stop_requested](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a12b6641eae79cfca381c5a4adc4cb2aa)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)|Inform the adapter that a soft stop has been requested.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_get_labels](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1ab3f0ff7c642bb703f228782a4e6aae93)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)|Get the current string map for the labels.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_set_labels](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a80e5b040228f255c243f45317f04eeac)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Set the current string map for the labels.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_get_additional_info](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a91c2c856bafd227bce07837642dba164)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)|Get the current string map for the additional info.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_set_additional_info](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a059a56aff73e1901768e2511f664f9f8)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Set the current string map for the additional info.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_get_annotations](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a5735f0da0826fe530291423a214ce456)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)|Get the current string map for the annotations.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_set_annotations](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a6f5ea8bf77b08b81ffefe160157a4ca0)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Set the current string map for the annotations.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_stats_base](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a71b7533f99e051e4a6969d28521e1e88)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStatsBase](structRallyHereStatsBase.xml.md#structRallyHereStatsBase) * stats, const [RallyHereStatsBaseProvided](structRallyHereStatsBaseProvided.xml.md#structRallyHereStatsBaseProvided) * provided, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Set the base stats for the game instance.|
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_stats_gauge](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a4c6cb8ff5eb5fc79e352e64fea634582)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, double value, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_stats_gauge_with_labels](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1acdb70d34626a0951b5123ca5a8c90ba7)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, double value, const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) labels, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_stats_arbitrary](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a9b1b98ccaabfe4412ab311c4e4bf12df)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, const char * data, unsigned int data_size, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_stats_arbitrary_move](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a8a05228013b9ad1fcdb0dbeea74d2d10)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, char * data, unsigned int data_size, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, char *data, void *user_data) callback, void * user_data)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_metrics_immediate_action](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1aaa958e15c14a2100748048adb0cf7277)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereMetricDefinition](structRallyHereMetricDefinition.xml.md#structRallyHereMetricDefinition) * definition, const [RallyHereMetricAction](structRallyHereMetricAction.xml.md#structRallyHereMetricAction) * action, [RallyHereMetricFlush](c__metrics_8h.xml.md#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa) flush)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_metrics_immediate_action_with_time](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a28bcafe6ae1c9ebf393fb93f80253f73)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereMetricDefinition](structRallyHereMetricDefinition.xml.md#structRallyHereMetricDefinition) * definition, const [RallyHereMetricAction](structRallyHereMetricAction.xml.md#structRallyHereMetricAction) * action, double timestamp, [RallyHereMetricFlush](c__metrics_8h.xml.md#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa) flush)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_metrics_statsd_output](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a109f917f5f91e81768a91e794125dc0d)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char ** data, size_t * data_size)||
|[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_metrics_graphite_output](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a221c9aa3189025c9ed28f73f812840f6)([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char ** data, size_t * data_size)||
## Logging

Logging levels that will passed to the optional log callback function 


### `rallyhere_set_log_callback` {#structRallyHereGameInstanceAdapter_1ad1d09e6062ad3db182a66f1829c7479b}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_set_log_callback([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereLogCallback](c__api_8h.xml.md#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|[RallyHereLogCallback](c__api_8h.xml.md#c__api_8h_1a64a7679e581842654beca7ae63664a26)|callback|
|void *|user_data|

#### Description



Sets a custom logger that can handle logs from inside of the server. 




## Lifetime

Manages the lifetime of a Rally Here Game Instance Adapter. 


### `rallyhere_create_game_instance_adapter` {#structRallyHereGameInstanceAdapter_1a6122df1cede4159d0d30be0eb7b45463}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_create_game_instance_adapter([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) *|adapter|
|const char *|arguments|

#### Description



Create a new adapter using a single string to represent the command line arguments it will use for initialization. 



### `rallyhere_create_game_instance_adaptern` {#structRallyHereGameInstanceAdapter_1aeef6eea3f33281f1a997fcb3462effd3}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_create_game_instance_adaptern([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, unsigned int arguments_length)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) *|adapter|
|const char *|arguments|
|unsigned int|arguments_length|

#### Description



Create a new adapter using a C-style array to represent the command line arguments it will use for initialization. 



### `rallyhere_create_game_instance_adapter_with_logger` {#structRallyHereGameInstanceAdapter_1a49786a749d31ba4c426b4b0059709704}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_create_game_instance_adapter_with_logger([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, [RallyHereLogCallback](c__api_8h.xml.md#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) *|adapter|
|const char *|arguments|
|[RallyHereLogCallback](c__api_8h.xml.md#c__api_8h_1a64a7679e581842654beca7ae63664a26)|callback|
|void *|user_data|

#### Description



Create a new adapter using a single string to represent the command line arguments it will use for initialization and a custom logging function to be used from the moment it is created. 

#### See also

[rallyhere_set_log_callback](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1ad1d09e6062ad3db182a66f1829c7479b)


### `rallyhere_create_game_instance_adaptern_with_logger` {#structRallyHereGameInstanceAdapter_1a85db0b2f34bb7f67347ff9bbe5ff2beb}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_create_game_instance_adaptern_with_logger([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, unsigned int arguments_length, [RallyHereLogCallback](c__api_8h.xml.md#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) *|adapter|
|const char *|arguments|
|unsigned int|arguments_length|
|[RallyHereLogCallback](c__api_8h.xml.md#c__api_8h_1a64a7679e581842654beca7ae63664a26)|callback|
|void *|user_data|

#### Description



Create a new adapter using a C-style array to represent the command line arguments it will use for initialization and a custom logging function to be used from the moment it is created. 

#### See also

[rallyhere_set_log_callback](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1ad1d09e6062ad3db182a66f1829c7479b)


### `rallyhere_destroy_game_instance_adapter` {#structRallyHereGameInstanceAdapter_1a291d4acc324c8dbab8786391fb5557ec}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_destroy_game_instance_adapter([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|

#### Description

Destroys the adapter.

Before any memory is freed every registered callback is called with the status RH_STATUS_CANCELLED so that any memory associated with the callback's user data can be freed. 




## Repeated at regular intervals



### `rallyhere_tick` {#structRallyHereGameInstanceAdapter_1a4db3f8b5b27ab240d0edb337719dd2fd}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_tick([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|

#### Description

Tick that's expected to be called every frame.

This is used to check for any messages from the game host. All deferred message processing and callbacks will be handled during this tick. 

#### Returns
RH_STATUS_OK if this adapter can still be used, otherwise an error code. 



### `rallyhere_healthy` {#structRallyHereGameInstanceAdapter_1af3d04dce0166b8f1ba22de46ef74da9f}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_healthy([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|

#### Description

Mark the adapter as healthy.

This is used to tell the game host that this adapter is still alive and well. Should be called at a regular interval. Every 5 seconds is a good rule of thumb. Currently only used by Agones. SIC relies on an open metrics endpoint created by this SDK. i3D relies on the Arcus socket connection. Multiplay reliies on the A2S socket connection. 




## State request and callbacks



### `rallyhere_connect` {#structRallyHereGameInstanceAdapter_1a4a665dd0eeb67b49043e7a54e0d397ed}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_connect([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Begin the process of connecting to the appropriate game host.

The provided callback will be called when the process completes with the result of the connection attempt. 



### `rallyhere_ready` {#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_ready([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Tell the game host that this GameInstance can now be allocated.

The provided callback will be called when the process completes with the result of the game host being told this is ready. 



### `rallyhere_on_allocated_callback` {#structRallyHereGameInstanceAdapter_1a3c96a6e133beb24d2f54af05b13a713c}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_on_allocated_callback([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereOnAllocatedCallback](c__api_8h.xml.md#c__api_8h_1a8664a168347b6152d05be2870fff5113) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|[RallyHereOnAllocatedCallback](c__api_8h.xml.md#c__api_8h_1a8664a168347b6152d05be2870fff5113)|callback|
|void *|user_data|

#### Description



Register the callback which will be called when the game host has assigned an allocation to this adapter. 



### `rallyhere_test_fake_allocation_response` {#structRallyHereGameInstanceAdapter_1adddb6d6ebd068feef7972c6188ebecfe}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_test_fake_allocation_response([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * fake_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char *|fake_data|

#### Description



Fake an allocation. This should only be used for testing purposes. 



### `rallyhere_allocate` {#structRallyHereGameInstanceAdapter_1afa437234d06d99d50e63479ae8ca3bd4}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_allocate([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Tell the game host that this GameInstance has been allocated.

When using 
[rallyhere_ready()](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)this is used to tell the Game Host that you have recieved the allocation information and have done the appropriate work. In the[rallyhere_reserve()](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19)case this is telling the Game Host that you are allocated by some other means. The provided callback will be called when the process completes with the result


### `rallyhere_reserve` {#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_reserve([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, unsigned int timeout_seconds, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) reserve_callback, void * reserve_user_data, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) ready_callback, void * ready_user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|unsigned int|timeout_seconds|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|reserve_callback|
|void *|reserve_user_data|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|ready_callback|
|void *|ready_user_data|

#### Description

Tell the game host to reserve this game instance rather than marking it ready.

This will stop the game host from destroying this game instance based on any ready timeouts. This is to be used when the game instance is manually handling sessions in a way that isn't expected to be handled by the normal matchmaking route. The provided callback will be called when the process completes with the result 



### `rallyhere_reserve_unconditional` {#structRallyHereGameInstanceAdapter_1afb679e1014b59a9a246c7c5b02c1062b}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_reserve_unconditional([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Tell the game host to reserve this game instance rather than marking it ready.

This will stop the game host from destroying this game instance based on any ready timeouts. This is to be used when the game instance is manually handling sessions in a way that isn't expected to be handled by the normal matchmaking route. The provided callback will be called when the process completes with the result 




## Soft stop

There are times when the game hosting system would like your instance to shut down, but it does not need to happen immediately. In these situations a soft-stop will be requested so that your instance can finish any currently running games and then stop gracefully. Currently only used by SIC and i3D. 


### `rallyhere_on_soft_stop_callback` {#structRallyHereGameInstanceAdapter_1a662073848a735b58f79f18df20196cf1}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_on_soft_stop_callback([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

The callback to trigger when the game host wants to stop the game instance.

In SIC this is expected to come from the SIGTERM handler. In i3D this is expected to come from the Arcus commands 



### `rallyhere_external_soft_stop_requested` {#structRallyHereGameInstanceAdapter_1a12b6641eae79cfca381c5a4adc4cb2aa}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_external_soft_stop_requested([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|

#### Description

Inform the adapter that a soft stop has been requested.

There are situations where soft stop comes from something external to the adapter and the adapter needs to be told. In this case the adapter will also trigger the on soft stop callback. This is not a deferred call, so the registered callback will be called immediately.
In SIC the SIGTERM handler can't always be overridden by this adapter. In those situations the caller is expected to call this function as part of their SIGTERM handling. 




## Stats labels

This is a map of string to string that can be used to store labels for stats. In SIC mode this will be exported as labels on every single prometheus metric. These must be set before calling 
[rallyhere_ready()](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)or[rallyhere_reserve()](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19)is called.

### `rallyhere_get_labels` {#structRallyHereGameInstanceAdapter_1ab3f0ff7c642bb703f228782a4e6aae93}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_get_labels([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|[RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) *|map|

#### Description

Get the current string map for the labels.

Caller is responsible for freeing the string map with rallyhere_string_map_destroy. 



### `rallyhere_set_labels` {#structRallyHereGameInstanceAdapter_1a80e5b040228f255c243f45317f04eeac}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_set_labels([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|map|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Set the current string map for the labels.

The string map will be copied. Provide a callback to be notified when the labels have been set. These must be set before calling 
[rallyhere_ready()](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)or[rallyhere_reserve()](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19)is called.



## Additional info

This is a map of string to string that can be used to store additional information about the game instance. In SIC mode this will be exported as labels on the "instance_info" gauge. These can be set after calls to 
[rallyhere_ready()](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)or[rallyhere_reserve()](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19).

### `rallyhere_get_additional_info` {#structRallyHereGameInstanceAdapter_1a91c2c856bafd227bce07837642dba164}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_get_additional_info([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|[RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) *|map|

#### Description

Get the current string map for the additional info.

Caller is responsible for freeing the string map with rallyhere_string_map_destroy. 



### `rallyhere_set_additional_info` {#structRallyHereGameInstanceAdapter_1a059a56aff73e1901768e2511f664f9f8}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_set_additional_info([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|map|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Set the current string map for the additional info.

The string map will be copied. Provide a callback to be notified when the labels have been set. These can be set after calls to 
[rallyhere_ready()](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)or[rallyhere_reserve()](structRallyHereGameInstanceAdapter.xml.md#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19).



## Stats annotations



### `rallyhere_get_annotations` {#structRallyHereGameInstanceAdapter_1a5735f0da0826fe530291423a214ce456}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_get_annotations([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|[RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) *|map|

#### Description

Get the current string map for the annotations.

Caller is responsible for freeing the string map. 



### `rallyhere_set_annotations` {#structRallyHereGameInstanceAdapter_1a6f5ea8bf77b08b81ffefe160157a4ca0}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_set_annotations([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|map|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Set the current string map for the annotations.

The string map will be copied. Provide a callback to be notified when the labels have been set. 




## Stats core



### `rallyhere_stats_base` {#structRallyHereGameInstanceAdapter_1a71b7533f99e051e4a6969d28521e1e88}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_stats_base([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStatsBase](structRallyHereStatsBase.xml.md#structRallyHereStatsBase) * stats, const [RallyHereStatsBaseProvided](structRallyHereStatsBaseProvided.xml.md#structRallyHereStatsBaseProvided) * provided, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereStatsBase](structRallyHereStatsBase.xml.md#structRallyHereStatsBase) *|stats|
|const [RallyHereStatsBaseProvided](structRallyHereStatsBaseProvided.xml.md#structRallyHereStatsBaseProvided) *|provided|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Set the base stats for the game instance.

Provide a callback to be notified when the stats have been set These will be exported as labels on the "instance_info" gauge. These will be exported as part of the A2S_INFO query. 




## Stats gauges



### `rallyhere_stats_gauge` {#structRallyHereGameInstanceAdapter_1a4c6cb8ff5eb5fc79e352e64fea634582}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_stats_gauge([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, double value, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char *|name|
|double|value|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description



Set the value of a gauge. Provide a callback to be notified when the stats have been set. These stats will be exported with the current labels 



### `rallyhere_stats_gauge_with_labels` {#structRallyHereGameInstanceAdapter_1acdb70d34626a0951b5123ca5a8c90ba7}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_stats_gauge_with_labels([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, double value, const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360) labels, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char *|name|
|double|value|
|const [RallyHereStringMapPtr](c__api_8h.xml.md#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|labels|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description



Set the value of a gauge. Provide a callback to be notified when the stats have been set. 
#### Parameters

labels
: The labels to export with the gauge 




## Stats arbitrary



### `rallyhere_stats_arbitrary` {#structRallyHereGameInstanceAdapter_1a9b1b98ccaabfe4412ab311c4e4bf12df}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_stats_arbitrary([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, const char * data, unsigned int data_size, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char *|name|
|const char *|data|
|unsigned int|data_size|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description



Set the value of an arbitrary stat. Provide a callback to be notified when the stats have been set. 
#### Parameters

data
: The data to export with the stat which is copied by the call 



### `rallyhere_stats_arbitrary_move` {#structRallyHereGameInstanceAdapter_1a8a05228013b9ad1fcdb0dbeea74d2d10}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_stats_arbitrary_move([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, char * data, unsigned int data_size, void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, char *data, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char *|name|
|char *|data|
|unsigned int|data_size|
|void(*)(const [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, char *data, void *user_data)|callback|
|void *|user_data|

#### Description



Set the value of an arbitrary stat. Provide a callback to be notified when the stats have been set. 
#### Parameters

data
: The data to export with the stat which must have been allocated with the same alloc as set for the adapter. This function takes ownership of that pointer. If the error code is not RH_OK then the pointer is not taken and the caller is responsible 




## Push metrics

Used for metrics that are push rather than pull. User of the SDK must choose between statsd or graphite as the target for the push.


#### attention
This is still a work in progress and the API is provided with the expectation that there will be better handling of automatically pushing those metrics in the future. The metric API is stable and generates the correct output foer statsd and graphite, but there is no internal code to send those values out yet. 


### `rallyhere_metrics_immediate_action` {#structRallyHereGameInstanceAdapter_1aaa958e15c14a2100748048adb0cf7277}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_metrics_immediate_action([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereMetricDefinition](structRallyHereMetricDefinition.xml.md#structRallyHereMetricDefinition) * definition, const [RallyHereMetricAction](structRallyHereMetricAction.xml.md#structRallyHereMetricAction) * action, [RallyHereMetricFlush](c__metrics_8h.xml.md#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa) flush)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereMetricDefinition](structRallyHereMetricDefinition.xml.md#structRallyHereMetricDefinition) *|definition|
|const [RallyHereMetricAction](structRallyHereMetricAction.xml.md#structRallyHereMetricAction) *|action|
|[RallyHereMetricFlush](c__metrics_8h.xml.md#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa)|flush|

#### Description






### `rallyhere_metrics_immediate_action_with_time` {#structRallyHereGameInstanceAdapter_1a28bcafe6ae1c9ebf393fb93f80253f73}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](c__status_8h.xml.md#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_metrics_immediate_action_with_time([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereMetricDefinition](structRallyHereMetricDefinition.xml.md#structRallyHereMetricDefinition) * definition, const [RallyHereMetricAction](structRallyHereMetricAction.xml.md#structRallyHereMetricAction) * action, double timestamp, [RallyHereMetricFlush](c__metrics_8h.xml.md#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa) flush)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereMetricDefinition](structRallyHereMetricDefinition.xml.md#structRallyHereMetricDefinition) *|definition|
|const [RallyHereMetricAction](structRallyHereMetricAction.xml.md#structRallyHereMetricAction) *|action|
|double|timestamp|
|[RallyHereMetricFlush](c__metrics_8h.xml.md#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa)|flush|

#### Description






### `rallyhere_metrics_statsd_output` {#structRallyHereGameInstanceAdapter_1a109f917f5f91e81768a91e794125dc0d}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_metrics_statsd_output([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char ** data, size_t * data_size)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char **|data|
|size_t *|data_size|

#### Description






### `rallyhere_metrics_graphite_output` {#structRallyHereGameInstanceAdapter_1a221c9aa3189025c9ed28f73f812840f6}

[RH_EXPORT](c__platform_8h.xml.md#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_metrics_graphite_output([RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char ** data, size_t * data_size)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](c__api_8h.xml.md#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char **|data|
|size_t *|data_size|

#### Description







