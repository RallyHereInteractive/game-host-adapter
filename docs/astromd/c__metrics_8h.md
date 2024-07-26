---
title: c_metrics.h File Reference
---

An interface for structuring metrics in a way that can be used either with statsd or graphite 
## Summary
| Kind | View | Description |
|------|------|-------------|
|struct|[RallyHereMetricActionIncrementWithSample](/game-host-adapter/structrallyheremetricactionincrementwithsample/#structRallyHereMetricActionIncrementWithSample)||
|struct|[RallyHereMetricActionSetWithSample](/game-host-adapter/structrallyheremetricactionsetwithsample/#structRallyHereMetricActionSetWithSample)||
|struct|[RallyHereMetricActionIncrement](/game-host-adapter/structrallyheremetricactionincrement/#structRallyHereMetricActionIncrement)||
|struct|[RallyHereMetricActionSet](/game-host-adapter/structrallyheremetricactionset/#structRallyHereMetricActionSet)||
|struct|[RallyHereMetricActionUnique](/game-host-adapter/structrallyheremetricactionunique/#structRallyHereMetricActionUnique)||
|struct|[RallyHereMetricAction](/game-host-adapter/structrallyheremetricaction/#structRallyHereMetricAction)||
|struct|[RallyHereMetricCheckpoint](/game-host-adapter/structrallyheremetriccheckpoint/#structRallyHereMetricCheckpoint)||
|struct|[RallyHereMetricDefinition](/game-host-adapter/structrallyheremetricdefinition/#structRallyHereMetricDefinition)||
|enum|[RallyHereMetricType](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1a5d296b8f5371e7f66d218ab96ca9618f)||
|enum|[RallyHereMetricActionType](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1ab8924252bdca9e75368c0256ad21b280)||
|enum|[RallyHereMetricFlush](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa)||
|typedef|[RallyHereMetricType](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1aa89a9a2cad39bc67edf0a86c58ab7f75)||
|typedef|[RallyHereMetricActionType](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1a0f584bba073df6feb1e7492408cd7a39)||
|typedef|[RallyHereMetricFlush](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1ae1518b512f4ad23fbb6291253b0bb068)||
|typedef|[RallyHereMetricRegistryPtr](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1ad652760d0664a541a9dde797e61da7fd)||
## Enums




### `RallyHereMetricType` <a id="c__metrics_8h_1a5d296b8f5371e7f66d218ab96ca9618f"></a>




| Enumerator | Initializer|
|------------|------------|
|`RALLY_HERE_METRIC_TYPE_COUNTER`|``|
|`RALLY_HERE_METRIC_TYPE_GAUGE`|``|
|`RALLY_HERE_METRIC_TYPE_TIMER`|``|
|`RALLY_HERE_METRIC_TYPE_SET`|``|



### `RallyHereMetricActionType` <a id="c__metrics_8h_1ab8924252bdca9e75368c0256ad21b280"></a>




| Enumerator | Initializer|
|------------|------------|
|`RALLY_HERE_METRIC_ACTION_INCREMENT`|``|
|`RALLY_HERE_METRIC_ACTION_INCREMENT_WITH_SAMPLE`|``|
|`RALLY_HERE_METRIC_ACTION_SET`|``|
|`RALLY_HERE_METRIC_ACTION_SET_WITH_SAMPLE`|``|
|`RALLY_HERE_METRIC_ACTION_UNIQUE`|``|



### `RallyHereMetricFlush` <a id="c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa"></a>




| Enumerator | Initializer|
|------------|------------|
|`RALLY_HERE_METRIC_FLUSH`|``|
|`RALLY_HERE_METRIC_NO_FLUSH`|``|



## Typedefs



### `RallyHereMetricType` <a id="c__metrics_8h_1aa89a9a2cad39bc67edf0a86c58ab7f75"></a>

typedef enum [RallyHereMetricType](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1a5d296b8f5371e7f66d218ab96ca9618f) RallyHereMetricType




### `RallyHereMetricActionType` <a id="c__metrics_8h_1a0f584bba073df6feb1e7492408cd7a39"></a>

typedef enum [RallyHereMetricActionType](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1ab8924252bdca9e75368c0256ad21b280) RallyHereMetricActionType




### `RallyHereMetricFlush` <a id="c__metrics_8h_1ae1518b512f4ad23fbb6291253b0bb068"></a>

typedef enum [RallyHereMetricFlush](/game-host-adapter/c__metrics_8h/#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa) RallyHereMetricFlush




### `RallyHereMetricRegistryPtr` <a id="c__metrics_8h_1ad652760d0664a541a9dde797e61da7fd"></a>

typedef RallyHereMetricRegistry * RallyHereMetricRegistryPtr





