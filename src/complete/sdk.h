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
#ifndef RH_CPP_SDK_H
#define RH_CPP_SDK_H

#include "c_platform.h"
#include "status.h"
#include "rh_string.h"
#include "rh_vector.h"
#include "rh_map.h"
#include "rh_format.h"
#include "custom_resource.h"
#include "sdk_logger.h"
#include "server_json_watcher.hpp"
#include "a2s_listener.hpp"


// TODO: Make sure their is a separate parameter for specifying the public port that isn't required to match the local port
// TODO: Support both ConnectionIP and ConnectionIPV6 on multiplay.
// TODO: Make sure SIC passes through the information in the same way that multiplay does

namespace boost
{
namespace asio
{
    namespace ssl
    {
        namespace detail
        {
            template<bool>
            class openssl_init;
            template<>
            class openssl_init<true>
            {
              public:
                openssl_init()
                {
                    int i = 0;
                }
            };
            template<>
            class openssl_init<false>
            {
              public:
                openssl_init()
                {
                    int i = 0;
                }
            };
        } // namespace detail
    }     // namespace ssl
} // namespace asio
} // namespace boost

#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/ssl.hpp"
#include "boost/beast/version.hpp"
#include "boost/asio.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/strand.hpp"
#include "boost/certify/extensions.hpp"
#include "boost/certify/https_verification.hpp"

#include "prometheus/exposer.h"
#include "prometheus/gauge.h"
#include "prometheus/labels.h"
#include "prometheus/registry.h"

#include "c_api.h"

#include <chrono>
#include <iostream>

namespace rallyhere
{

using string_body = boost::beast::http::basic_string_body<char, std::char_traits<char>, i3d::one::StandardAllocator<char>>;

struct Callbacks
{
    std::function<void(const Status&)> OnConnect;
    void* OnConnectUserData;
};
class StringMap
{
  public:
    void Clear()
    {
        m_Map.clear();
    }
    Status Get(const char* key, const char** value, unsigned int* valueSize)
    {
        auto it = m_Map.find(key);
        if (it == m_Map.end())
            return RH_STATUS_KEY_NOT_FOUND;
        *value = it->second.c_str();
        *valueSize = it->second.size();
        return RH_STATUS_OK;
    }
    Status Set(const char* key, const char* value)
    {
        m_Map.insert_or_assign(key, value);
        return RH_STATUS_OK;
    }
    Status Set(const char* key, const char* value, unsigned int valueSize)
    {
        m_Map.insert_or_assign(key, rallyhere::string{ value, valueSize });
        return RH_STATUS_OK;
    }
    void Set(const char* key, const rallyhere::string& value)
    {
        m_Map.insert_or_assign(key, value);
    }

    auto&& get()
    {
        return m_Map;
    }
    auto&& get() const
    {
        return m_Map;
    }

  protected:
    rallyhere::map<rallyhere::string, rallyhere::string> m_Map;
};

struct SICCredentials
{
    rallyhere::string m_CredentialsFile;
    rallyhere::string m_APIUserName;
    rallyhere::string m_APIPassword;
    rallyhere::string m_APIClientId;
    rallyhere::string m_APISecret;
    bool m_UseCredentialsAsClientId{false};
};

struct TimedStatsChange
{
    std::chrono::steady_clock::time_point time;
    RallyHereStatsBase base;
    RallyHereStatsBaseProvided provided;
};

struct SimulatedGame
{
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    short players;
    short max_players;
};

class GameInstanceAdapter
{
  public:
    using base_callback_function_t = std::function<void(const RallyHereStatusCode&, void*)>;
    using on_allocated_callback_function_t = std::function<void(RallyHereStringMapPtr, const RallyHereStatusCode&, void*)>;
    using log_callback_function_t = std::function<void(RallyHereLogLevel, std::string_view)>;

  public:
    RH_EXPORT GameInstanceAdapter(std::string_view arguments);
    RH_EXPORT GameInstanceAdapter(int argc, const char* argv[]);
    RH_EXPORT GameInstanceAdapter(std::string_view arguments, log_callback_function_t logCallback);
    RH_EXPORT GameInstanceAdapter(int argc, const char* argv[], log_callback_function_t logCallback);
    RH_EXPORT virtual ~GameInstanceAdapter();
    GameInstanceAdapter(const GameInstanceAdapter&) = delete;
    GameInstanceAdapter& operator=(const GameInstanceAdapter&) = delete;

