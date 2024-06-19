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
#include "shared_test_data.h"

void on_connect_callback(const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCCodeData*>(user_data);
    auto&& lest_env = data->lest_env;
    data->connect_result = code;
    EXPECT(data->connect_called == false);
    data->connect_called = true;
}

void on_reserve_callback(const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCCodeData*>(user_data);
    auto&& lest_env = data->lest_env;
    EXPECT(data->reserve_called == false);
    data->reserve_result = code;
    data->reserve_called = true;
}

void on_set_labels_callback(const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCCodeData*>(user_data);
    auto&& lest_env = data->lest_env;
    if (data->set_labels_called)
        int i = 0;
    EXPECT(data->set_labels_called == false);
    data->set_labels_result = code;
    data->set_labels_called = true;
}

void on_set_additional_info_callback(const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCCodeData*>(user_data);
    data->set_additional_info_result = code;
    data->set_additional_info_called = true;
}

void on_set_base_stats_callback(const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCCodeData*>(user_data);
    data->set_base_stats_result = code;
    data->set_base_stats_called = true;
}

void on_ready_callback(const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCCodeData*>(user_data);
    auto&& lest_env = data->lest_env;
    if (!rallyhere_is_cancelled(code))
        EXPECT(data->ready_called == false);
    data->ready_result = code;
    data->ready_called = true;
}

void on_allocated_callback(RallyHereStringMapPtr allocation_info, const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCCodeData*>(user_data);
    auto&& lest_env = data->lest_env;
    if (!rallyhere_is_cancelled(code))
        EXPECT(data->allocated_called == false);
    data->allocated_result = code;
    data->allocated_called = true;
    if (allocation_info)
    {
        const char* value_to_check = nullptr;
        unsigned int value_size = 0;
        auto result = rallyhere_string_map_get(allocation_info, "allocation_id", &value_to_check, &value_size);
        if (RH_STATUS_OK == result)
            data->allocation_id = { value_to_check, value_size };
        result = rallyhere_string_map_get(allocation_info, "session_id", &value_to_check, &value_size);
        if (RH_STATUS_OK == result)
            data->session_id = { value_to_check, value_size };
        result = rallyhere_string_map_get(allocation_info, "public_host", &value_to_check, &value_size);
        if (RH_STATUS_OK == result)
            data->public_host = { value_to_check, value_size };
        result = rallyhere_string_map_get(allocation_info, "public_port", &value_to_check, &value_size);
        if (RH_STATUS_OK == result)
            data->public_port = { value_to_check, value_size };
    }
    rallyhere_string_map_destroy(allocation_info);
}

void on_allocated_multiples_callback(RallyHereStringMapPtr allocation_info, const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCCodeData*>(user_data);
    data->allocated_result = code;
    data->allocated_called = true;
    std::cerr << "on_allocated_multiples_callback " << data->allocated_result << std::endl;
    if (allocation_info)
    {
        const char* value_to_check = nullptr;
        unsigned int value_size = 0;
        auto result = rallyhere_string_map_get(allocation_info, "allocation_id", &value_to_check, &value_size);
        if (RH_STATUS_OK == result)
            data->allocation_id = { value_to_check, value_size };
        result = rallyhere_string_map_get(allocation_info, "session_id", &value_to_check, &value_size);
        if (RH_STATUS_OK == result)
            data->session_id = { value_to_check, value_size };
        result = rallyhere_string_map_get(allocation_info, "public_host", &value_to_check, &value_size);
        if (RH_STATUS_OK == result)
            data->public_host = { value_to_check, value_size };
        result = rallyhere_string_map_get(allocation_info, "public_port", &value_to_check, &value_size);
        if (RH_STATUS_OK == result)
            data->public_port = { value_to_check, value_size };
    }
    rallyhere_string_map_destroy(allocation_info);
}

void on_allocate_callback(const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCCodeData*>(user_data);
    auto&& lest_env = data->lest_env;
    EXPECT(data->allocate_called == false);
    data->allocate_result = code;
    data->allocate_called = true;
}

void on_soft_stop_callback(const RallyHereStatusCode& code, void* user_data)
{
    auto data = static_cast<TestCCodeData*>(user_data);
    data->soft_stop_result = code;
    data->soft_stop_called_count++;
}

void on_soft_stop_callback_v2(const RallyHereStatusCode& code, void* user_data, int timeout)
{
    auto data = static_cast<TestCCodeData*>(user_data);
    data->soft_stop_result = code;
    data->soft_stop_called_count++;
    data->soft_stop_timeout = timeout;
}