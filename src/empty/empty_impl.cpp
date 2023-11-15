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
#include "c_api.h"
#include <unordered_map>

void rallyhere_allocator_set_alloc(void* (*callback)(unsigned int size))
{
}
void rallyhere_allocator_set_free(void (*callback)(void*))
{
}
void rallyhere_allocator_set_realloc(void* (*callback)(void*, unsigned int size))
{
}

void rallyhere_set_log_callback(RallyHereGameInstanceAdapterPtr adapter, RallyHereLogCallback callback, void* user_data)
{
}
void rallyhere_set_default_log_level(RallyHereLogLevel level)
{
}

void rallyhere_global_init()
{
}

void rallyhere_global_cleanup()
{
}

RallyHereStatusCode rallyhere_create_game_instance_adapter(RallyHereGameInstanceAdapterPtr* adapter, const char* arguments)
{
    return RH_STATUS_ERROR;
}
RallyHereStatusCode
    rallyhere_create_game_instance_adaptern(RallyHereGameInstanceAdapterPtr* adapter, const char* arguments, unsigned int arguments_length)
{
    return RH_STATUS_ERROR;
}
RallyHereStatusCode rallyhere_create_game_instance_adapter_with_logger(RallyHereGameInstanceAdapterPtr* adapter,
                                                                       const char* arguments,
                                                                       RallyHereLogCallback callback,
                                                                       void* user_data)
{
    return RH_STATUS_ERROR;
}
RallyHereStatusCode rallyhere_create_game_instance_adaptern_with_logger(RallyHereGameInstanceAdapterPtr* adapter,
                                                                        const char* arguments,
                                                                        unsigned int arguments_length,
                                                                        RallyHereLogCallback callback,
                                                                        void* user_data)
{
    return RH_STATUS_ERROR;
}
void rallyhere_destroy_game_instance_adapter(RallyHereGameInstanceAdapterPtr adapter)
{
}
RallyHereStatusCode rallyhere_tick(RallyHereGameInstanceAdapterPtr adapter)
{
    return RH_STATUS_ERROR;
}
RallyHereStatusCode rallyhere_healthy(RallyHereGameInstanceAdapterPtr adapter)
{
    return RH_STATUS_ERROR;
}

void rallyhere_connect(RallyHereGameInstanceAdapterPtr adapter, void (*callback)(const RallyHereStatusCode& code, void* user_data), void* user_data)
{
}

void rallyhere_ready(RallyHereGameInstanceAdapterPtr adapter, void (*callback)(const RallyHereStatusCode& code, void* user_data), void* user_data)
{
}

void rallyhere_on_allocated_callback(RallyHereGameInstanceAdapterPtr adapter, RallyHereOnAllocatedCallback callback, void* user_data)
{
}

void rallyhere_test_fake_allocation_response(RallyHereGameInstanceAdapterPtr adapter, const char* fake_data)
{
}

void rallyhere_allocate(RallyHereGameInstanceAdapterPtr adapter, void (*callback)(const RallyHereStatusCode& code, void* user_data), void* user_data)
{
}

void rallyhere_reserve(RallyHereGameInstanceAdapterPtr adapter,
                       unsigned int timeout_seconds,
                       void (*reserve_callback)(const RallyHereStatusCode& code, void* user_data),
                       void* reserve_user_data,
                       void (*ready_callback)(const RallyHereStatusCode& code, void* user_data),
                       void* ready_user_data)
{
}

void rallyhere_reserve_unconditional(RallyHereGameInstanceAdapterPtr adapter,
                                     void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                     void* user_data)
{
}

void rallyhere_on_soft_stop_callback(RallyHereGameInstanceAdapterPtr adapter,
                                     void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                     void* user_data)
{
}
void rallyhere_external_soft_stop_requested(RallyHereGameInstanceAdapterPtr adapter)
{
}

