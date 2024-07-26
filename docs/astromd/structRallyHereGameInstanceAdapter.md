---
title: RallyHereGameInstanceAdapter Struct
---


Opaque type and handle to a Rally Here Game Instance Adapter.
This struct is intentionally empty because its true interface is only defined internally. Any methods which take this as the first argument can be thought of as methods on this struct.
Callbacks are either called immediately at the call site or when calling 
[rallyhere_tick()](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a4db3f8b5b27ab240d0edb337719dd2fd).## Summary
| Kind | View | Description |
|------|------|-------------|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_set_log_callback](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1ad1d09e6062ad3db182a66f1829c7479b)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereLogCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_create_game_instance_adapter](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a6122df1cede4159d0d30be0eb7b45463)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_create_game_instance_adaptern](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1aeef6eea3f33281f1a997fcb3462effd3)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, unsigned int arguments_length)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_create_game_instance_adapter_with_logger](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a49786a749d31ba4c426b4b0059709704)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, [RallyHereLogCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_create_game_instance_adaptern_with_logger](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a85db0b2f34bb7f67347ff9bbe5ff2beb)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, unsigned int arguments_length, [RallyHereLogCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_destroy_game_instance_adapter](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a291d4acc324c8dbab8786391fb5557ec)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)|Destroys the adapter.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_tick](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a4db3f8b5b27ab240d0edb337719dd2fd)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)|Tick that's expected to be called every frame.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_healthy](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1af3d04dce0166b8f1ba22de46ef74da9f)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)|Mark the adapter as healthy.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_connect](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a4a665dd0eeb67b49043e7a54e0d397ed)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Begin the process of connecting to the appropriate game host.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_ready](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Tell the game host that this GameInstance can now be allocated.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_on_allocated_callback](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a3c96a6e133beb24d2f54af05b13a713c)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereOnAllocatedCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a8664a168347b6152d05be2870fff5113) callback, void * user_data)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_test_fake_allocation_response](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1adddb6d6ebd068feef7972c6188ebecfe)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * fake_data)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_allocate](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1afa437234d06d99d50e63479ae8ca3bd4)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Tell the game host that this GameInstance has been allocated.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_get_public_host_and_port](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a31b26a826279e5dbffc672c887f114a7)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)|Get any configured public host and port information.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_reserve](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, unsigned int timeout_seconds, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) reserve_callback, void * reserve_user_data, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) ready_callback, void * ready_user_data)|Tell the game host to reserve this game instance rather than marking it ready.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_reserve_unconditional](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1afb679e1014b59a9a246c7c5b02c1062b)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Tell the game host to reserve this game instance rather than marking it ready.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_on_soft_stop_callback](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a662073848a735b58f79f18df20196cf1)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|The callback to trigger when the game host wants to stop the game instance. This callback should be implmeneted as if it were being called from a signal handler. It should do only the minimum amount of work necessary to inform the rest of the system that a soft stop has been requested. In many cases this should simply be atomically setting a boolean to true and little more.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_external_soft_stop_requested](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a12b6641eae79cfca381c5a4adc4cb2aa)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)|Inform the adapter that a soft stop has been requested.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_get_labels](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1ab3f0ff7c642bb703f228782a4e6aae93)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)|Get the current string map for the labels.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_set_labels](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a80e5b040228f255c243f45317f04eeac)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Set the current string map for the labels.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_get_additional_info](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a91c2c856bafd227bce07837642dba164)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)|Get the current string map for the additional info.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_set_additional_info](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a059a56aff73e1901768e2511f664f9f8)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Set the current string map for the additional info.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_get_annotations](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a5735f0da0826fe530291423a214ce456)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)|Get the current string map for the annotations.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_set_annotations](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a6f5ea8bf77b08b81ffefe160157a4ca0)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Set the current string map for the annotations.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_stats_base](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a71b7533f99e051e4a6969d28521e1e88)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStatsBase](/game-host-adapter/structrallyherestatsbase/#structRallyHereStatsBase) * stats, const [RallyHereStatsBaseProvided](/game-host-adapter/structrallyherestatsbaseprovided/#structRallyHereStatsBaseProvided) * provided, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)|Set the base stats for the game instance.|
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_stats_gauge](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a4c6cb8ff5eb5fc79e352e64fea634582)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, double value, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_stats_gauge_with_labels](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1acdb70d34626a0951b5123ca5a8c90ba7)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, double value, const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) labels, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_stats_arbitrary](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a9b1b98ccaabfe4412ab311c4e4bf12df)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, const char * data, unsigned int data_size, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_stats_arbitrary_move](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a8a05228013b9ad1fcdb0dbeea74d2d10)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, char * data, unsigned int data_size, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, char *data, void *user_data) callback, void * user_data)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_metrics_immediate_action](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1aaa958e15c14a2100748048adb0cf7277)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereMetricDefinition](/game-host-adapter/structrallyheremetricdefinition/#structRallyHereMetricDefinition) * definition, const [RallyHereMetricAction](/game-host-adapter/structrallyheremetricaction/#structRallyHereMetricAction) * action, [RallyHereMetricFlush](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa) flush)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784)|[rallyhere_metrics_immediate_action_with_time](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a28bcafe6ae1c9ebf393fb93f80253f73)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereMetricDefinition](/game-host-adapter/structrallyheremetricdefinition/#structRallyHereMetricDefinition) * definition, const [RallyHereMetricAction](/game-host-adapter/structrallyheremetricaction/#structRallyHereMetricAction) * action, double timestamp, [RallyHereMetricFlush](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa) flush)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_metrics_statsd_output](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a109f917f5f91e81768a91e794125dc0d)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char ** data, size_t * data_size)||
|[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void|[rallyhere_metrics_graphite_output](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a221c9aa3189025c9ed28f73f812840f6)([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char ** data, size_t * data_size)||
## Logging

Logging levels that will passed to the optional log callback function 


### `rallyhere_set_log_callback` <a id="structRallyHereGameInstanceAdapter_1ad1d09e6062ad3db182a66f1829c7479b"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_set_log_callback([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereLogCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|[RallyHereLogCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a64a7679e581842654beca7ae63664a26)|callback|
|void *|user_data|

#### Description



Sets a custom logger that can handle logs from inside of the server. 




## Lifetime

Manages the lifetime of a Rally Here Game Instance Adapter. 


### `rallyhere_create_game_instance_adapter` <a id="structRallyHereGameInstanceAdapter_1a6122df1cede4159d0d30be0eb7b45463"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_create_game_instance_adapter([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) *|adapter|
|const char *|arguments|

#### Description



Create a new adapter using a single string to represent the command line arguments it will use for initialization. 



### `rallyhere_create_game_instance_adaptern` <a id="structRallyHereGameInstanceAdapter_1aeef6eea3f33281f1a997fcb3462effd3"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_create_game_instance_adaptern([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, unsigned int arguments_length)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) *|adapter|
|const char *|arguments|
|unsigned int|arguments_length|

#### Description



Create a new adapter using a C-style array to represent the command line arguments it will use for initialization. 



### `rallyhere_create_game_instance_adapter_with_logger` <a id="structRallyHereGameInstanceAdapter_1a49786a749d31ba4c426b4b0059709704"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_create_game_instance_adapter_with_logger([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, [RallyHereLogCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) *|adapter|
|const char *|arguments|
|[RallyHereLogCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a64a7679e581842654beca7ae63664a26)|callback|
|void *|user_data|

#### Description



Create a new adapter using a single string to represent the command line arguments it will use for initialization and a custom logging function to be used from the moment it is created. 

#### See also

[rallyhere_set_log_callback](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1ad1d09e6062ad3db182a66f1829c7479b)


### `rallyhere_create_game_instance_adaptern_with_logger` <a id="structRallyHereGameInstanceAdapter_1a85db0b2f34bb7f67347ff9bbe5ff2beb"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_create_game_instance_adaptern_with_logger([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) * adapter, const char * arguments, unsigned int arguments_length, [RallyHereLogCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a64a7679e581842654beca7ae63664a26) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) *|adapter|
|const char *|arguments|
|unsigned int|arguments_length|
|[RallyHereLogCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a64a7679e581842654beca7ae63664a26)|callback|
|void *|user_data|

#### Description



Create a new adapter using a C-style array to represent the command line arguments it will use for initialization and a custom logging function to be used from the moment it is created. 

#### See also

[rallyhere_set_log_callback](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1ad1d09e6062ad3db182a66f1829c7479b)


### `rallyhere_destroy_game_instance_adapter` <a id="structRallyHereGameInstanceAdapter_1a291d4acc324c8dbab8786391fb5557ec"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_destroy_game_instance_adapter([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|

#### Description

Destroys the adapter.

Before any memory is freed every registered callback is called with the status RH_STATUS_CANCELLED so that any memory associated with the callback's user data can be freed. 




## Repeated at Regular Intervals



### `rallyhere_tick` <a id="structRallyHereGameInstanceAdapter_1a4db3f8b5b27ab240d0edb337719dd2fd"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_tick([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|

#### Description

Tick that's expected to be called every frame.

This is used to check for any messages from the game host. All deferred message processing and callbacks will be handled during this tick. 

#### Returns
RH_STATUS_OK if this adapter can still be used, otherwise an error code. 



### `rallyhere_healthy` <a id="structRallyHereGameInstanceAdapter_1af3d04dce0166b8f1ba22de46ef74da9f"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_healthy([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|

#### Description

Mark the adapter as healthy.

This is used to tell the game host that this adapter is still alive and well. Should be called at a regular interval. Every 5 seconds is a good rule of thumb. Currently only used by Agones. SIC relies on an open metrics endpoint created by this SDK. i3D relies on the Arcus socket connection. Multiplay reliies on the A2S socket connection. 




## State request and callbacks



### `rallyhere_connect` <a id="structRallyHereGameInstanceAdapter_1a4a665dd0eeb67b49043e7a54e0d397ed"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_connect([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Begin the process of connecting to the appropriate game host.

The provided callback will be called when the process completes with the result of the connection attempt. 



### `rallyhere_ready` <a id="structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_ready([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Tell the game host that this GameInstance can now be allocated.

The provided callback will be called when the process completes with the result of the game host being told this is ready. 



### `rallyhere_on_allocated_callback` <a id="structRallyHereGameInstanceAdapter_1a3c96a6e133beb24d2f54af05b13a713c"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_on_allocated_callback([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereOnAllocatedCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a8664a168347b6152d05be2870fff5113) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|[RallyHereOnAllocatedCallback](/game-host-adapter/c__api_8h/#c__api_8h_1a8664a168347b6152d05be2870fff5113)|callback|
|void *|user_data|

#### Description



Register the callback which will be called when the game host has assigned an allocation to this adapter. 



### `rallyhere_test_fake_allocation_response` <a id="structRallyHereGameInstanceAdapter_1adddb6d6ebd068feef7972c6188ebecfe"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_test_fake_allocation_response([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * fake_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char *|fake_data|

#### Description



Fake an allocation. This should only be used for testing purposes. 



### `rallyhere_allocate` <a id="structRallyHereGameInstanceAdapter_1afa437234d06d99d50e63479ae8ca3bd4"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_allocate([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Tell the game host that this GameInstance has been allocated.

When using 
[rallyhere_ready()](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)this is used to tell the Game Host that you have recieved the allocation information and have done the appropriate work. In the[rallyhere_reserve()](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19)case this is telling the Game Host that you are allocated by some other means. The provided callback will be called when the process completes with the result


### `rallyhere_get_public_host_and_port` <a id="structRallyHereGameInstanceAdapter_1a31b26a826279e5dbffc672c887f114a7"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_get_public_host_and_port([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|[RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) *|map|

#### Description

Get any configured public host and port information.

There are situations where you want to use the Reserve flow, but would like to reuse the operational paramaters provided to SIC by RallyHere operations. This allows you to get a string map with the public_host and public_port that were provided to the game host. This is currently only valid in SIC mode.
Caller is responsible for freeing the string map with rallyhere_string_map_destroy. 



### `rallyhere_reserve` <a id="structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_reserve([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, unsigned int timeout_seconds, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) reserve_callback, void * reserve_user_data, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) ready_callback, void * ready_user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|unsigned int|timeout_seconds|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|reserve_callback|
|void *|reserve_user_data|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|ready_callback|
|void *|ready_user_data|

#### Description

Tell the game host to reserve this game instance rather than marking it ready.

This will stop the game host from destroying this game instance based on any ready timeouts. This is to be used when the game instance is manually handling sessions in a way that isn't expected to be handled by the normal matchmaking route. The provided callback will be called when the process completes with the result 



### `rallyhere_reserve_unconditional` <a id="structRallyHereGameInstanceAdapter_1afb679e1014b59a9a246c7c5b02c1062b"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_reserve_unconditional([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Tell the game host to reserve this game instance rather than marking it ready.

This will stop the game host from destroying this game instance based on any ready timeouts. This is to be used when the game instance is manually handling sessions in a way that isn't expected to be handled by the normal matchmaking route. The provided callback will be called when the process completes with the result 




## Soft Stop

There are times when the game hosting system would like your instance to shut down, but it does not need to happen immediately. In these situations a soft-stop will be requested so that your instance can finish any currently running games and then stop gracefully. Currently only used by SIC and i3D. 


### `rallyhere_on_soft_stop_callback` <a id="structRallyHereGameInstanceAdapter_1a662073848a735b58f79f18df20196cf1"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_on_soft_stop_callback([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

The callback to trigger when the game host wants to stop the game instance. This callback should be implmeneted as if it were being called from a signal handler. It should do only the minimum amount of work necessary to inform the rest of the system that a soft stop has been requested. In many cases this should simply be atomically setting a boolean to true and little more.

In SIC this is expected to come from the SIGTERM handler. In i3D this is expected to come from the Arcus commands 



### `rallyhere_external_soft_stop_requested` <a id="structRallyHereGameInstanceAdapter_1a12b6641eae79cfca381c5a4adc4cb2aa"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_external_soft_stop_requested([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|

#### Description

Inform the adapter that a soft stop has been requested.

There are situations where soft stop comes from something external to the adapter and the adapter needs to be told. In this case the adapter will also trigger the on soft stop callback. This is not a deferred call, so the registered callback will be called immediately.
In SIC the SIGTERM handler can't always be overridden by this adapter. In those situations the caller is expected to call this function as part of their SIGTERM handling. 




## Stats Labels

This is a map of string to string that can be used to store labels for stats. In SIC mode this will be exported as labels on every single prometheus metric. These must be set before calling 
[rallyhere_ready()](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)or[rallyhere_reserve()](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19)is called.

### `rallyhere_get_labels` <a id="structRallyHereGameInstanceAdapter_1ab3f0ff7c642bb703f228782a4e6aae93"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_get_labels([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|[RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) *|map|

#### Description

Get the current string map for the labels.

Caller is responsible for freeing the string map with rallyhere_string_map_destroy. 



### `rallyhere_set_labels` <a id="structRallyHereGameInstanceAdapter_1a80e5b040228f255c243f45317f04eeac"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_set_labels([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|map|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Set the current string map for the labels.

The string map will be copied. Provide a callback to be notified when the labels have been set. These must be set before calling 
[rallyhere_ready()](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)or[rallyhere_reserve()](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19)is called.



## Additional Info

This is a map of string to string that can be used to store additional information about the game instance. In SIC mode this will be exported as labels on the "instance_info" gauge. These can be set after calls to 
[rallyhere_ready()](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)or[rallyhere_reserve()](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19).

### `rallyhere_get_additional_info` <a id="structRallyHereGameInstanceAdapter_1a91c2c856bafd227bce07837642dba164"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_get_additional_info([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|[RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) *|map|

#### Description

Get the current string map for the additional info.

Caller is responsible for freeing the string map with rallyhere_string_map_destroy. 



### `rallyhere_set_additional_info` <a id="structRallyHereGameInstanceAdapter_1a059a56aff73e1901768e2511f664f9f8"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_set_additional_info([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|map|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Set the current string map for the additional info.

The string map will be copied. Provide a callback to be notified when the labels have been set. These can be set after calls to 
[rallyhere_ready()](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1a5844a4e3c29f530db2d61f647e925f88)or[rallyhere_reserve()](/game-host-adapter/structrallyheregameinstanceadapter/#structRallyHereGameInstanceAdapter_1ab7b0b24e192ce285af9c233e68d12c19).



## Stats Annotations



### `rallyhere_get_annotations` <a id="structRallyHereGameInstanceAdapter_1a5735f0da0826fe530291423a214ce456"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_get_annotations([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) * map)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|[RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) *|map|

#### Description

Get the current string map for the annotations.

Caller is responsible for freeing the string map. 



### `rallyhere_set_annotations` <a id="structRallyHereGameInstanceAdapter_1a6f5ea8bf77b08b81ffefe160157a4ca0"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_set_annotations([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) map, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|map|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Set the current string map for the annotations.

The string map will be copied. Provide a callback to be notified when the labels have been set. 




## Stats core



### `rallyhere_stats_base` <a id="structRallyHereGameInstanceAdapter_1a71b7533f99e051e4a6969d28521e1e88"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_stats_base([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereStatsBase](/game-host-adapter/structrallyherestatsbase/#structRallyHereStatsBase) * stats, const [RallyHereStatsBaseProvided](/game-host-adapter/structrallyherestatsbaseprovided/#structRallyHereStatsBaseProvided) * provided, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereStatsBase](/game-host-adapter/structrallyherestatsbase/#structRallyHereStatsBase) *|stats|
|const [RallyHereStatsBaseProvided](/game-host-adapter/structrallyherestatsbaseprovided/#structRallyHereStatsBaseProvided) *|provided|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description

Set the base stats for the game instance.

Provide a callback to be notified when the stats have been set These will be exported as labels on the "instance_info" gauge. These will be exported as part of the A2S_INFO query. 




## Stats gauges



### `rallyhere_stats_gauge` <a id="structRallyHereGameInstanceAdapter_1a4c6cb8ff5eb5fc79e352e64fea634582"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_stats_gauge([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, double value, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char *|name|
|double|value|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description



Set the value of a gauge. Provide a callback to be notified when the stats have been set. These stats will be exported with the current labels 



### `rallyhere_stats_gauge_with_labels` <a id="structRallyHereGameInstanceAdapter_1acdb70d34626a0951b5123ca5a8c90ba7"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_stats_gauge_with_labels([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, double value, const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360) labels, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char *|name|
|double|value|
|const [RallyHereStringMapPtr](/game-host-adapter/c__api_8h/#c__api_8h_1a8901377cd48d4831c6380593a2a60360)|labels|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description



Set the value of a gauge. Provide a callback to be notified when the stats have been set. 
#### Parameters

labels
: The labels to export with the gauge 




## Stats arbitrary



### `rallyhere_stats_arbitrary` <a id="structRallyHereGameInstanceAdapter_1a9b1b98ccaabfe4412ab311c4e4bf12df"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_stats_arbitrary([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, const char * data, unsigned int data_size, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char *|name|
|const char *|data|
|unsigned int|data_size|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, void *user_data)|callback|
|void *|user_data|

#### Description



Set the value of an arbitrary stat. Provide a callback to be notified when the stats have been set. 
#### Parameters

data
: The data to export with the stat which is copied by the call 



### `rallyhere_stats_arbitrary_move` <a id="structRallyHereGameInstanceAdapter_1a8a05228013b9ad1fcdb0dbeea74d2d10"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_stats_arbitrary_move([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char * name, char * data, unsigned int data_size, void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, char *data, void *user_data) callback, void * user_data)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char *|name|
|char *|data|
|unsigned int|data_size|
|void(*)(const [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) &code, char *data, void *user_data)|callback|
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


### `rallyhere_metrics_immediate_action` <a id="structRallyHereGameInstanceAdapter_1aaa958e15c14a2100748048adb0cf7277"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_metrics_immediate_action([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereMetricDefinition](/game-host-adapter/structrallyheremetricdefinition/#structRallyHereMetricDefinition) * definition, const [RallyHereMetricAction](/game-host-adapter/structrallyheremetricaction/#structRallyHereMetricAction) * action, [RallyHereMetricFlush](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa) flush)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereMetricDefinition](/game-host-adapter/structrallyheremetricdefinition/#structRallyHereMetricDefinition) *|definition|
|const [RallyHereMetricAction](/game-host-adapter/structrallyheremetricaction/#structRallyHereMetricAction) *|action|
|[RallyHereMetricFlush](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa)|flush|

#### Description






### `rallyhere_metrics_immediate_action_with_time` <a id="structRallyHereGameInstanceAdapter_1a28bcafe6ae1c9ebf393fb93f80253f73"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) [RallyHereStatusCode](/game-host-adapter/c__status_8h/#c__status_8h_1a95ac5c56776303edbbc5c3f125916784) rallyhere_metrics_immediate_action_with_time([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const [RallyHereMetricDefinition](/game-host-adapter/structrallyheremetricdefinition/#structRallyHereMetricDefinition) * definition, const [RallyHereMetricAction](/game-host-adapter/structrallyheremetricaction/#structRallyHereMetricAction) * action, double timestamp, [RallyHereMetricFlush](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa) flush)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const [RallyHereMetricDefinition](/game-host-adapter/structrallyheremetricdefinition/#structRallyHereMetricDefinition) *|definition|
|const [RallyHereMetricAction](/game-host-adapter/structrallyheremetricaction/#structRallyHereMetricAction) *|action|
|double|timestamp|
|[RallyHereMetricFlush](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa)|flush|

#### Description






### `rallyhere_metrics_statsd_output` <a id="structRallyHereGameInstanceAdapter_1a109f917f5f91e81768a91e794125dc0d"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_metrics_statsd_output([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char ** data, size_t * data_size)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char **|data|
|size_t *|data_size|

#### Description






### `rallyhere_metrics_graphite_output` <a id="structRallyHereGameInstanceAdapter_1a221c9aa3189025c9ed28f73f812840f6"></a>

[RH_EXPORT](/game-host-adapter/c__platform_8h/#c__platform_8h_1ab0f7a4ccdb6515b62edbb26fd4cd0808) void rallyhere_metrics_graphite_output([RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b) adapter, const char ** data, size_t * data_size)

#### Parameters

| Type | Name |
|------|------|
|[RallyHereGameInstanceAdapterPtr](/game-host-adapter/c__api_8h/#c__api_8h_1ab9f295af86f1286c21e57d4a73c76f8b)|adapter|
|const char **|data|
|size_t *|data_size|

#### Description







