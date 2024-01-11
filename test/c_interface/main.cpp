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
#include "lest/lest.hpp"
#include "c_api.h"
#include "configuration.h"
#include <cstring>

using namespace std;

struct dummy_struct
{
    int32_t first;
    int32_t second;
    int64_t third;
};

static bool did_alloc = false;

void *fake_alloc(unsigned int size)
{
    did_alloc = true;
    return std::malloc(size);
}

static bool did_free = false;

void fake_free(void *p) {
    did_free = true;
    std::free(p);
}

static bool did_realloc = false;

void *fake_realloc(void *p, unsigned int bytes) {
    did_realloc = true;
    return std::realloc(p, bytes);
}

lest::tests& specification()
{
    static lest::tests tests;
    return tests;
}

//@formatter:off
// clang-format off
const lest::test main_module[] =
{
    CASE("OK status code works")
    {
        RallyHereStatusCode ok{};
        auto c_msg = rallyhere_status_text(ok);
        EXPECT(0 == std::strcmp(c_msg, "RH_STATUS_OK"));
    },
    CASE("Custom allocator basics work")
    {
        rallyhere_allocator_set_alloc(fake_alloc);
        rallyhere_allocator_set_free(fake_free);
        rallyhere_allocator_set_realloc(fake_realloc);
        RallyHereStringMapPtr labels;
        EXPECT(rallyhere_string_map_create(&labels) == RH_STATUS_OK);
        rallyhere_string_map_destroy(labels);
        EXPECT(did_alloc == true);
    }
};
//@formatter:on
// clang-format on

MODULE( specification(), main_module );

int main( int argc, char * argv[] )
{
    handle_launch_parameters(argc, argv);
    putenv("HIREZ_SIC_REPORTED_PUBLIC_HOST=unknownhostname");
    return lest::run(specification(), 1, argv);
}