RallyHereStatusCode rallyhere_string_map_create(RallyHereStringMapPtr* map)
{
    return RH_STATUS_ERROR;
}
void rallyhere_string_map_destroy(RallyHereStringMapPtr map)
{
}
RallyHereStatusCode rallyhere_string_map_copy(RallyHereStringMapPtr dest, const RallyHereStringMapPtr src)
{
    return RH_STATUS_ERROR;
}
void rallyhere_string_map_clear(RallyHereStringMapPtr map)
{
}
RallyHereStatusCode rallyhere_string_map_get(RallyHereStringMapPtr map, const char* key, const char** value, unsigned int* value_size)
{
    return RH_STATUS_ERROR;
}
RallyHereStatusCode rallyhere_string_map_set(RallyHereStringMapPtr map, const char* key, const char* value)
{
    return RH_STATUS_ERROR;
}
RallyHereStatusCode rallyhere_string_map_setn(RallyHereStringMapPtr map, const char* key, const char* value, unsigned int value_size)
{
    return RH_STATUS_ERROR;
}

void rallyhere_get_labels(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
{
}
void rallyhere_set_labels(RallyHereGameInstanceAdapterPtr adapter,
                          const RallyHereStringMapPtr map,
                          void (*callback)(const RallyHereStatusCode& code, void* user_data),
                          void* user_data)
{
}

void rallyhere_get_additional_info(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
{
}
void rallyhere_set_additional_info(RallyHereGameInstanceAdapterPtr adapter,
                                   const RallyHereStringMapPtr map,
                                   void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                   void* user_data)
{
}

void rallyhere_get_annotations(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
{
}
void rallyhere_set_annotations(RallyHereGameInstanceAdapterPtr adapter,
                               const RallyHereStringMapPtr map,
                               void (*callback)(const RallyHereStatusCode& code, void* user_data),
                               void* user_data)
{
}

RallyHereStatusCode rallyhere_stats_base(RallyHereGameInstanceAdapterPtr adapter,
                                         const RallyHereStatsBase* stats,
                                         const RallyHereStatsBaseProvided* provided,
                                         void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                         void* user_data)
{
    return RH_STATUS_ERROR;
}

void rallyhere_stats_gauge(RallyHereGameInstanceAdapterPtr adapter,
                           const char* name,
                           double value,
                           void (*callback)(const RallyHereStatusCode& code, void* user_data),
                           void* user_data)
{
}
void rallyhere_stats_gauge_with_labels(RallyHereGameInstanceAdapterPtr adapter,
                                       const char* name,
                                       double value,
                                       const RallyHereStringMapPtr labels,
                                       void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                       void* user_data)
{
}

void rallyhere_stats_arbitrary(RallyHereGameInstanceAdapterPtr adapter,
                               const char* name,
                               const char* data,
                               unsigned int data_size,
                               void (*callback)(const RallyHereStatusCode& code, void* user_data),
                               void* user_data)
{
}
void rallyhere_stats_arbitrary_move(RallyHereGameInstanceAdapterPtr adapter,
                                    const char* name,
                                    char* data,
                                    unsigned int data_size,
                                    void (*callback)(const RallyHereStatusCode& code, char* data, void* user_data),
                                    void* user_data)
{
}

RallyHereStatusCode rallyhere_metrics_immediate_action(RallyHereGameInstanceAdapterPtr adapter,
                                                       const RallyHereMetricDefinition* definition,
                                                       const RallyHereMetricAction* action,
                                                       RallyHereMetricFlush flush)
{
    return RH_STATUS_ERROR;
}
RallyHereStatusCode rallyhere_metrics_immediate_action_with_time(RallyHereGameInstanceAdapterPtr adapter,
                                                                 const RallyHereMetricDefinition* definition,
                                                                 const RallyHereMetricAction* action,
                                                                 double timestamp,
                                                                 RallyHereMetricFlush flush)
{
    return RH_STATUS_ERROR;
}
void rallyhere_metrics_statsd_output(RallyHereGameInstanceAdapterPtr adapter, const char** data, size_t* data_size)
{
}
void rallyhere_metrics_graphite_output(RallyHereGameInstanceAdapterPtr adapter, const char** data, size_t* data_size)
{
}

bool rallyhere_is_error(RallyHereStatusCode err)
{
    return err != RH_STATUS_OK;
}

bool rallyhere_is_cancelled(RallyHereStatusCode err)
{
    return err == RH_STATUS_CANCELLED;
}

#define RH_SYMBOL_STRING_PAIR(e) e, #e

const char* rallyhere_status_text(RallyHereStatusCode err)
{
    static std::unordered_map<RallyHereStatusCode, const char*> lookup = {
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_OK) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_ERROR) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_NO_BOOTSTRAP_MODE_PROVIDED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_CREDENTIALS_FILE_NOT_TWO_LINES) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_NO_RALLYHERE_URL_PROVIDED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_NO_SIC_PROFILE_ID_PROVIDED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_CANCELLED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_VALIDATION_ERROR_ADAPTER_IS_NULLPTR) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_ADAPTER_ALLOCATION_ERROR) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_VALIDATION_ERROR_MAP_IS_NULLPTR) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_MAP_ALLOCATION_ERROR) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_VALIDATION_ERROR_KEY_IS_NULLPTR) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_VALIDATION_ERROR_VALUE_IS_NULLPTR) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_VALIDATION_ERROR_VALUE_SIZE_IS_NULLPTR) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_KEY_NOT_FOUND) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_COULD_NOT_PARSE_RESPONSE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_RESPONSE_NO_ACCESS_TOKEN) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_RESPONSE_NO_REFRESH_TOKEN) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_RESPONSE_NO_AUTH_SUCCESS) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_RESPONSE_AUTH_SUCCESS_NOT_BOOL) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_POLL_INTERVAL_MUST_BE_NUMERIC) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_CANNOT_POLL_WITHOUT_ON_ALLOCATED_CALLBACK) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_PROMETHEUS_PORT_MUST_BE_NUMERIC) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_PROMETHEUS_ALREADY_STARTED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_PROMETHEUS_COULD_NOT_START) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_REQUEST_TIMEOUT_MUST_BE_NUMERIC) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_POLL_UNKNOWN_STATE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_CONTINUE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_SWITCHINGPROTOCOLS) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_PROCESSING) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_OK) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_CREATED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_ACCEPTED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_NON_AUTHORITATIVE_INFORMATION) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_NO_CONTENT) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_RESET_CONTENT) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_PARTIAL_CONTENT) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_MULTI_STATUS) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_ALREADY_REPORTED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_IM_USED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_MULTIPLE_CHOICES) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_MOVED_PERMANENTLY) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_FOUND) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_SEE_OTHER) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_NOT_MODIFIED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_USE_PROXY) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_TEMPORARY_REDIRECT) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_PERMANENT_REDIRECT) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_BAD_REQUEST) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_UNAUTHORIZED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_PAYMENT_REQUIRED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_FORBIDDEN) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_NOT_FOUND) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_METHOD_NOT_ALLOWED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_NOT_ACCEPTABLE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_PROXY_AUTHENTICATION_REQUIRED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_REQUEST_TIMEOUT) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_CONFLICT) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_GONE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_LENGTH_REQUIRED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_PRECONDITION_FAILED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_PAYLOAD_TOO_LARGE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_URI_TOO_LONG) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_UNSUPPORTED_MEDIA_TYPE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_RANGE_NOT_SATISFIABLE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_EXPECTATION_FAILED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_MISDIRECTED_REQUEST) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_UNPROCESSABLE_ENTITY) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_LOCKED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_FAILED_DEPENDENCY) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_UPGRADE_REQUIRED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_PRECONDITION_REQUIRED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_TOO_MANY_REQUESTS) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_CONNECTION_CLOSED_WITHOUT_RESPONSE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_UNAVAILABLE_FOR_LEGAL_REASONS) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_CLIENT_CLOSED_REQUEST) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_INTERNAL_SERVER_ERROR) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_NOT_IMPLEMENTED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_BAD_GATEWAY) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_SERVICE_UNAVAILABLE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_GATEWAY_TIMEOUT) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_HTTP_VERSION_NOT_SUPPORTED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_VARIANT_ALSO_NEGOTIATES) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_INSUFFICIENT_STORAGE) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_LOOP_DETECTED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_NOT_EXTENDED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_NETWORK_AUTHENTICATION_REQUIRED) },
        { RH_SYMBOL_STRING_PAIR(RH_STATUS_HTTP_NETWORK_CONNECT_TIMEOUT_ERROR) },
    };

    auto it = lookup.find(err);
    if (it == lookup.end())
    {
        return "";
    }
    return it->second;
}