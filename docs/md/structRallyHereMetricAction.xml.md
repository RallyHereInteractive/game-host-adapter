# RallyHereMetricAction Struct



## Summary
| Kind | View | Description |
|------|------|-------------|
|[RallyHereMetricActionType](c__metrics_8h.xml.md#c__metrics_8h_1ab8924252bdca9e75368c0256ad21b280)|[type](structRallyHereMetricAction.xml.md#structRallyHereMetricAction_1a38eefef5aea9eb30a8a98858bbe1fd87)||
|const char *|[label_names](structRallyHereMetricAction.xml.md#structRallyHereMetricAction_1a7ea459c2b57aceae25b4a64fa7af2a87)||
|const char *|[label_values](structRallyHereMetricAction.xml.md#structRallyHereMetricAction_1a5f60337cbe56addbc1efcdf413b30941)||
|[RallyHereMetricActionIncrementWithSample](structRallyHereMetricActionIncrementWithSample.xml.md#structRallyHereMetricActionIncrementWithSample)|[increment_with_sample](structRallyHereMetricAction.xml.md#structRallyHereMetricAction_1acdd9b8ab052985f11f69479520d14ddc)||
|[RallyHereMetricActionSetWithSample](structRallyHereMetricActionSetWithSample.xml.md#structRallyHereMetricActionSetWithSample)|[set_with_sample](structRallyHereMetricAction.xml.md#structRallyHereMetricAction_1a953dc621717d4fd5dc88ac78a57e480d)||
|[RallyHereMetricActionIncrement](structRallyHereMetricActionIncrement.xml.md#structRallyHereMetricActionIncrement)|[increment](structRallyHereMetricAction.xml.md#structRallyHereMetricAction_1a3a2aca58050d0e13e79a6c245189150d)||
|[RallyHereMetricActionSet](structRallyHereMetricActionSet.xml.md#structRallyHereMetricActionSet)|[set](structRallyHereMetricAction.xml.md#structRallyHereMetricAction_1a2fde817163a71d14b17179557d0180b2)||
|[RallyHereMetricActionUnique](structRallyHereMetricActionUnique.xml.md#structRallyHereMetricActionUnique)|[unique](structRallyHereMetricAction.xml.md#structRallyHereMetricAction_1ac6be45ccd5a7a84afb51068b5399fd0b)||
|union RallyHereMetricAction::@0|[action](structRallyHereMetricAction.xml.md#structRallyHereMetricAction_1a6f65572b126b3aaf389916ff31580195)||
## Public-attrib



### `type` {#structRallyHereMetricAction_1a38eefef5aea9eb30a8a98858bbe1fd87}

`RallyHereMetricActionType RallyHereMetricAction::type`






### `label_names` {#structRallyHereMetricAction_1a7ea459c2b57aceae25b4a64fa7af2a87}

`const char* RallyHereMetricAction::label_names`



A list of label names, separated by \0, terminated by \0\0 



### `label_values` {#structRallyHereMetricAction_1a5f60337cbe56addbc1efcdf413b30941}

`const char* RallyHereMetricAction::label_values`



A list of label values, separated by \0, terminated by \0\0 



### `increment_with_sample` {#structRallyHereMetricAction_1acdd9b8ab052985f11f69479520d14ddc}

`RallyHereMetricActionIncrementWithSample RallyHereMetricAction::increment_with_sample`






### `set_with_sample` {#structRallyHereMetricAction_1a953dc621717d4fd5dc88ac78a57e480d}

`RallyHereMetricActionSetWithSample RallyHereMetricAction::set_with_sample`






### `increment` {#structRallyHereMetricAction_1a3a2aca58050d0e13e79a6c245189150d}

`RallyHereMetricActionIncrement RallyHereMetricAction::increment`






### `set` {#structRallyHereMetricAction_1a2fde817163a71d14b17179557d0180b2}

`RallyHereMetricActionSet RallyHereMetricAction::set`






### `unique` {#structRallyHereMetricAction_1ac6be45ccd5a7a84afb51068b5399fd0b}

`RallyHereMetricActionUnique RallyHereMetricAction::unique`






### `action` {#structRallyHereMetricAction_1a6f65572b126b3aaf389916ff31580195}

`union RallyHereMetricAction::@0 RallyHereMetricAction::action`



The action to take 




