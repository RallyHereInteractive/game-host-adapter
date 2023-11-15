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

#pragma once

/* i3D.net ONE Game Hosting SDK
 * @copyright Apache-2.0
 */

#include "c_platform.h"
#include <assert.h>
#include <functional>

namespace i3d {
namespace one {

namespace allocator {

// Override alloc, which is used by all allocation functions in this
// namespace. Default is ::operator new.
void set_alloc(std::function<void *(std::size_t)>);

// Override free, which is used by all deallocation functions in this
// namespace. Default is ::operator delete.
void set_free(std::function<void(void *)>);

// Override realloc, which is used by the realloc function in this
// namespace. Default is ::operator delete.
void set_realloc(std::function<void *(void *, std::size_t)>);

// Sets the allocators back to the default.
void reset_overrides();

// Use the function set by set_alloc to allocate memory.
RH_CPP_EXPORT void *alloc(std::size_t);

// Use the function set by set_free to free memory.
RH_CPP_EXPORT void free(void *);

// Reallocate existing memory with new size. Must match standard c behavior.
void *realloc(void *p, std::size_t s);

// Equivalent to the new operator, but using the function set by set_alloc.
template <class T, class... Args>
RH_CPP_EXPORT T *create(Args &&... args) {
    auto data = alloc(sizeof(T));
    auto p = reinterpret_cast<T *>(data);
    ::new (p) T(std::forward<Args>(args)...);
    return p;
}

// Equivalent to the delete operator, but using the function set by set_free.
template <class T>
RH_CPP_EXPORT void destroy(T *p) noexcept {
    p->~T();
    i3d::one::allocator::free(p);
}

template <class T>
struct destroyer
{
    void operator()(T *p) noexcept {
        destroy(p);
    }
};

// Allocates an array using the function set by set_alloc. Each array element
// has its constructor called, with the same arguments. The returned memory must
// be deleted via destroy_array.
template <class T, class... Args>
T *create_array(std::size_t count, Args &&... args) {
    // Allocate additional space to store the array length.
    std::size_t padding = sizeof(std::size_t);
    auto data = alloc(sizeof(T) * count + padding);

    // The padding will be at the front of the data. Set it to the array length.
    std::size_t *start = reinterpret_cast<std::size_t *>(data);
    *start = count;

    // The actual array elements start directly after the padding.
    auto p = reinterpret_cast<T *>(start + 1);
    auto element = p;
    for (std::size_t i = 0; i < count; ++i) {
        ::new (element) T(std::forward<Args>(args)...);
        ++element;
    }

    return p;
}

// Deallocates an array using the function set by set_free. Each array element
// has its destructor called. The destroyed memory must have been allocated via
// create_array.
template <class T>
void destroy_array(T *p) noexcept {
    assert(p);

    // The buffer allocated will start before the array length preceding the
    // array elements (p).
    std::size_t *start = (reinterpret_cast<std::size_t *>(p) - 1);
    // The array length is first.
    std::size_t count = *start;

    // Call destructors.
    T *element = reinterpret_cast<T *>(p);
    for (std::size_t i = 0; i < count; ++i) {
        element->~T();
        ++element;
    }

    // Free the original buffer including array length.
    free(start);
}

}  // namespace allocator

// Custom c++ 11 std::allocator to use for all std types.
template <class T>
class StandardAllocator {
public:
    using value_type = T;

    StandardAllocator() noexcept {}
    template <class U>
    StandardAllocator(StandardAllocator<U> const &other) noexcept {}

    value_type *  // Use pointer if pointer is not a value_type*
    allocate(std::size_t n) {
        return static_cast<value_type *>(allocator::alloc(n * sizeof(value_type)));
    }

    RH_CPP_EXPORT void deallocate(value_type *p,
                                  std::size_t) noexcept  // Use pointer if pointer is not a value_type*
    {
        allocator::free(p);
    }
};

template <class T, class U>
bool operator==(StandardAllocator<T> const &, StandardAllocator<U> const &) noexcept {
    return true;
}

template <class T, class U>
bool operator!=(StandardAllocator<T> const &x, StandardAllocator<U> const &y) noexcept {
    return !(x == y);
}

}  // namespace one
}  // namespace i3d
