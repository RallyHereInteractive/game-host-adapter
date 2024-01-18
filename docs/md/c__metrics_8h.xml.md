# c_metrics.h File Reference



An interface for structuring metrics in a way that can be used either with statsd or graphite 
## Summary
| Kind | View | Description |
|------|------|-------------|
|struct|[RallyHereMetricActionIncrementWithSample](structRallyHereMetricActionIncrementWithSample.xml.md#structRallyHereMetricActionIncrementWithSample)||
|struct|[RallyHereMetricActionSetWithSample](structRallyHereMetricActionSetWithSample.xml.md#structRallyHereMetricActionSetWithSample)||
|struct|[RallyHereMetricActionIncrement](structRallyHereMetricActionIncrement.xml.md#structRallyHereMetricActionIncrement)||
|struct|[RallyHereMetricActionSet](structRallyHereMetricActionSet.xml.md#structRallyHereMetricActionSet)||
|struct|[RallyHereMetricActionUnique](structRallyHereMetricActionUnique.xml.md#structRallyHereMetricActionUnique)||
|struct|[RallyHereMetricAction](structRallyHereMetricAction.xml.md#structRallyHereMetricAction)||
|struct|[RallyHereMetricCheckpoint](structRallyHereMetricCheckpoint.xml.md#structRallyHereMetricCheckpoint)||
|struct|[RallyHereMetricDefinition](structRallyHereMetricDefinition.xml.md#structRallyHereMetricDefinition)||
|enum|[RallyHereMetricType](c__metrics_8h.xml.md#c__metrics_8h_1a5d296b8f5371e7f66d218ab96ca9618f)||
|enum|[RallyHereMetricActionType](c__metrics_8h.xml.md#c__metrics_8h_1ab8924252bdca9e75368c0256ad21b280)||
|enum|[RallyHereMetricFlush](c__metrics_8h.xml.md#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa)||
|typedef|[RallyHereMetricType](c__metrics_8h.xml.md#c__metrics_8h_1aa89a9a2cad39bc67edf0a86c58ab7f75)||
|typedef|[RallyHereMetricActionType](c__metrics_8h.xml.md#c__metrics_8h_1a0f584bba073df6feb1e7492408cd7a39)||
|typedef|[RallyHereMetricFlush](c__metrics_8h.xml.md#c__metrics_8h_1ae1518b512f4ad23fbb6291253b0bb068)||
|typedef|[RallyHereMetricRegistryPtr](c__metrics_8h.xml.md#c__metrics_8h_1ad652760d0664a541a9dde797e61da7fd)||
## Enum




### `RallyHereMetricType` {#c__metrics_8h_1a5d296b8f5371e7f66d218ab96ca9618f}




| Enumerator | Initializer|
|------------|------------|
|`RALLY_HERE_METRIC_TYPE_COUNTER`|``|
|`RALLY_HERE_METRIC_TYPE_GAUGE`|``|
|`RALLY_HERE_METRIC_TYPE_TIMER`|``|
|`RALLY_HERE_METRIC_TYPE_SET`|``|



### `RallyHereMetricActionType` {#c__metrics_8h_1ab8924252bdca9e75368c0256ad21b280}




| Enumerator | Initializer|
|------------|------------|
|`RALLY_HERE_METRIC_ACTION_INCREMENT`|``|
|`RALLY_HERE_METRIC_ACTION_INCREMENT_WITH_SAMPLE`|``|
|`RALLY_HERE_METRIC_ACTION_SET`|``|
|`RALLY_HERE_METRIC_ACTION_SET_WITH_SAMPLE`|``|
|`RALLY_HERE_METRIC_ACTION_UNIQUE`|``|



### `RallyHereMetricFlush` {#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa}




| Enumerator | Initializer|
|------------|------------|
|`RALLY_HERE_METRIC_FLUSH`|``|
|`RALLY_HERE_METRIC_NO_FLUSH`|``|



## Typedef



### `RallyHereMetricType` {#c__metrics_8h_1aa89a9a2cad39bc67edf0a86c58ab7f75}

typedef enum [RallyHereMetricType](c__metrics_8h.xml.md#c__metrics_8h_1a5d296b8f5371e7f66d218ab96ca9618f) RallyHereMetricType




### `RallyHereMetricActionType` {#c__metrics_8h_1a0f584bba073df6feb1e7492408cd7a39}

typedef enum [RallyHereMetricActionType](c__metrics_8h.xml.md#c__metrics_8h_1ab8924252bdca9e75368c0256ad21b280) RallyHereMetricActionType




### `RallyHereMetricFlush` {#c__metrics_8h_1ae1518b512f4ad23fbb6291253b0bb068}

typedef enum [RallyHereMetricFlush](c__metrics_8h.xml.md#c__metrics_8h_1a510cdf6b2232f28c4f667342930bcaaa) RallyHereMetricFlush




### `RallyHereMetricRegistryPtr` {#c__metrics_8h_1ad652760d0664a541a9dde797e61da7fd}

typedef RallyHereMetricRegistry * RallyHereMetricRegistryPtr