    /** Tell the game host that this GameInstance can now be allocated. */
    RH_EXPORT Status Ready();
    /** Tell the game host to reserve this game instance rather than marking it ready. This will stop the game host from destroying thisl
     * game instance based on any ready timeouts. This is to be used when the game instance is manually handling sessions in a way that
     * isn't expected to be handled by the normal matchmaking route. */
    RH_EXPORT Status Reserve(std::chrono::seconds seconds);
    /** Inform the game host that this game instance is still healthy. Should be called at a regular interval. Every 5 seconds is a good rule of
     * thumb. Currently only used by Agones.
     * SIC relies on an open metrics endpoint created by this SDK.
     * i3D relies on the Arcus socket connection.
     * Multiplay reliies on the A2S socket connection.
     */
    RH_EXPORT bool Health();

    Status CreateGameInstanceAdapter(const char* arguments)
    {
        return RH_STATUS_OK;
    }
    Status CreateGameInstanceAdaptern(const char* arguments, unsigned int argumentsLength)
    {
        return RH_STATUS_OK;
    }

    void DestroyGameInstanceAdapter()
    {
    }
    /** Tick that's expected to be called every frame. This is used to check for any messages from the game host. */
    RH_CPP_EXPORT Status Tick();
    Status Healthy()
    {
        return RH_STATUS_OK;
    }

    /** Connect to the appropriate game host. This action may not actually happen at this time. */
    RH_CPP_EXPORT void Connect(base_callback_function_t callback, void* user_data);

    void Ready(base_callback_function_t callback, void* user_data);

    void OnAllocatedCallback(on_allocated_callback_function_t callback, void* user_data)
    {
        m_OnAllocatedCallback = callback;
        m_OnAllocatedUserData = user_data;
    }

    void Allocate(base_callback_function_t callback, void* user_data);

    void Reserve(unsigned int timeout_seconds,
                 base_callback_function_t reserve_callback,
                 void* reserve_user_data,
                 base_callback_function_t ready_callback,
                 void* ready_user_data);
    void ReserveUnconditional(base_callback_function_t callback, void* user_data);
    void OnSoftStopCallback(base_callback_function_t callback, void* user_data)
    {
        m_SoftStopCallback = callback;
        m_SoftStopUserData = user_data;
    }
    void ExternalSoftStopRequested()
    {
        m_ExternalSoftStopRequested.store(true, std::memory_order_relaxed);
    }
    void OnLogCallback(log_callback_function_t callback)
    {
        m_LogCallback = callback;
    }

    auto&& GetLabels()
    {
        return m_Labels;
    }
    auto&& GetLabels() const
    {
        return m_Labels;
    }
    void SetLabels(const rallyhere::StringMap& map, base_callback_function_t callback, void* user_data)
    {
        if (Registry)
        {
            callback(RH_STATUS_PROMETHEUS_ALREADY_STARTED, user_data);
            return;
        }
        m_Labels = map;
        callback(RH_STATUS_OK, user_data);
    }
    auto&& GetAdditionalInfoLabels()
    {
        return m_AdditionalInfoLabels;
    }
    auto&& GetAdditionalInfoLabels() const
    {
        return m_AdditionalInfoLabels;
    }
    void SetAdditionalInfoLabels(const rallyhere::StringMap& map, base_callback_function_t callback, void* user_data)
    {
        m_AdditionalInfoLabels = map;
        try
        {
            RefreshAdditionalInfoLabels();
        }
        catch (const std::exception& e)
        {
            log().log(RH_LOG_LEVEL_ERROR, "Could not start Prometheus: {}", e.what());
            if (callback)
                callback(RH_STATUS_PROMETHEUS_COULD_NOT_START, user_data);
            return;
        }
        if (callback)
            callback(RH_STATUS_OK, user_data);
    }
    auto&& GetAnnotations()
    {
        return m_Annotations;
    }
    auto&& GetAnnotations() const
    {
        return m_Annotations;
    }
    void SetAnnotations(const rallyhere::StringMap& map, base_callback_function_t callback, void* user_data)
    {
        m_Annotations = map;
    }
    Status StatsBase(const RallyHereStatsBase* stats, const RallyHereStatsBaseProvided* provided, base_callback_function_t callback, void* user_data);
    void StatsGauge(const char* name, double value, base_callback_function_t callback, void* user_data);
    void StatsGaugeWithLabels(const char* name, double value, const rallyhere::StringMap& labels, base_callback_function_t callback, void* user_data);
    void StatsArbitrary(const char* name, const char* data, unsigned int data_size, base_callback_function_t callback, void* user_data)
    {
    }
    void StatsArbitraryMove(const char* name,
                            char* data,
                            unsigned int data_size,
                            std::function<void(const RallyHereStatusCode&, char*, void*)> callback,
                            void* user_data)
    {
    }

