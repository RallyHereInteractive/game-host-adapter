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
#include "sdk.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include "rh_format.h"
#include "version.h"

#if PLATFORM_LINUX
#include <unistd.h>
#include <sys/inotify.h>
#endif

#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "boost/asio.hpp"
#include "boost/url.hpp"
#if PLATFORM_LINUX
#include "boost/json/src.hpp"
#endif
#include "boost/lexical_cast.hpp"
#include "boost/json/monotonic_resource.hpp"
#include "boost/algorithm/string.hpp"

#include "auth.h"

#include "file_watcher.hpp"

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

#include "sdk_helpers.hpp"
#include "sdk_formatters.hpp"

namespace rallyhere
{

GameInstanceAdapter::GameInstanceAdapter(std::string_view arguments) : m_Arguments{}
{
    ParseCommandLineArgs(m_Arguments, arguments);
    Setup();
}

GameInstanceAdapter::GameInstanceAdapter(int argc, const char* argv[]) : m_Arguments{}
{
    for (size_t i = 0; i < argc; ++i)
    {
        m_Arguments.push_back(argv[i]);
    }
    Setup();
}


GameInstanceAdapter::GameInstanceAdapter(std::string_view arguments, log_callback_function_t logCallback) : m_Arguments{}, m_LogCallback{logCallback}
{
    ParseCommandLineArgs(m_Arguments, arguments);
    Setup();
}

GameInstanceAdapter::GameInstanceAdapter(int argc, const char* argv[], log_callback_function_t logCallback) : m_Arguments{}, m_LogCallback{logCallback}
{
    for (size_t i = 0; i < argc; ++i)
    {
        m_Arguments.push_back(argv[i]);
    }
    Setup();
}

GameInstanceAdapter::~GameInstanceAdapter()
{
    if (m_ReservationToReadyCallback)
        m_ReservationToReadyCallback(RH_STATUS_CANCELLED, m_ReservationToReadyUserData);
    if (m_OnAllocateCallback)
        m_OnAllocateCallback(RH_STATUS_CANCELLED, m_OnAllocateUserData);
    if (m_OnAllocatedCallback)
        m_OnAllocatedCallback(nullptr, RH_STATUS_CANCELLED, m_OnAllocatedUserData);
    m_LogCallback = {};
    m_ReservationToReadyCallback = {};
    m_OnAllocateCallback = {};
    m_OnAllocatedCallback = {};
    m_CancelSignal.emit(boost::asio::cancellation_type::partial);
    if (m_MultiplayServerFileWatcher)
        m_MultiplayServerFileWatcher->cancel();
    if (m_A2SListener)
        m_A2SListener->cancel();
    if (m_IoContext.stopped())
        m_IoContext.restart();
    m_IoContext.run();
}

Status GameInstanceAdapter::Tick()
{
    auto copyFileWatcher = m_MultiplayServerFileWatcher;
    auto copyA2SListener = m_A2SListener;
    if (m_IoContext.stopped())
        m_IoContext.restart();
    m_IoContext.poll();

    auto externalSoftStopRequested = m_ExternalSoftStopRequested.exchange(false, std::memory_order_relaxed);
    if (externalSoftStopRequested && m_SoftStopCallback)
        m_SoftStopCallback(RH_STATUS_OK, m_SoftStopUserData);

    auto now = std::chrono::steady_clock::now();
    if (m_ReservationBecomeReady != std::chrono::steady_clock::time_point{} && now >= m_ReservationBecomeReady)
    {
        m_ReservationBecomeReady = {};
        Ready(m_ReservationToReadyCallback, m_ReservationToReadyUserData);
    }
    if (m_Registered && now >= m_NextPollTime && !m_Allocated)
    {
        if (!m_OnAllocatedCallback)
            return RH_STATUS_CANNOT_POLL_WITHOUT_ON_ALLOCATED_CALLBACK;
        m_NextPollTime = now + m_SicPollInterval;
        PollSIC();
    }
    if (LastTickedGauge)
        LastTickedGauge->SetToCurrentTime();
    for (auto it = m_FakeStatChanges.begin(); it != m_FakeStatChanges.end();)
    {
        if (now >= it->time)
        {
            StatsBaseImpl(&it->base, &it->provided, nullptr, nullptr, false);
            it = m_FakeStatChanges.erase(it);
        }
        else
        {
            ++it;
        }
    }
    if (now > m_NextSimulatedGame)
    {
        m_NextSimulatedGame = now + std::chrono::minutes{1};
        SimulateGame();
    }
    return RH_STATUS_OK;
}

/** Connect to the appropriate game host. This action may not actually happen at this time. */
void GameInstanceAdapter::Connect(base_callback_function_t callback, void* user_data)
{
    if (m_ModeName == "SIC")
        ConnectSIC(callback, user_data);
    else if (m_ModeName == "Multiplay")
        ConnectMultiplay(callback, user_data);
    else if (m_ModeName == "AutoCreate")
    {
        if (callback)
            callback(RH_STATUS_OK, user_data);
    }
    else
        if (callback)
            callback(RH_STATUS_ERROR, user_data);
}

void GameInstanceAdapter::Ready(base_callback_function_t callback, void* user_data)
{
    if (m_ModeName == "SIC")
        ReadySIC(callback, user_data);
    else if (m_ModeName == "Multiplay")
        ReadyMultiplay(callback, user_data);
    else if (m_ModeName == "AutoCreate")
    {}
    else
        if (callback)
            callback(RH_STATUS_ERROR, user_data);
}

void GameInstanceAdapter::Allocate(base_callback_function_t callback, void* user_data)
{
    if (LastAllocatedGauge)
        LastAllocatedGauge->SetToCurrentTime();
    callback(RH_STATUS_OK, user_data);
    if (m_OnAllocatedCallback)
        m_OnAllocatedCallback(nullptr, RH_STATUS_OK, m_OnAllocatedUserData);
    m_ReservationBecomeReady = {};
    m_ReservationToReadyCallback = {};
    m_ReservationToReadyUserData = {};
}

void GameInstanceAdapter::Reserve(unsigned int timeout_seconds,
                                  base_callback_function_t reserve_callback,
                                  void* reserve_user_data,
                                  base_callback_function_t ready_callback,
                                  void* ready_user_data)
{
    if (0 == timeout_seconds)
    {
        ReserveUnconditional(reserve_callback, reserve_user_data);
        return;
    }
    m_ReservationBecomeReady = std::chrono::steady_clock::now() + std::chrono::seconds(timeout_seconds);
    m_ReservationToReadyCallback = ready_callback;
    m_ReservationToReadyUserData = ready_user_data;
    RebuildPrometheus();
    reserve_callback(RH_STATUS_OK, reserve_user_data);
}

void GameInstanceAdapter::ReserveUnconditional(base_callback_function_t callback, void* user_data)
{
    RebuildPrometheus();
    callback(RH_STATUS_OK, user_data);
}

Status GameInstanceAdapter::StatsBase(const RallyHereStatsBase* stats, const RallyHereStatsBaseProvided* provided, base_callback_function_t callback, void* user_data)
{
    return StatsBaseImpl(stats, provided, callback, user_data, true);
}

template<typename T, typename J>
void update_if_changed(T& left, const J& right, bool& changed)
{
    if (left != right)
    {
        left = right;
        changed = true;
    }
}

Status GameInstanceAdapter::StatsBaseImpl(const RallyHereStatsBase* stats, const RallyHereStatsBaseProvided* provided, base_callback_function_t callback, void* user_data, bool use_simulation_locks)
{
    auto stat_not_locked = [&](const rallyhere::string& name)
    {
        if (!use_simulation_locks)
            return true;
        return std::find(m_FakeSimulateLock.begin(), m_FakeSimulateLock.end(), name) == m_FakeSimulateLock.end();
    };

    bool changed = false;
    if (provided->set_name && stat_not_locked("name"))
        update_if_changed(m_StatsBase.name, stats->name, changed);
    if (provided->set_map && stat_not_locked("map"))
        update_if_changed(m_StatsBase.map, stats->map, changed);
    if (provided->set_folder && stat_not_locked("folder"))
        update_if_changed(m_StatsBase.folder, stats->folder, changed);
    if (provided->set_game && stat_not_locked("game"))
        update_if_changed(m_StatsBase.game, stats->game, changed);
    if (provided->set_id && stat_not_locked("id"))
        update_if_changed(m_StatsBase.id, stats->id, changed);
    if (provided->set_players && stat_not_locked("players"))
        update_if_changed(m_StatsBase.players, stats->players, changed);
    if (provided->set_max_players && !m_ForcedMaxPlayers && stat_not_locked("max_players"))
        update_if_changed(m_StatsBase.max_players, stats->max_players, changed);
    if (provided->set_bots && stat_not_locked("bots"))
        update_if_changed(m_StatsBase.bots, stats->bots, changed);
    if (provided->set_server_type && stat_not_locked("server_type"))
        update_if_changed(m_StatsBase.server_type, stats->server_type, changed);
    if (provided->set_environment && stat_not_locked("environment"))
        update_if_changed(m_StatsBase.environment, stats->environment, changed);
    if (provided->set_visibility && stat_not_locked("visibility"))
        update_if_changed(m_StatsBase.visibility, stats->visibility, changed);
    if (provided->set_anticheat && stat_not_locked("anticheat"))
        update_if_changed(m_StatsBase.anticheat, stats->anticheat, changed);
    if (provided->set_version && stat_not_locked("version"))
        update_if_changed(m_StatsBase.version, stats->version, changed);
    if (provided->set_name || provided->set_map || provided->set_folder || provided->set_game || provided->set_version)
        if (changed)
            RefreshAdditionalInfoLabels();
    // Add to Prometheus
    if (provided->set_players)
        if (ConnectedPlayersGauge)
            ConnectedPlayersGauge->Set(m_StatsBase.players);
    if (provided->set_max_players && !m_ForcedMaxPlayers)
        if (MaxAllowedPlayersGauge)
            MaxAllowedPlayersGauge->Set(m_StatsBase.max_players);
    // Update A2S
    if (m_A2SListener)
    {
        auto deferred_callback = [callback, user_data]() {
            if (callback)
                callback(RH_STATUS_OK, user_data);
        };
        m_A2SListener->update_server_info(m_StatsBase, *provided, deferred_callback);
    }
    else
    {
        if (callback)
            callback(RH_STATUS_OK, user_data);
    }
    return RH_STATUS_OK;
}

void GameInstanceAdapter::StatsGauge(const char* name, double value, base_callback_function_t callback, void* user_data)
{
    if (!Registry)
        return;
    prometheus::Labels alwaysPresentLabels = BuildAlwaysPresentLabels();

    auto&& gauge = prometheus::BuildGauge()
            .Name(name)
            .Labels(alwaysPresentLabels)
            .Register(*Registry)
            .Add({});
    gauge.Set(value);
}

void GameInstanceAdapter::StatsGaugeWithLabels(const char* name, double value, const rallyhere::StringMap& labels, base_callback_function_t callback, void* user_data)
{
    if (!Registry)
        return;
    prometheus::Labels alwaysPresentLabels = BuildAlwaysPresentLabels();

    prometheus::Labels dynamicInfoLabels;
    for (auto&& p : labels.get())
    {
        auto insert_pair = std::make_pair(std::string_view{p.first.data(), p.first.size()}, std::string_view{p.second.data(), p.second.size()});
        dynamicInfoLabels.insert(insert_pair);
    }

    auto&& gauge = prometheus::BuildGauge()
            .Name(name)
            .Labels(alwaysPresentLabels)
            .Register(*Registry)
            .Add(dynamicInfoLabels);
    gauge.Set(value);
}

Status GameInstanceAdapter::MetricsImmediateAction(const RallyHereMetricDefinition* definition,
                                                   const RallyHereMetricAction* action,
                                                   RallyHereMetricFlush flush)
{
    // Statsd
    auto statsd_status = MetricsImmediateActionStatsd(definition, action, std::nullopt, flush);
    // Graphite
    auto graphite_status = MetricsImmediateActionGraphite(definition, action, std::nullopt, flush);

    // Only return the failure if both fail
    if (!statsd_status && !graphite_status)
        return RH_STATUS_ERROR;
    return RH_STATUS_OK;
}

Status GameInstanceAdapter::MetricsImmediateAction(const RallyHereMetricDefinition* definition,
                                                   const RallyHereMetricAction* action,
                                                   double timestamp,
                                                   RallyHereMetricFlush flush)
{
    // Statsd
    auto statsd_status = MetricsImmediateActionStatsd(definition, action, timestamp, flush);
    // Graphite
    auto graphite_status = MetricsImmediateActionGraphite(definition, action, timestamp, flush);

    // Only return the failure if both fail
    if (!statsd_status && !graphite_status)
        return RH_STATUS_ERROR;
    return RH_STATUS_OK;
}

std::string_view GameInstanceAdapter::GetModeName() const
{
    return m_ModeName;
}

void GameInstanceAdapter::Setup()
{
    rallyhere::string bootstrapMode{};
    rallyhere::string provider{};
    m_UserAgent = BOOST_BEAST_VERSION_STRING;
    for (auto&& arg : m_Arguments)
    {
        rallyhere::string tmp;
        if (ParseArgument("rhbootstrapmode=", arg, bootstrapMode))
        {
            continue;
        }
        if (ParseArgument("ghaprovider=", arg, provider))
        {
            continue;
        }
        if (ParseArgument("rallyhereurl=", arg, m_RallyHereUrl))
        {
            continue;
        }
        if (ParseArgument("rhuseragent=", arg, m_UserAgent))
        {
            continue;
        }
        if (ParseArgument("rhforcereportmaxplayers=", arg, tmp))
        {
            try
            {
                m_ForcedMaxPlayers = boost::lexical_cast<decltype(m_ForcedMaxPlayers)::value_type>(tmp);
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_REPORT_FORCED_MAX_PLAYERS_MUST_BE_UNSIGNED_INT_8 };
            }
            continue;
        }
        if (ParseArgument("rhdefaultreportmaxplayers=", arg, tmp))
        {
            try
            {
                m_DefaultMaxPlayers = boost::lexical_cast<decltype(m_DefaultMaxPlayers)>(tmp);
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_REPORT_DEFAULT_MAX_PLAYERS_MUST_BE_UNSIGNED_INT_8 };
            }
            continue;
        }
        if (ParseArgument("rhsimulatecurrentplayersat=", arg, tmp))
        {
            rallyhere::vector<rallyhere::string> arguments;
            boost::split(arguments, tmp, boost::is_any_of(","), boost::token_compress_on);
            if (arguments.size() != 2)
            {
                m_Status = { RH_STATUS_SIMULATE_CURRENT_PLAYERS_AT_MUST_BE_TWO_VALUES };
                continue;
            }
            TimedStatsChange change{};
            try
            {
                short seconds = boost::lexical_cast<short>(arguments[0]);
                change.time = std::chrono::steady_clock::now() + std::chrono::seconds{seconds};
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_SIMULATE_CURRENT_PLAYERS_AT_MUST_BE_UNSIGNED_INT_8 };
                continue;
            }
            try
            {
                change.base.players = boost::lexical_cast<short>(arguments[1]);
                change.provided.set_players = true;
                m_FakeStatChanges.push_back(change);
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_SIMULATE_CURRENT_PLAYERS_AT_MUST_BE_UNSIGNED_INT_8 };
                continue;
            }
        }
        if (ParseArgument("rhsimulatemaxplayersat=", arg, tmp))
        {
            rallyhere::vector<rallyhere::string> arguments;
            boost::split(arguments, tmp, boost::is_any_of(","), boost::token_compress_on);
            if (arguments.size() != 2)
            {
                m_Status = { RH_STATUS_SIMULATE_MAX_PLAYERS_AT_MUST_BE_TWO_VALUES };
                continue;
            }
            TimedStatsChange change{};
            try
            {
                short seconds = boost::lexical_cast<short>(arguments[0]);
                change.time = std::chrono::steady_clock::now() + std::chrono::seconds{seconds};
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_SIMULATE_MAX_PLAYERS_AT_MUST_BE_UNSIGNED_INT_8 };
                continue;
            }
            try
            {
                change.base.max_players = boost::lexical_cast<short>(arguments[1]);
                change.provided.set_max_players = true;
                m_FakeStatChanges.push_back(change);
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_SIMULATE_MAX_PLAYERS_AT_MUST_BE_UNSIGNED_INT_8 };
                continue;
            }
        }
        if (ParseArgument("rhsimulatelock=", arg, tmp))
        {
            boost::split(m_FakeSimulateLock, tmp, boost::is_any_of(","), boost::token_compress_on);
        }
        if (ParseArgument("rhsimulatorgamestartuplag=", arg, tmp))
        {
            m_RandomSimulator = true;
            rallyhere::vector<rallyhere::string> arguments;
            boost::split(arguments, tmp, boost::is_any_of(","), boost::token_compress_on);
            if (arguments.size() != 2)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
            try
            {
                m_SimGameStartupLag = {
                    std::chrono::seconds(boost::lexical_cast<std::chrono::seconds::rep>(arguments[0])),
                    std::chrono::seconds(boost::lexical_cast<std::chrono::seconds::rep>(arguments[1])),
                };
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
        }
        if (ParseArgument("rhsimulatorgamelength=", arg, tmp))
        {
            m_RandomSimulator = true;
            rallyhere::vector<rallyhere::string> arguments;
            boost::split(arguments, tmp, boost::is_any_of(","), boost::token_compress_on);
            if (arguments.size() != 2)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
            try
            {
                m_SimGameLength = {
                    std::chrono::seconds(boost::lexical_cast<std::chrono::seconds::rep>(arguments[0])),
                    std::chrono::seconds(boost::lexical_cast<std::chrono::seconds::rep>(arguments[1])),
                };
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
        }
        if (ParseArgument("rhsimulatorplayersingame=", arg, tmp))
        {
            m_RandomSimulator = true;
            rallyhere::vector<rallyhere::string> arguments;
            boost::split(arguments, tmp, boost::is_any_of(","), boost::token_compress_on);
            if (arguments.size() != 2)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
            try
            {
                m_SimPlayersInGame = {
                    boost::lexical_cast<short>(arguments[0]),
                    boost::lexical_cast<short>(arguments[1]),
                };
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
        }
        if (ParseArgument("rhsimulatorplayersingame=", arg, tmp))
        {
            m_RandomSimulator = true;
            rallyhere::vector<rallyhere::string> arguments;
            boost::split(arguments, tmp, boost::is_any_of(","), boost::token_compress_on);
            if (arguments.size() != 2)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
            try
            {
                m_SimPlayersInGame = {
                    boost::lexical_cast<short>(arguments[0]),
                    boost::lexical_cast<short>(arguments[1]),
                };
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
        }
        if (ParseArgument("rhsimulatormaxplayersingame=", arg, tmp))
        {
            m_RandomSimulator = true;
            rallyhere::vector<rallyhere::string> arguments;
            boost::split(arguments, tmp, boost::is_any_of(","), boost::token_compress_on);
            if (arguments.size() != 2)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
            try
            {
                m_SimMaxPlayersInGame = {
                    boost::lexical_cast<short>(arguments[0]),
                    boost::lexical_cast<short>(arguments[1]),
                };
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
        }
        if (ParseArgument("rhsimulatornumberofgames=", arg, tmp))
        {
            m_RandomSimulator = true;
            rallyhere::vector<rallyhere::string> arguments;
            boost::split(arguments, tmp, boost::is_any_of(","), boost::token_compress_on);
            if (arguments.size() != 2)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
            try
            {
                m_SimNumberOfGames = {
                    boost::lexical_cast<short>(arguments[0]),
                    boost::lexical_cast<short>(arguments[1]),
                };
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_SIMULATE_GAME_STARTUP_LAG_MUST_BE_TWO_VALUES };
                continue;
            }
        }
        if (ParseArgument("rhsimulatorurl=", arg, m_SimulatorUrl))
        {
            m_NextSimulatedGame = std::chrono::steady_clock::now();
            continue;
        }
    }
    if (m_RallyHereUrl.empty())
    {
        m_Status = { RH_STATUS_NO_RALLYHERE_URL_PROVIDED };
        return;
    }
    SetupA2S();
    //m_SslContext.set_verify_mode(ssl::context::verify_peer | ssl::context::verify_fail_if_no_peer_cert);
    m_SslContext.set_verify_mode(ssl::context::verify_none);
    m_SslContext.set_default_verify_paths();
    boost::certify::enable_native_https_server_verification(m_SslContext);
    if (!provider.empty())
    {
        m_ModeName = provider;
    }
    else
    {
        m_ModeName = bootstrapMode;
    }
    log().log(RH_LOG_LEVEL_INFO, "Chosen mode: {} from bootstrap: {} or provider: {}", m_ModeName, bootstrapMode, provider);
    if (m_ModeName == "SIC")
    {
        SetupSIC();
    }
    else if (m_ModeName == "Multiplay")
    {
        SetupMultiplay();
    }
    else if (m_ModeName == "AutoCreate")
    {}
    else
    {
        m_Status = { RH_STATUS_NO_BOOTSTRAP_MODE_PROVIDED };
    }

    // Generate all of the fake games
    if (m_RandomSimulator)
    {
        auto logger = log();
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<short> numberOfGamesDistribution(m_SimNumberOfGames.first, m_SimNumberOfGames.second);
        std::uniform_int_distribution<short> playersInGameDistribution(m_SimPlayersInGame.first, m_SimPlayersInGame.second);
        std::uniform_int_distribution<short> maxPlayersInGameDistribution(m_SimMaxPlayersInGame.first, m_SimMaxPlayersInGame.second);
        std::uniform_int_distribution<std::chrono::seconds::rep> gameStartupLagDistribution(m_SimGameStartupLag.first.count(), m_SimGameStartupLag.second.count());
        std::uniform_int_distribution<std::chrono::seconds::rep> gameLengthDistribution(m_SimGameLength.first.count(), m_SimGameLength.second.count());
        auto numberOfGames = numberOfGamesDistribution(g);
        logger.log(RH_LOG_LEVEL_INFO, "Simulating {} games", numberOfGames);
        auto last_time_point = std::chrono::steady_clock::now();
        for (short i = 0; i < numberOfGames; ++i)
        {
            SimulatedGame game{};
            game.players = playersInGameDistribution(g);
            game.max_players = maxPlayersInGameDistribution(g);
            game.start = last_time_point + std::chrono::seconds{ gameStartupLagDistribution(g) };
            game.end = game.start + std::chrono::seconds{ gameLengthDistribution(g) };
            logger.log(RH_LOG_LEVEL_INFO,
                       "Simulated game {} players {} max_players {} start {} end {}",
                       i,
                       game.players,
                       game.max_players,
                       game.start.time_since_epoch().count(),
                       game.end.time_since_epoch().count());
            // Start with the max players
            TimedStatsChange init{};
            init.time = last_time_point;
            init.base.max_players = game.max_players;
            init.provided.set_max_players = true;
            m_FakeStatChanges.push_back(init);
            // Have the players join when the game starts
            TimedStatsChange start{};
            start.time = game.start;
            start.base.players = game.players;
            start.base.max_players = game.max_players;
            start.provided.set_players = true;
            start.provided.set_max_players = true;
            m_FakeStatChanges.push_back(start);
            // Have everybody leave when the game is over
            TimedStatsChange end{};
            end.time = game.end;
            end.base.players = 0;
            end.base.max_players = game.max_players;
            end.provided.set_players = true;
            end.provided.set_max_players = true;
            m_FakeStatChanges.push_back(end);
            // The next game starts when the last one ends
            last_time_point = game.end;
        }
    }
}

void GameInstanceAdapter::SetupA2S()
{
    for (auto&& arg : m_Arguments)
    {
        rallyhere::string tmp;
        if (ParseArgument("A2SPort=", arg, tmp))
        {
            try
            {
                m_A2SQueryPort = boost::lexical_cast<decltype(m_A2SQueryPort)>(tmp);
            }
            catch (const boost::bad_lexical_cast&e)
            {
                m_Status = { RH_STATUS_A2S_QUERY_PORT_MUST_BE_SHORT_NUMBER};
            }
            continue;
        }
        if (ParseArgument("A2Schallenge=", arg, tmp))
        {
            if (tmp[0] == 'f' || tmp[0] == 'F' || tmp[0] == '0' || tmp[0] == 'n' || tmp[0] == 'N')
                m_A2SChallenge = false;
            continue;
        }
    }

    m_StatsBase.server_type = 'd';
#if PLATFORM_WINDOWS
    m_StatsBase.environment = 'w';
#elif PLATFORM_APPLE
    m_StatsBase.environment = 'm';
#elif PLATFORM_LINUX
    m_StatsBase.environment = 'l';
#else
#error "platform not implemented"
#endif
    m_StatsBase.visibility = 0;
    m_StatsBase.anticheat = 0;
    m_StatsBase.max_players = m_DefaultMaxPlayers;
    if (m_ForcedMaxPlayers)
        m_StatsBase.max_players = *m_ForcedMaxPlayers;
}

prometheus::Labels GameInstanceAdapter::BuildAlwaysPresentLabels()
{
    prometheus::Labels alwaysPresentLabels;
    for (auto&& p : m_Labels.get())
    {
        auto insert_pair = std::make_pair(std::string_view{p.first.data(), p.first.size()}, std::string_view{p.second.data(), p.second.size()});
        alwaysPresentLabels.insert(insert_pair);
    }
    for (auto&& p : m_InternalLabels.get())
    {
        auto insert_pair = std::make_pair(std::string_view{p.first.data(), p.first.size()}, std::string_view{p.second.data(), p.second.size()});
        alwaysPresentLabels.insert(insert_pair);
    }
    return alwaysPresentLabels;
}

void GameInstanceAdapter::RebuildPrometheus()
{
    if (m_SicPrometheusBind.empty())
    {
        if (!m_HasLoggedNoPrometheus)
        {
            log().log(RH_LOG_LEVEL_INFO, "No prometheus configured");
            m_HasLoggedNoPrometheus = true;
        }
        return;
    }

    log().log(RH_LOG_LEVEL_INFO, "Setting up Prometheus metrics endpoint");
    if (Exposer && Registry)
    {
        Exposer->RemoveCollectable(Registry);
    }

    auto alloc = i3d::one::StandardAllocator<prometheus::Registry>{};
    Registry = std::allocate_shared<prometheus::Registry>(alloc);

    prometheus::Labels alwaysPresentLabels = BuildAlwaysPresentLabels();

    auto& connectedGaugeFamily = prometheus::BuildGauge()
                                     .Name("connected_clients_total")
                                     .Help("Number of clients (players/spectators) connected to instance")
                                     .Labels(alwaysPresentLabels)
                                     .Register(*Registry);
    ConnectedPlayersGauge = &connectedGaugeFamily.Add({ { "client_type", "player" } });
    ConnectedSpectatorsGauge = &connectedGaugeFamily.Add({ { "client_type", "spectator" } });
    ConnectedPlayersGauge->Set(m_StatsBase.players);

    MaxAllowedPlayersGauge = &prometheus::BuildGauge()
                                  .Name("max_allowed_players_total")
                                  .Help("Number of players allowed based on game queue")
                                  .Labels(alwaysPresentLabels)
                                  .Register(*Registry)
                                  .Add({});
    MaxAllowedPlayersGauge->Set(m_StatsBase.max_players);

    LastTickedGauge = &prometheus::BuildGauge()
                           .Name("last_ticked_timestamp")
                           .Help("The unix timestamp when the prometheus module was ticked by the engine")
                           .Labels(alwaysPresentLabels)
                           .Register(*Registry)
                           .Add({});
    LastTickedGauge->SetToCurrentTime();

    LastAllocatedGauge = &prometheus::BuildGauge()
                              .Name("last_allocated_timestamp")
                              .Help("The unix timestamp when the instance was last moved to the allocated state")
                              .Labels(alwaysPresentLabels)
                              .Register(*Registry)
                              .Add({});

    InfoGaugeFamily = nullptr;
    RefreshAdditionalInfoLabels();

    if (!Exposer)
    {
        auto tmp = i3d::one::allocator::create<prometheus::Exposer>(std::string{ m_SicPrometheusBind.data(), m_SicPrometheusBind.size() }, 1);
        Exposer.reset(tmp);
    }
    Exposer->RegisterCollectable(Registry);
}

void GameInstanceAdapter::RefreshAdditionalInfoLabels()
{
    if (!Registry)
        return;
    if (!InfoGaugeFamily)
    {
        prometheus::Labels alwaysPresentLabels = BuildAlwaysPresentLabels();
        InfoGaugeFamily = &prometheus::BuildGauge()
                .Name("instance_info")
                .Help("General instance info")
                .Labels(alwaysPresentLabels)
                .Register(*Registry);
    }
    else
    {
        InfoGaugeFamily->Remove(InfoGauge);
    }
    prometheus::Labels dynamicInfoLabels;
    AddStatsBaseLabels(dynamicInfoLabels);
    for (auto&& p : m_AdditionalInfoLabels.get())
    {
        auto insert_pair = std::make_pair(std::string_view{p.first.data(), p.first.size()}, std::string_view{p.second.data(), p.second.size()});
        dynamicInfoLabels.insert(insert_pair);
    }
    for (auto&& p : m_InternalAdditionalInfoLabels.get())
    {
        auto insert_pair = std::make_pair(std::string_view{p.first.data(), p.first.size()}, std::string_view{p.second.data(), p.second.size()});
        dynamicInfoLabels.insert(insert_pair);
    }
    dynamicInfoLabels.insert(std::make_pair(std::string_view{"gha_version"}, std::string_view{GAME_HOST_ADAPTER_VERSION}));

    InfoGauge = &InfoGaugeFamily->Add(dynamicInfoLabels);
    InfoGauge->Set(1);
}

void GameInstanceAdapter::AddStatsBaseLabels(prometheus::Labels& dynamicInfoLabels)
{
    auto insert_pair = std::make_pair(std::string_view{"name"}, std::string_view{m_StatsBase.name.data(), m_StatsBase.name.size()});
    dynamicInfoLabels.insert(insert_pair);
    insert_pair = std::make_pair(std::string_view{"map"}, std::string_view{m_StatsBase.map.data(), m_StatsBase.map.size()});
    dynamicInfoLabels.insert(insert_pair);
    insert_pair = std::make_pair(std::string_view{"folder"}, std::string_view{m_StatsBase.folder.data(), m_StatsBase.folder.size()});
    dynamicInfoLabels.insert(insert_pair);
    insert_pair = std::make_pair(std::string_view{"game"}, std::string_view{m_StatsBase.game.data(), m_StatsBase.game.size()});
    dynamicInfoLabels.insert(insert_pair);
    insert_pair = std::make_pair(std::string_view{"version"}, std::string_view{m_StatsBase.version.data(), m_StatsBase.version.size()});
    dynamicInfoLabels.insert(insert_pair);
}

Status GameInstanceAdapter::MetricsImmediateActionStatsd(const RallyHereMetricDefinition* definition,
                                                         const RallyHereMetricAction* action,
                                                         std::optional<double> timestamp,
                                                         RallyHereMetricFlush flush)
{
    if (m_StatsdHost.empty())
        return RH_STATUS_METRIC_BACKEND_STATSD_NOT_CONFIGURED;

    if (action->type == RALLY_HERE_METRIC_ACTION_INCREMENT && action->action.increment.change == 0)
        return RH_STATUS_OK;
    if (action->type == RALLY_HERE_METRIC_ACTION_INCREMENT_WITH_SAMPLE && action->action.increment_with_sample.change == 0)
        return RH_STATUS_OK;

    fmt::format_to(std::back_inserter(m_StatsdBuffer), "{}", definition->name);
    switch (action->type)
    {
    case RALLY_HERE_METRIC_ACTION_INCREMENT:
        if (definition->type == RALLY_HERE_METRIC_TYPE_GAUGE)
        {
            if (action->action.increment.change > 0)
                fmt::format_to(std::back_inserter(m_StatsdBuffer), ":+{}", action->action.increment.change);
            else
                fmt::format_to(std::back_inserter(m_StatsdBuffer), ":{}", action->action.increment.change);
        }
        else
        {
            fmt::format_to(std::back_inserter(m_StatsdBuffer), ":{}", action->action.increment.change);
        }
        break;
    case RALLY_HERE_METRIC_ACTION_INCREMENT_WITH_SAMPLE:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), ":{}", action->action.increment_with_sample.change);
        break;
    case RALLY_HERE_METRIC_ACTION_SET:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), ":{}", action->action.set.value);
        break;
    case RALLY_HERE_METRIC_ACTION_SET_WITH_SAMPLE:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), ":{}", action->action.set_with_sample.value);
        break;
    case RALLY_HERE_METRIC_ACTION_UNIQUE:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), ":{}", action->action.unique.unique);
        break;
    }

    switch (definition->type)
    {
    case RALLY_HERE_METRIC_TYPE_COUNTER:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), "|c");
        break;
    case RALLY_HERE_METRIC_TYPE_GAUGE:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), "|g");
        break;
    case RALLY_HERE_METRIC_TYPE_TIMER:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), "|ms");
        break;
    case RALLY_HERE_METRIC_TYPE_SET:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), "|s");
        break;
    }

    switch (action->type)
    {
    case RALLY_HERE_METRIC_ACTION_INCREMENT:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), "\n");
        break;
    case RALLY_HERE_METRIC_ACTION_INCREMENT_WITH_SAMPLE:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), "|@{}\n", action->action.increment_with_sample.sample_rate);
        break;
    case RALLY_HERE_METRIC_ACTION_SET:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), "\n");
        break;
    case RALLY_HERE_METRIC_ACTION_SET_WITH_SAMPLE:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), "|@{}\n", action->action.set_with_sample.sample_rate);
        break;
    case RALLY_HERE_METRIC_ACTION_UNIQUE:
        fmt::format_to(std::back_inserter(m_StatsdBuffer), "\n");
        break;
    }

    if (flush == RALLY_HERE_METRIC_FLUSH)
    {
        m_StatsdTmpOutput = { m_StatsdBuffer.data(), m_StatsdBuffer.size() };
        m_StatsdBuffer.clear();
    }

    return RH_STATUS_OK;
}

