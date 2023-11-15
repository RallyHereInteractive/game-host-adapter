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
#ifndef CUSTOM_RESOURCE_H
#define CUSTOM_RESOURCE_H

#include "c_platform.h"
#include "boost/json.hpp"
#include "allocator.h"
#include "boost/json/monotonic_resource.hpp"


namespace rallyhere
{
class custom_resource : public boost::json::memory_resource
{
  public:
    custom_resource() noexcept = default;
    ~custom_resource() = default;

    void* do_allocate(std::size_t n, std::size_t) override
    {
        return i3d::one::StandardAllocator<char>{}.allocate(n);
    }

    void do_deallocate(void* p, std::size_t n, std::size_t) override
    {
        i3d::one::StandardAllocator<char>{}.deallocate(static_cast<char*>(p), n);
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        return this == &other;
    }
};
} // namespace rallyhere
#endif // CUSTOM_RESOURCE_H