    Status MetricsImmediateAction(const RallyHereMetricDefinition* definition,
                                  const RallyHereMetricAction* action,
                                  RallyHereMetricFlush flush);
    Status MetricsImmediateAction(const RallyHereMetricDefinition* definition,
                                  const RallyHereMetricAction* action,
                                  double timestamp,
                                  RallyHereMetricFlush flush);
    void MetricsStatsdOutput(const char** data, size_t* data_size)
    {
        *data = m_StatsdTmpOutput.c_str();
        *data_size = m_StatsdTmpOutput.size();
    }
    void MetricsGraphiteOutput(const char** data, size_t* data_size)
    {
        *data = m_GraphiteTmpOutput.c_str();
        *data_size = m_GraphiteTmpOutput.size();
    }

    /** The callback to trigger when the game host wants to stop the game instance.
     * In SIC this is expected to come from the SIGTERM handler.
     * In i3D this is expected to come from the Arcus commands
     */
    RH_EXPORT bool SetSoftStopCallback();

    RH_EXPORT std::string_view GetModeName() const;

    /// @name SIC Getters
    /// @{
    RH_EXPORT std::string_view GetSicId() const
    {
        return m_SicId;
    }
    RH_EXPORT std::string_view GetSicGroupTags() const
    {
        return m_SicGroupTags;
    }
    /// @}

    /// @name State Functions
    /// @{
    bool good() const
    {
        return m_Status.ok();
    }
    bool fail() const
    {
        return !m_Status.ok();
    }
    bool operator!() const
    {
        return fail();
    }
    explicit operator bool() const
    {
        return !fail();
    }
    rallyhere::Status status() const
    {
        return m_Status;
    }
    /// @}

    logger log()
    {
        return logger(*this);
    }
    friend class logger;

    void FakeAllocated(const char *data)
    {
        m_FakeAllocationData = data;
    }

  private:
    /** Inform the game host that this game instance is done with its current allocation. Should only be called as part of the destruction of
     * this object. */
    RH_EXPORT Status Deallocate();

    RH_EXPORT void Setup();
    RH_EXPORT void SetupA2S();
    RH_EXPORT void SetupSIC();
    RH_EXPORT void SetupMultiplay();
    RH_EXPORT void DoPollSIC(std::function<void(RallyHereStringMapPtr, const RallyHereStatusCode&)> next);
    RH_EXPORT void PollSIC();

    void Login(std::function<void(const RallyHereStatusCode&)> next);
    void ConnectSIC(base_callback_function_t callback, void* user_data);
    void ConnectMultiplay(base_callback_function_t callback, void* user_data);

    void ReadySIC(base_callback_function_t callback, void* user_data);
    void ReadyMultiplay(base_callback_function_t callback, void* user_data);
    void CheckServerJson();
    void StopCheckingServerJson();

    template<typename FuncT>
    void CallAfterAuthValidation(FuncT func)
    {
        // FIXME Implement
        func();
    }

    /// Currently just a placeholder to mark places where we know authorization failed and we need to retry it.
    void RetryAuthorization() {}

    std::pair<boost::beast::http::request<string_body>, boost::system::error_code> BuildLoginRequest(const rallyhere::string& in_url_str);
    std::pair<boost::beast::http::request<string_body>, boost::system::error_code> BuildRegistrationRequest(const rallyhere::string& in_url_str);
    std::pair<boost::beast::http::request<string_body>, boost::system::error_code> BuildPollRequest(const rallyhere::string& in_url_str);

