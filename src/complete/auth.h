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
#include "c_platform.h"
#include "status.h"

namespace rallyhere
{
namespace auth
{

    struct login_options
    {
        std::string_view grantType;
        std::string_view portalAccessToken;
        std::string_view portalDisplayName;
        std::string_view portalParentAccessToken;
        bool bAcceptEULA;
        bool bAcceptTOS;
        bool bAcceptPP;
    };

    struct login_callback_info
    {
        rallyhere::Status status;
        void* clientData;
        std::string_view accessToken;
        std::string_view refreshToken;
        std::string_view personId;
        bool needsToAcceptEULA;
        bool needsToAcceptTOS;
        bool needsToAcceptPP;
    };
} // namespace auth
} // namespace rallyhere