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
#include "shared_test_data.h"

#include "lest/lest.hpp"
#include "a2s.hpp"

void validate_server_info(lest::env& lest_env, const rallyhere::server_info& info);
rallyhere::server_info get_stats(lest::env& lest_env, RallyHereGameInstanceAdapterPtr adapter, TestCCodeData& data);
void get_stats_fail_challenge(lest::env& lest_env, RallyHereGameInstanceAdapterPtr adapter, TestCCodeData& data);
size_t get_stats_pending_challenges(lest::env& lest_env, RallyHereGameInstanceAdapterPtr adapter, TestCCodeData& data, int num_pending);
