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
#include "c_status.h"
#include "status.h"

#ifdef __cplusplus
extern "C"
{
#endif

    bool rallyhere_is_error(RallyHereStatusCode err)
    {
        return rallyhere::is_error(err);
    }

    bool rallyhere_is_cancelled(RallyHereStatusCode err)
    {
        return rallyhere::is_cancelled(err);
    }

    const char* rallyhere_status_text(RallyHereStatusCode err)
    {
        return rallyhere::status_text(err);
    }

#ifdef __cplusplus
};
#endif
