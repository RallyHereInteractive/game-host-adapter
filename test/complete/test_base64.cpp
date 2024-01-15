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
#include "test_base64.h"
#include "cpp-base64/base64.hpp"
#include "rh_string.h"

//@formatter:off
// clang-format off
static const lest::test module[] = {
    CASE("test vectors")
    {
        base64<rallyhere::string> encoder;
        using namespace std::literals;
        EXPECT(encoder.encode(""sv, false) == "");
        EXPECT(encoder.encode("f"sv, false) == "Zg==");
        EXPECT(encoder.encode("fo"sv, false) == "Zm8=");
        EXPECT(encoder.encode("foo"sv, false) == "Zm9v");
        EXPECT(encoder.encode("foob"sv, false) == "Zm9vYg==");
        EXPECT(encoder.encode("fooba"sv, false) == "Zm9vYmE=");
        EXPECT(encoder.encode("foobar"sv, false) == "Zm9vYmFy");
    },
};
//@formatter:on
// clang-format off

extern lest::tests& specification();

MODULE( specification(), module );