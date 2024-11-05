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
#include "allocator.h"
#include "sdk.h"

namespace i3d
{
namespace one
{
    namespace
    {

        void allocator_set_alloc(void* (*callback)(unsigned int size))
        {
            // Wrapper for size_t.
            auto wrapper = [callback](size_t size) -> void* {
                return callback(static_cast<unsigned int>(size));
            };
            allocator::set_alloc(wrapper);
        }

        void allocator_set_free(void(callback)(void*))
        {
            allocator::set_free(callback);
        }

        void allocator_set_realloc(void* (*callback)(void*, unsigned int size))
        {
            // Wrapper for size_t.
            auto wrapper = [callback](void* p, size_t size) -> void* {
                return callback(p, static_cast<unsigned int>(size));
            };
            allocator::set_realloc(wrapper);
        }

    } // Unnamed namespace.
} // namespace one
} // namespace i3d

namespace one = i3d::one;

extern "C"
{
    void rallyhere_allocator_set_alloc(void* (*callback)(unsigned int size))
    {
        one::allocator_set_alloc(callback);
    }

    void rallyhere_allocator_set_free(void (*callback)(void*))
    {
        one::allocator_set_free(callback);
    }

    void rallyhere_allocator_set_realloc(void* (*callback)(void*, unsigned int size))
    {
        one::allocator_set_realloc(callback);
    }
}; // extern "C"

namespace rallyhere
{
namespace
{
    void global_init()
    {
    }
    void global_cleanup()
    {
    }

    RallyHereStatusCode create_game_instance_adapter(RallyHereGameInstanceAdapterPtr* adapter, const char* arguments)
    {
        if (nullptr == adapter)
            return RH_STATUS_VALIDATION_ERROR_ADAPTER_IS_NULLPTR;
        auto a = one::allocator::create<rallyhere::GameInstanceAdapter>(arguments);
        if (!a)
            return RH_STATUS_ADAPTER_ALLOCATION_ERROR;
        if (!a->good())
        {
            auto status = a->status();
            one::allocator::destroy(a);
            return status.code();
        }
        *adapter = reinterpret_cast<RallyHereGameInstanceAdapterPtr>(a);
        return RH_STATUS_OK;
    }

    RallyHereStatusCode create_game_instance_adaptern(RallyHereGameInstanceAdapterPtr* adapter, const char* arguments, unsigned int arguments_length)
    {
        if (nullptr == adapter)
            return RH_STATUS_VALIDATION_ERROR_ADAPTER_IS_NULLPTR;
        auto a = one::allocator::create<rallyhere::GameInstanceAdapter>(std::string_view{arguments, arguments_length});
        if (!a)
            return RH_STATUS_ADAPTER_ALLOCATION_ERROR;
        if (!a->good())
        {
            auto status = a->status();
            one::allocator::destroy(a);
            return status.code();
        }
        *adapter = reinterpret_cast<RallyHereGameInstanceAdapterPtr>(a);
        return RH_STATUS_OK;
    }

    RallyHereStatusCode create_game_instance_adapter_with_logger(RallyHereGameInstanceAdapterPtr* adapter, const char* arguments, RallyHereLogCallback callback, void* user_data)
    {
        if (nullptr == adapter)
            return RH_STATUS_VALIDATION_ERROR_ADAPTER_IS_NULLPTR;
        auto wrapper = [callback, user_data](RallyHereLogLevel level, std::string_view message) {
            callback(level, message.data(), message.size(), user_data);
        };
        auto a = one::allocator::create<rallyhere::GameInstanceAdapter>(arguments, wrapper);
        if (!a)
            return RH_STATUS_ADAPTER_ALLOCATION_ERROR;
        if (!a->good())
        {
            auto status = a->status();
            one::allocator::destroy(a);
            return status.code();
        }
        *adapter = reinterpret_cast<RallyHereGameInstanceAdapterPtr>(a);
        return RH_STATUS_OK;
    }

