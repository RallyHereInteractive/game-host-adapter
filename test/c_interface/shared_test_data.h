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
#pragma once

#include "c_api.h"
#include "rh_string.h"
#include "lest/lest.hpp"

struct TestCCodeData
{
    lest::env& lest_env;
    RallyHereGameInstanceAdapterPtr adapter;
    bool connect_called;
    RallyHereStatusCode connect_result;
    bool reserve_called;
    RallyHereStatusCode reserve_result;
    bool set_labels_called;
    RallyHereStatusCode set_labels_result;
    bool ready_called;
    RallyHereStatusCode ready_result;
    bool allocated_called;
    RallyHereStatusCode allocated_result;
    bool allocate_called;
    RallyHereStatusCode allocate_result;
    size_t soft_stop_called_count;
    RallyHereStatusCode soft_stop_result;
    rallyhere::string allocation_id;
    rallyhere::string session_id;
    rallyhere::string public_host;
    rallyhere::string public_port;
    bool set_additional_info_called;
    RallyHereStatusCode set_additional_info_result;
    bool set_base_stats_called;
    RallyHereStatusCode set_base_stats_result;
};

void on_connect_callback(const RallyHereStatusCode& code, void* user_data);
void on_second_connect_callback(const RallyHereStatusCode& code, void* user_data);
void on_reserve_callback(const RallyHereStatusCode& code, void* user_data);
void on_set_labels_callback(const RallyHereStatusCode& code, void* user_data);
void on_set_additional_info_callback(const RallyHereStatusCode& code, void* user_data);
void on_set_base_stats_callback(const RallyHereStatusCode& code, void* user_data);
void on_ready_callback(const RallyHereStatusCode& code, void* user_data);
void on_allocated_callback(RallyHereStringMapPtr allocation_info, const RallyHereStatusCode& code, void* user_data);
void on_allocated_multiples_callback(RallyHereStringMapPtr allocation_info, const RallyHereStatusCode& code, void* user_data);
void on_allocate_callback(const RallyHereStatusCode& code, void* user_data);
void on_soft_stop_callback(const RallyHereStatusCode& code, void* user_data);