    prometheus::Labels BuildAlwaysPresentLabels();
    void RebuildPrometheus();
    void RefreshAdditionalInfoLabels();
    void AddStatsBaseLabels(prometheus::Labels& labels);
    Status StatsBaseImpl(const RallyHereStatsBase* stats, const RallyHereStatsBaseProvided* provided, base_callback_function_t callback, void* user_data, bool use_simulation_locks);

    Status MetricsImmediateActionStatsd(const RallyHereMetricDefinition* definition,
                                        const RallyHereMetricAction* action,
                                        std::optional<double> timestamp,
                                        RallyHereMetricFlush flush);
    Status MetricsImmediateActionGraphite(const RallyHereMetricDefinition* definition,
                                          const RallyHereMetricAction* action,
                                          std::optional<double> timestamp,
                                          RallyHereMetricFlush flush);

    std::pair<boost::beast::http::request<string_body>, boost::system::error_code> BuildSimulateGameRequest(const rallyhere::string& in_url_str);
    void SimulateGame();

    Callbacks m_Callbacks{};
    log_callback_function_t m_LogCallback{};
    rallyhere::Status m_Status{};
    rallyhere::vector<rallyhere::string> m_Arguments{};
    rallyhere::string m_ModeName{};
    boost::asio::io_context m_IoContext{};
    boost::asio::ssl::context m_SslContext{ boost::asio::ssl::context::tlsv12_client };
    rallyhere::custom_resource m_CustomResource{};
    // This means we can only have one session at a time. Increase the number of these if this is updated to have persistent sessions.
    boost::asio::cancellation_signal m_CancelSignal;
    base_callback_function_t m_SoftStopCallback{};
    void *m_SoftStopUserData{nullptr};
    std::atomic<bool> m_ExternalSoftStopRequested{false};

    /// @name SIC
    /// @{
    SICCredentials m_SICCredentials;
    rallyhere::string m_RallyHereUrl;
    rallyhere::string m_SicProfileId;
    rallyhere::string m_SicServerId;
    rallyhere::string m_SicHostName;
    rallyhere::string m_Port;
    rallyhere::string m_PublicPort;
    rallyhere::string m_MultiHome;
    rallyhere::string m_SicId;
    rallyhere::string m_SicGroupTags;
    std::vector<rallyhere::string> m_ExtraSicGroupTags;
    std::chrono::seconds m_SicPollInterval{1};
    rallyhere::string m_SicPrometheusBind;
    rallyhere::string m_SicPrometheusPortCli;
    bool m_SicPrometheusBindSet{false};
    bool m_SicPrometheusPortSet{false};
    rallyhere::string m_SicPublicHost{};
    std::chrono::milliseconds m_SicRequestTimeout{5000};
    /// @}

    /// @name SIC Public access information debugging
    /// @{
    rallyhere::string m_SicPublicHostCli;
    rallyhere::string m_SicHostNameCli;
    rallyhere::string m_SicHostNameEnvHost;
    rallyhere::string m_SicHostNameEnvIp;
    bool m_SicHostNameQueryLocal{false};
    /// @}

    /// @name SIC Auth
    /// @{
    boost::json::object m_LastAuthResult{&m_CustomResource};
    rallyhere::string m_AccessToken;
    rallyhere::string m_RefreshToken;
    bool m_RefreshTokenValid{false};
    /// @}

    /// @name SIC Ready
    /// @{
    bool m_Registered{false};
    bool m_PollInProgress{false};
    std::chrono::steady_clock::time_point m_NextPollTime{};
    size_t m_PollMaxFailures{5};
    size_t m_PollFailures{0};
    /// For testing purposes. Fake data that will make up the body of the next poll result
    rallyhere::string m_FakeAllocationData;
    /// @}

    /// @name Reservations
    /// @{
    std::chrono::steady_clock::time_point m_ReservationBecomeReady{};
    base_callback_function_t m_ReservationToReadyCallback{};
    void* m_ReservationToReadyUserData{};
    /// @}