    RallyHereStatusCode create_game_instance_adaptern_with_logger(RallyHereGameInstanceAdapterPtr* adapter, const char* arguments, unsigned int arguments_length, RallyHereLogCallback callback, void* user_data)
    {
        if (nullptr == adapter)
            return RH_STATUS_VALIDATION_ERROR_ADAPTER_IS_NULLPTR;
        auto wrapper = [callback, user_data](RallyHereLogLevel level, std::string_view message) {
            callback(level, message.data(), message.size(), user_data);
        };
        auto a = one::allocator::create<rallyhere::GameInstanceAdapter>(std::string_view{arguments, arguments_length}, wrapper);
        if (!a)
            return RH_STATUS_ADAPTER_ALLOCATION_ERROR;
        if (!a->good())
        {
            auto status = a->status();
            one::allocator::destroy(a);
            return status.code();
        }
        *adapter = reinterpret_cast<RallyHereGameInstanceAdapterPtr>(a);
        return RH_STATUS_OK;
    }

    void destroy_game_instance_adapter(RallyHereGameInstanceAdapterPtr adapter)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        one::allocator::destroy(a);
    }

    RallyHereStatusCode tick(RallyHereGameInstanceAdapterPtr adapter)
    {
        if (nullptr == adapter)
            return RH_STATUS_VALIDATION_ERROR_ADAPTER_IS_NULLPTR;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        return a->Tick().code();
    }
    RallyHereStatusCode healthy(RallyHereGameInstanceAdapterPtr adapter)
    {
        if (nullptr == adapter)
            return RH_STATUS_VALIDATION_ERROR_ADAPTER_IS_NULLPTR;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        return a->Healthy().code();
    }
    void connect(RallyHereGameInstanceAdapterPtr adapter, void (*callback)(const RallyHereStatusCode& code, void* user_data), void* user_data)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->Connect(callback, user_data);
    }
    void ready(RallyHereGameInstanceAdapterPtr adapter, void (*callback)(const RallyHereStatusCode& code, void* user_data), void* user_data)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->Ready(callback, user_data);
    }
    void on_allocated_callback(RallyHereGameInstanceAdapterPtr adapter,
                               RallyHereOnAllocatedCallback callback,
                               void* user_data)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->OnAllocatedCallback(callback, user_data);
    }

    void test_fake_allocation_response(RallyHereGameInstanceAdapterPtr adapter, const char *fake_data)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->FakeAllocated(fake_data);
    }

    void allocate(RallyHereGameInstanceAdapterPtr adapter, void (*callback)(const RallyHereStatusCode& code, void* user_data), void* user_data)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->Allocate(callback, user_data);
    }

    void get_public_host_and_port(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
    {
        if (nullptr == adapter)
            return;
        if (nullptr == map)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        auto m = a->GetPublicHostAndPort();
        *map = reinterpret_cast<RallyHereStringMapPtr>(m);
    }

    void get_user_agent_string(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
    {
        if (nullptr == adapter)
            return;
        if (nullptr == map)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        auto m = a->GetUserAgent();
        *map = reinterpret_cast<RallyHereStringMapPtr>(m);
    }

    void reserve(RallyHereGameInstanceAdapterPtr adapter,
                 unsigned int timeout_seconds,
                 void (*reserve_callback)(const RallyHereStatusCode& code, void* user_data),
                 void* reserve_user_data,
                 void (*ready_callback)(const RallyHereStatusCode& code, void* user_data),
                 void* ready_user_data)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->Reserve(timeout_seconds, reserve_callback, reserve_user_data, ready_callback, ready_user_data);
    }

    void reserve_unconditional(RallyHereGameInstanceAdapterPtr adapter,
                               void (*callback)(const RallyHereStatusCode& code, void* user_data),
                               void* user_data)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->ReserveUnconditional(callback, user_data);
    }
    void on_soft_stop_callback(RallyHereGameInstanceAdapterPtr adapter,
                               void (*callback)(const RallyHereStatusCode& code, void* user_data),
                               void* user_data)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->OnSoftStopCallback(callback, user_data);
    }
    void external_soft_stop_requested(RallyHereGameInstanceAdapterPtr adapter)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->ExternalSoftStopRequested();
    }
    void set_log_callback(RallyHereGameInstanceAdapterPtr adapter,
                          RallyHereLogCallback callback,
                          void* user_data)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        auto wrapper = [callback, user_data](RallyHereLogLevel level, std::string_view message) {
            callback(level, message.data(), message.size(), user_data);
        };
        a->OnLogCallback(wrapper);
    }
    void set_default_log_level(RallyHereLogLevel level)
    {
        logger::SetDefaultLogLevel(level);
    }
    RallyHereStatusCode string_map_create(RallyHereStringMapPtr* map)
    {
        if (nullptr == map)
            return RH_STATUS_VALIDATION_ERROR_MAP_IS_NULLPTR;
        auto m = one::allocator::create<rallyhere::StringMap>();
        if (!m)
            return RH_STATUS_MAP_ALLOCATION_ERROR;
        *map = reinterpret_cast<RallyHereStringMapPtr>(m);
        return RH_STATUS_OK;
    }
    void string_map_destroy(RallyHereStringMapPtr map)
    {
        if (nullptr == map)
            return;
        auto m = reinterpret_cast<rallyhere::StringMap*>(map);
        one::allocator::destroy(m);
    }
    RallyHereStatusCode string_map_copy(RallyHereStringMapPtr dest, const RallyHereStringMapPtr src)
    {
        if (nullptr == dest)
            return RH_STATUS_VALIDATION_ERROR_MAP_IS_NULLPTR;
        if (nullptr == src)
            return RH_STATUS_VALIDATION_ERROR_MAP_IS_NULLPTR;
        auto d = reinterpret_cast<rallyhere::StringMap*>(dest);
        auto s = reinterpret_cast<rallyhere::StringMap*>(src);
        *d = *s;
        return RH_STATUS_OK;
    }
    void string_map_clear(RallyHereStringMapPtr map)
    {
        if (nullptr == map)
            return;
        auto m = reinterpret_cast<rallyhere::StringMap*>(map);
        m->Clear();
    }
    RallyHereStatusCode string_map_get(RallyHereStringMapPtr map, const char* key, const char** value, unsigned int* value_size)
    {
        if (nullptr == map)
            return RH_STATUS_VALIDATION_ERROR_MAP_IS_NULLPTR;
        if (nullptr == key)
            return RH_STATUS_VALIDATION_ERROR_KEY_IS_NULLPTR;
        if (nullptr == value)
            return RH_STATUS_VALIDATION_ERROR_VALUE_IS_NULLPTR;
        if (nullptr == value_size)
            return RH_STATUS_VALIDATION_ERROR_VALUE_SIZE_IS_NULLPTR;
        auto m = reinterpret_cast<rallyhere::StringMap*>(map);
        return m->Get(key, value, value_size).code();
    }
    RallyHereStatusCode string_map_set(RallyHereStringMapPtr map, const char* key, const char* value)
    {
        if (nullptr == map)
            return RH_STATUS_VALIDATION_ERROR_MAP_IS_NULLPTR;
        if (nullptr == key)
            return RH_STATUS_VALIDATION_ERROR_KEY_IS_NULLPTR;
        if (nullptr == value)
            return RH_STATUS_VALIDATION_ERROR_VALUE_IS_NULLPTR;
        auto m = reinterpret_cast<rallyhere::StringMap*>(map);
        return m->Set(key, value).code();
    }
    RallyHereStatusCode string_map_setn(RallyHereStringMapPtr map, const char* key, const char* value, unsigned int value_size)
    {
        if (nullptr == map)
            return RH_STATUS_VALIDATION_ERROR_MAP_IS_NULLPTR;
        if (nullptr == key)
            return RH_STATUS_VALIDATION_ERROR_KEY_IS_NULLPTR;
        if (nullptr == value)
            return RH_STATUS_VALIDATION_ERROR_VALUE_IS_NULLPTR;
        auto m = reinterpret_cast<rallyhere::StringMap*>(map);
        return m->Set(key, value, value_size).code();
    }
    void get_labels(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
    {
        if (nullptr == adapter)
            return;
        if (nullptr == map)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        auto m = a->GetLabels();
        *map = reinterpret_cast<RallyHereStringMapPtr>(&m);
    }
    void set_labels(RallyHereGameInstanceAdapterPtr adapter,
                    const RallyHereStringMapPtr map,
                    void (*callback)(const RallyHereStatusCode& code, void* user_data),
                    void* user_data)
    {
        if (nullptr == adapter)
            return;
        if (nullptr == map)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        auto m = reinterpret_cast<rallyhere::StringMap*>(map);
        a->SetLabels(*m, callback, user_data);
    }
    void get_additional_info(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
    {
        if (nullptr == adapter)
            return;
        if (nullptr == map)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        auto m = a->GetAdditionalInfoLabels();
        *map = reinterpret_cast<RallyHereStringMapPtr>(&m);
    }
    void set_additional_info(RallyHereGameInstanceAdapterPtr adapter,
                             const RallyHereStringMapPtr map,
                             void (*callback)(const RallyHereStatusCode& code, void* user_data),
                             void* user_data)
    {
        if (nullptr == adapter)
            return;
        if (nullptr == map)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        auto m = reinterpret_cast<rallyhere::StringMap*>(map);
        a->SetAdditionalInfoLabels(*m, callback, user_data);
    }
    void get_annotations(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
    {
        if (nullptr == adapter)
            return;
        if (nullptr == map)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        auto m = a->GetAnnotations();
        *map = reinterpret_cast<RallyHereStringMapPtr>(&m);
    }
    void set_annotations(RallyHereGameInstanceAdapterPtr adapter,
                         const RallyHereStringMapPtr map,
                         void (*callback)(const RallyHereStatusCode& code, void* user_data),
                         void* user_data)
    {
        if (nullptr == adapter)
            return;
        if (nullptr == map)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        auto m = reinterpret_cast<rallyhere::StringMap*>(map);
        a->SetAnnotations(*m, callback, user_data);
    }
    RallyHereStatusCode stats_base(RallyHereGameInstanceAdapterPtr adapter,
                                   const RallyHereStatsBase* stats,
                                   const RallyHereStatsBaseProvided* provided,
                                   void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                   void* user_data)
    {
        if (nullptr == adapter)
            return RH_STATUS_VALIDATION_ERROR_ADAPTER_IS_NULLPTR;
        if (nullptr == stats)
            return RH_STATUS_VALIDATION_ERROR_STATS_IS_NULLPTR;
        if (nullptr == provided)
            return RH_STATUS_VALIDATION_ERROR_STATS_PROVIDED_IS_NULLPTR;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        return a->StatsBase(stats, provided, callback, user_data).code();
    }
    void stats_gauge(RallyHereGameInstanceAdapterPtr adapter,
                     const char* name,
                     double value,
                     void (*callback)(const RallyHereStatusCode& code, void* user_data),
                     void* user_data)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->StatsGauge(name, value, callback, user_data);
    }
    void stats_gauge_with_labels(RallyHereGameInstanceAdapterPtr adapter,
                                 const char* name,
                                 double value,
                                 const RallyHereStringMapPtr labels,
                                 void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                 void* user_data)
    {
        if (nullptr == adapter)
            return;
        if (nullptr == labels)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        auto l = reinterpret_cast<rallyhere::StringMap*>(labels);
        a->StatsGaugeWithLabels(name, value, *l, callback, user_data);
    }
    void stats_arbitrary(RallyHereGameInstanceAdapterPtr adapter,
                         const char* name,
                         const char* data,
                         unsigned int data_size,
                         void (*callback)(const RallyHereStatusCode& code, void* user_data),
                         void* user_data)
    {
    }
    void stats_arbitrary_move(RallyHereGameInstanceAdapterPtr adapter,
                              const char* name,
                              char* data,
                              unsigned int data_size,
                              void (*callback)(const RallyHereStatusCode& code, char* data, void* user_data),
                              void* user_data)
    {
    }
    RallyHereStatusCode metrics_immediate_action(RallyHereGameInstanceAdapterPtr adapter,
                                                 const RallyHereMetricDefinition* definition,
                                                 const RallyHereMetricAction* action,
                                                 RallyHereMetricFlush flush)
    {
        if (nullptr == adapter)
            return RH_STATUS_VALIDATION_ERROR_ADAPTER_IS_NULLPTR;
        if (nullptr == definition)
            return RH_STATUS_VALIDATION_ERROR_DEFINITION_IS_NULLPTR;
        if (nullptr == action)
            return RH_STATUS_VALIDATION_ERROR_ACTION_IS_NULLPTR;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        return a->MetricsImmediateAction(definition, action, flush).code();
    }
    RallyHereStatusCode metrics_immediate_action_with_time(RallyHereGameInstanceAdapterPtr adapter,
                                                           const RallyHereMetricDefinition* definition,
                                                           const RallyHereMetricAction* action,
                                                           double timestamp,
                                                           RallyHereMetricFlush flush)
    {
        if (nullptr == adapter)
            return RH_STATUS_VALIDATION_ERROR_ADAPTER_IS_NULLPTR;
        if (nullptr == definition)
            return RH_STATUS_VALIDATION_ERROR_DEFINITION_IS_NULLPTR;
        if (nullptr == action)
            return RH_STATUS_VALIDATION_ERROR_ACTION_IS_NULLPTR;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        return a->MetricsImmediateAction(definition, action, timestamp, flush).code();
    }
    void metrics_statsd_output(RallyHereGameInstanceAdapterPtr adapter, const char** data, size_t* data_size)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->MetricsStatsdOutput(data, data_size);
    }
    void metrics_graphite_output(RallyHereGameInstanceAdapterPtr adapter, const char** data, size_t* data_size)
    {
        if (nullptr == adapter)
            return;
        auto a = reinterpret_cast<rallyhere::GameInstanceAdapter*>(adapter);
        a->MetricsGraphiteOutput(data, data_size);
    }

} // namespace
} // namespace rallyhere

