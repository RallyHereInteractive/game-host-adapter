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

#include "allocator.h"

/* i3D.net ONE Game Hosting SDK
 * @copyright Apache-2.0
 */

#include <assert.h>
#include <cstdlib>

namespace i3d {
namespace one {

namespace allocator {

namespace {

// Wrapper to allow assignment to the std::function.
void *default_alloc(std::size_t bytes) {
    return std::malloc(bytes);
}

// Wrapper to allow assignment to the std::function.
void default_free(void *p) {
    std::free(p);
}

void *default_realloc(void *p, std::size_t bytes) {
    return std::realloc(p, bytes);
}

}  // namespace

// Global allocation overridable functions.
std::function<void *(std::size_t)> _alloc = default_alloc;
std::function<void(void *)> _free = default_free;
std::function<void *(void *, std::size_t)> _realloc = default_realloc;

void set_alloc(std::function<void *(std::size_t)> fn) {
    _alloc = fn;
}

void set_free(std::function<void(void *)> fn) {
    _free = fn;
}

void set_realloc(std::function<void *(void *, std::size_t)> fn) {
    _realloc = fn;
}

void reset_overrides() {
    _alloc = default_alloc;
    _free = default_free;
    _realloc = default_realloc;
}

void *alloc(std::size_t bytes) {
    assert(_alloc);
    void *p = _alloc(bytes);
    assert(p != nullptr);
    return p;
}

void free(void *p) {
    assert(_free);
    _free(p);
}

void *realloc(void *p, std::size_t s) {
    return _realloc(p, s);
}

}  // namespace allocator

}  // namespace one
}  // namespace i3d