    /// @name Allocation
    /// @{
    /// The callback to trigger when we have acknowledged the allocation and the game host adapter has sent thot
    /// or when we have manually allocated and the game host adapter has acknowledged that
    base_callback_function_t m_OnAllocateCallback{};
    void* m_OnAllocateUserData{};
    /// The callback to trigger when the game host adapter notifies us that we have been allocated
    on_allocated_callback_function_t m_OnAllocatedCallback{};
    void* m_OnAllocatedUserData{};
    bool m_Allocated{false};
    rallyhere::string m_AllocationId{};
    rallyhere::string m_SessionId{};
    rallyhere::string m_SessionAuth{};
    rallyhere::string m_PublicHost{};
    rallyhere::string m_AllocatedPublicPort{};
    rallyhere::string m_LastPolledState{"Deallocated"};
    /// @}

    /// @name SIC Prometheus
    /// @{
    std::unique_ptr<prometheus::Exposer, i3d::one::allocator::destroyer<prometheus::Exposer>> Exposer;
    std::shared_ptr<prometheus::Registry> Registry;
    prometheus::Gauge* ConnectedPlayersGauge{};
    prometheus::Gauge* ConnectedSpectatorsGauge{};
    prometheus::Gauge* MaxAllowedPlayersGauge{};
    prometheus::Gauge* LastTickedGauge{};
    prometheus::Gauge* LastAllocatedGauge{};
    prometheus::Family<prometheus::Gauge>* InfoGaugeFamily{};
    prometheus::Gauge* InfoGauge{};
    /// @}

    ///@name Stats
    /// @{
    struct StatsBase m_StatsBase{};
    rallyhere::StringMap m_Labels;
    rallyhere::StringMap m_InternalLabels;
    std::vector<rallyhere::string> m_ExtraLabels;
    rallyhere::StringMap m_AdditionalInfoLabels;
    rallyhere::StringMap m_InternalAdditionalInfoLabels;
    std::vector<rallyhere::string> m_ExtraAdditionalInfoLabels;
    rallyhere::StringMap m_Annotations;
    bool m_HasLoggedNoPrometheus{false};
    std::optional<short> m_ForcedMaxPlayers;
    short m_DefaultMaxPlayers{0};
    bool m_AppendHostNameToLabels{false};
    /// @}

    ///@name Fake Stat Changes
    /// @{
    rallyhere::vector<TimedStatsChange> m_FakeStatChanges{};
    rallyhere::vector<rallyhere::string> m_FakeSimulateLock;
    bool m_RandomSimulator{false};
    std::pair<std::chrono::seconds, std::chrono::seconds> m_SimGameStartupLag{10, 60};
    std::pair<std::chrono::seconds, std::chrono::seconds> m_SimGameLength{std::chrono::minutes(5), std::chrono::minutes(20)};
    std::pair<short, short> m_SimPlayersInGame{0, 5};
    std::pair<short, short> m_SimMaxPlayersInGame{5, 5};
    std::pair<short, short> m_SimNumberOfGames{0, 10};
    rallyhere::string m_SimulatorUrl;
    std::chrono::steady_clock::time_point m_NextSimulatedGame;
    /// @}

    ///@name Multiplay
    /// @{
    rallyhere::string m_MultiplayServerFile;
    std::shared_ptr<ServerJsonWatcher> m_MultiplayServerFileWatcher{};
    /// @}

    ///@aname A2S
    /// @{
    short m_A2SQueryPort{0};
    std::shared_ptr<rallyhere::a2s_listener> m_A2SListener{};
    bool m_A2SChallenge{true};
    /// @}

    rallyhere::string m_UserAgent;

    ///@name Push Metrics Provider statsd
    /// @{
    rallyhere::string m_StatsdHost{"fake"};
    rallyhere::string m_StatsdPort;
    rallyhere::string m_StatsdPrefix;
    rallyhere::memory_buffer m_StatsdBuffer;
    rallyhere::string m_StatsdTmpOutput;
    /// @}

    ///@name Push Metrics Provider graphite
    /// @{
    rallyhere::string m_GraphiteHost{"fake"};
    rallyhere::string m_GraphitePort;
    rallyhere::string m_GraphitePrefix;
    rallyhere::memory_buffer m_GraphiteBuffer;
    rallyhere::string m_GraphiteTmpOutput;
    /// @}
};

} // namespace rallyhere

#endif // RH_CPP_SDK_H