extern "C"
{
    void rallyhere_global_init()
    {
        rallyhere::global_init();
    }
    void rallyhere_global_cleanup()
    {
        rallyhere::global_cleanup();
    }

    RallyHereStatusCode rallyhere_create_game_instance_adapter(RallyHereGameInstanceAdapterPtr* adapter, const char* arguments)
    {
        return rallyhere::create_game_instance_adapter(adapter, arguments);
    }

    RallyHereStatusCode
        rallyhere_create_game_instance_adaptern(RallyHereGameInstanceAdapterPtr* adapter, const char* arguments, unsigned int arguments_length)
    {
        return rallyhere::create_game_instance_adaptern(adapter, arguments, arguments_length);
    }

    RallyHereStatusCode rallyhere_create_game_instance_adapter_with_logger(RallyHereGameInstanceAdapterPtr* adapter, const char* arguments, RallyHereLogCallback callback, void* user_data)
    {
        return rallyhere::create_game_instance_adapter_with_logger(adapter, arguments, callback, user_data);
    }

    RallyHereStatusCode
        rallyhere_create_game_instance_adaptern_with_logger(RallyHereGameInstanceAdapterPtr* adapter, const char* arguments, unsigned int arguments_length, RallyHereLogCallback callback, void* user_data)
    {
        return rallyhere::create_game_instance_adaptern_with_logger(adapter, arguments, arguments_length, callback, user_data);
    }

    void rallyhere_destroy_game_instance_adapter(RallyHereGameInstanceAdapterPtr adapter)
    {
        rallyhere::destroy_game_instance_adapter(adapter);
    }

    RallyHereStatusCode rallyhere_tick(RallyHereGameInstanceAdapterPtr adapter)
    {
        return rallyhere::tick(adapter);
    }
    RallyHereStatusCode rallyhere_healthy(RallyHereGameInstanceAdapterPtr adapter)
    {
        return rallyhere::healthy(adapter);
    }
    void rallyhere_connect(RallyHereGameInstanceAdapterPtr adapter,
                           void (*callback)(const RallyHereStatusCode& code, void* user_data),
                           void* user_data)
    {
        rallyhere::connect(adapter, callback, user_data);
    }

    void rallyhere_ready(RallyHereGameInstanceAdapterPtr adapter, void (*callback)(const RallyHereStatusCode& code, void* user_data), void* user_data)
    {
        rallyhere::ready(adapter, callback, user_data);
    }

    void rallyhere_on_allocated_callback(RallyHereGameInstanceAdapterPtr adapter,
                                         RallyHereOnAllocatedCallback callback,
                                         void* user_data)
    {
        rallyhere::on_allocated_callback(adapter, callback, user_data);
    }

    void rallyhere_test_fake_allocation_response(RallyHereGameInstanceAdapterPtr adapter, const char *fake_data)
    {
        rallyhere::test_fake_allocation_response(adapter, fake_data);
    }

    void rallyhere_allocate(RallyHereGameInstanceAdapterPtr adapter,
                            void (*callback)(const RallyHereStatusCode& code, void* user_data),
                            void* user_data)
    {
        rallyhere::allocate(adapter, callback, user_data);
    }

    void rallyhere_get_public_host_and_port(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
    {
        rallyhere::get_public_host_and_port(adapter, map);
    }

    void rallyhere_get_user_agent_string(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
    {
        rallyhere::get_user_agent_string(adapter, map);
    }

    void rallyhere_reserve(RallyHereGameInstanceAdapterPtr adapter,
                           unsigned int timeout_seconds,
                           void (*reserve_callback)(const RallyHereStatusCode& code, void* user_data),
                           void* reserve_user_data,
                           void (*ready_callback)(const RallyHereStatusCode& code, void* user_data),
                           void* ready_user_data)
    {
        rallyhere::reserve(adapter, timeout_seconds, reserve_callback, reserve_user_data, ready_callback, ready_user_data);
    }

    void rallyhere_reserve_unconditional(RallyHereGameInstanceAdapterPtr adapter,
                                         void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                         void* user_data)
    {
        rallyhere::reserve_unconditional(adapter, callback, user_data);
    }
    void rallyhere_on_soft_stop_callback(RallyHereGameInstanceAdapterPtr adapter,
                                         void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                         void* user_data)
    {
        rallyhere::on_soft_stop_callback(adapter, callback, user_data);
    }
    void rallyhere_external_soft_stop_requested(RallyHereGameInstanceAdapterPtr adapter)
    {
        rallyhere::external_soft_stop_requested(adapter);
    }
    void rallyhere_set_log_callback(RallyHereGameInstanceAdapterPtr adapter,
                                    RallyHereLogCallback callback,
                                    void* user_data)
    {
        return rallyhere::set_log_callback(adapter, callback, user_data);
    }
    void rallyhere_set_default_log_level(RallyHereLogLevel level)
    {
        return rallyhere::set_default_log_level(level);
    }
    RallyHereStatusCode rallyhere_string_map_create(RallyHereStringMapPtr* map)
    {
        return rallyhere::string_map_create(map);
    }
    void rallyhere_string_map_destroy(RallyHereStringMapPtr map)
    {
        rallyhere::string_map_destroy(map);
    }
    RallyHereStatusCode rallyhere_string_map_copy(RallyHereStringMapPtr dest, const RallyHereStringMapPtr src)
    {
        return rallyhere::string_map_copy(dest, src);
    }
    void rallyhere_string_map_clear(RallyHereStringMapPtr map)
    {
        rallyhere::string_map_clear(map);
    }
    RallyHereStatusCode rallyhere_string_map_get(RallyHereStringMapPtr map, const char* key, const char** value, unsigned int* value_size)
    {
        return rallyhere::string_map_get(map, key, value, value_size);
    }
    RallyHereStatusCode rallyhere_string_map_set(RallyHereStringMapPtr map, const char* key, const char* value)
    {
        return rallyhere::string_map_set(map, key, value);
    }
    RallyHereStatusCode rallyhere_string_map_setn(RallyHereStringMapPtr map, const char* key, const char* value, unsigned int value_size)
    {
        return rallyhere::string_map_setn(map, key, value, value_size);
    }
    void rallyhere_get_labels(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
    {
        rallyhere::get_labels(adapter, map);
    }
    void rallyhere_set_labels(RallyHereGameInstanceAdapterPtr adapter,
                              const RallyHereStringMapPtr map,
                              void (*callback)(const RallyHereStatusCode& code, void* user_data),
                              void* user_data)
    {
        rallyhere::set_labels(adapter, map, callback, user_data);
    }
    void rallyhere_get_additional_info(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
    {
        rallyhere::get_additional_info(adapter, map);
    }
    void rallyhere_set_additional_info(RallyHereGameInstanceAdapterPtr adapter,
                              const RallyHereStringMapPtr map,
                              void (*callback)(const RallyHereStatusCode& code, void* user_data),
                              void* user_data)
    {
        rallyhere::set_additional_info(adapter, map, callback, user_data);
    }
    void rallyhere_get_annotations(RallyHereGameInstanceAdapterPtr adapter, RallyHereStringMapPtr* map)
    {
        rallyhere::get_annotations(adapter, map);
    }
    void rallyhere_set_annotations(RallyHereGameInstanceAdapterPtr adapter,
                                   const RallyHereStringMapPtr map,
                                   void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                   void* user_data)
    {
        rallyhere::set_annotations(adapter, map, callback, user_data);
    }
    RallyHereStatusCode rallyhere_stats_base(RallyHereGameInstanceAdapterPtr adapter,
                                             const RallyHereStatsBase* stats,
                                             const RallyHereStatsBaseProvided* provided,
                                             void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                             void* user_data)
    {
        return rallyhere::stats_base(adapter, stats, provided, callback, user_data);
    }
    void rallyhere_stats_gauge(RallyHereGameInstanceAdapterPtr adapter,
                               const char* name,
                               double value,
                               void (*callback)(const RallyHereStatusCode& code, void* user_data),
                               void* user_data)
    {
        rallyhere::stats_gauge(adapter, name, value, callback, user_data);
    }
    void rallyhere_stats_gauge_with_labels(RallyHereGameInstanceAdapterPtr adapter,
                                           const char* name,
                                           double value,
                                           const RallyHereStringMapPtr labels,
                                           void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                           void* user_data)
    {
        rallyhere::stats_gauge_with_labels(adapter, name, value, labels, callback, user_data);
    }
    void rallyhere_stats_arbitrary(RallyHereGameInstanceAdapterPtr adapter,
                                   const char* name,
                                   const char* data,
                                   unsigned int data_size,
                                   void (*callback)(const RallyHereStatusCode& code, void* user_data),
                                   void* user_data)
    {
        rallyhere::stats_arbitrary(adapter, name, data, data_size, callback, user_data);
    }
    void rallyhere_stats_arbitrary_move(RallyHereGameInstanceAdapterPtr adapter,
                                        const char* name,
                                        char* data,
                                        unsigned int data_size,
                                        void (*callback)(const RallyHereStatusCode& code, char* data, void* user_data),
                                        void* user_data)
    {
        rallyhere::stats_arbitrary_move(adapter, name, data, data_size, callback, user_data);
    }
    RallyHereStatusCode rallyhere_metrics_immediate_action(RallyHereGameInstanceAdapterPtr adapter,
                                                           const RallyHereMetricDefinition* definition,
                                                           const RallyHereMetricAction* action,
                                                           RallyHereMetricFlush flush)
    {
        return rallyhere::metrics_immediate_action(adapter, definition, action, flush);
    }
    RallyHereStatusCode rallyhere_metrics_immediate_action_with_time(RallyHereGameInstanceAdapterPtr adapter,
                                                                     const RallyHereMetricDefinition* definition,
                                                                     const RallyHereMetricAction* action,
                                                                     double timestamp,
                                                                     RallyHereMetricFlush flush)
    {
        return rallyhere::metrics_immediate_action_with_time(adapter, definition, action, timestamp, flush);
    }
    void rallyhere_metrics_statsd_output(RallyHereGameInstanceAdapterPtr adapter, const char** data, size_t* data_size)
    {
        rallyhere::metrics_statsd_output(adapter, data, data_size);
    }
    void rallyhere_metrics_graphite_output(RallyHereGameInstanceAdapterPtr adapter, const char** data, size_t* data_size)
    {
        rallyhere::metrics_graphite_output(adapter, data, data_size);
    }

}; // extern "C"