void add_graphite_tags(rallyhere::memory_buffer& buffer, const char* label_names, const char* label_values)
{
    if (label_names)
    {
        bool more_labels = true;
        auto label_name = std::string_view{ label_names };
        auto label_value = std::string_view{ label_values };
        while (more_labels)
        {
            if (label_name.empty())
            {
                more_labels = false;
                break;
            }
            fmt::format_to(std::back_inserter(buffer), ";{}={}", label_name, label_value);
            label_name = std::string_view{ label_name.data() + label_name.size() + 1 };
            label_value = std::string_view{ label_value.data() + label_value.size() + 1 };
        }
    }
}

Status GameInstanceAdapter::MetricsImmediateActionGraphite(const RallyHereMetricDefinition* definition,
                                                           const RallyHereMetricAction* action,
                                                           std::optional<double> timestamp,
                                                           RallyHereMetricFlush flush)
{
    if (m_GraphiteHost.empty())
        return RH_STATUS_METRIC_BACKEND_GRAPHITE_NOT_CONFIGURED;

    // Graphite basically only supports gauges natively.
    // We could support counters if we tracked the last value and did the math ourselves.
    switch (definition->type)
    {
    case RALLY_HERE_METRIC_TYPE_COUNTER:
        return RH_STATUS_METRIC_TYPE_NOT_SUPPORTED_BY_BACKEND;
        break;
    case RALLY_HERE_METRIC_TYPE_GAUGE:
        break;
    case RALLY_HERE_METRIC_TYPE_TIMER:
        return RH_STATUS_METRIC_TYPE_NOT_SUPPORTED_BY_BACKEND;
        break;
    case RALLY_HERE_METRIC_TYPE_SET:
        return RH_STATUS_METRIC_TYPE_NOT_SUPPORTED_BY_BACKEND;
        break;
    }

    fmt::format_to(std::back_inserter(m_GraphiteBuffer), "{}", definition->name);
    add_graphite_tags(m_GraphiteBuffer, definition->label_names, definition->label_values);
    add_graphite_tags(m_GraphiteBuffer, action->label_names, action->label_values);
    switch (action->type)
    {
    case RALLY_HERE_METRIC_ACTION_INCREMENT:
        return RH_STATUS_METRIC_ACTION_NOT_SUPPORTED_BY_BACKEND;
        break;
    case RALLY_HERE_METRIC_ACTION_INCREMENT_WITH_SAMPLE:
        return RH_STATUS_METRIC_ACTION_NOT_SUPPORTED_BY_BACKEND;
        break;
    case RALLY_HERE_METRIC_ACTION_SET:
        fmt::format_to(std::back_inserter(m_GraphiteBuffer), " {}", action->action.increment.change);
        break;
    case RALLY_HERE_METRIC_ACTION_SET_WITH_SAMPLE:
        // Just ignore the sample rate and still send the value
        fmt::format_to(std::back_inserter(m_GraphiteBuffer), " {}", action->action.increment_with_sample.change);
        break;
    case RALLY_HERE_METRIC_ACTION_UNIQUE:
        return RH_STATUS_METRIC_ACTION_NOT_SUPPORTED_BY_BACKEND;
        break;
    }

    if (timestamp)
    {
        fmt::format_to(std::back_inserter(m_GraphiteBuffer), " {}\n", static_cast<uint64_t>(std::floor(*timestamp)));
    }
    else
    {
        fmt::format_to(std::back_inserter(m_GraphiteBuffer), " -1\n");
    }

    if (flush == RALLY_HERE_METRIC_FLUSH)
    {
        m_GraphiteTmpOutput = { m_GraphiteBuffer.data(), m_GraphiteBuffer.size() };
        m_GraphiteBuffer.clear();
    }

    return RH_STATUS_OK;
}


} // namespace rallyhere
