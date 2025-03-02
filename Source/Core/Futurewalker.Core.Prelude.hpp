// SPDX-License-Identifier: MIT
#pragma once

#include <cstdint>
#include <limits>

#ifndef FW_DETAIL
    #define FW_DETAIL _d
#endif

#ifndef FW_EXPORT
    #define FW_EXPORT _e
#endif

#ifndef FW_NS
    #define FW_NS Futurewalker
#endif

#ifndef FW_DETAIL_NS
    #define FW_DETAIL_NS FW_NS::FW_DETAIL
#endif

#ifndef FW_ENABLE_DEBUG
    #ifdef _NDEBUG
        #define FW_ENABLE_DEBUG 0
    #else
        #define FW_ENABLE_DEBUG 1
    #endif
#endif

namespace FW_NS
{
namespace FW_DETAIL
{
namespace FW_EXPORT
{
using uint8_t = std::uint8_t;
using uint16_t = std::uint16_t;
using uint32_t = std::uint32_t;
using uint64_t = std::uint64_t;

using int8_t = std::int8_t;
using int16_t = std::int16_t;
using int32_t = std::int32_t;
using int64_t = std::int64_t;

using uintptr_t = std::uintptr_t;
using intptr_t = std::intptr_t;

// TODO: Use std::float32_t or std::float64_t when available.
// TODO: Add float16_t when available.
using float32_t = float;
using float64_t = double;

static_assert(std::numeric_limits<unsigned char>::digits == 8, "Requirement: char must be 8bit");
static_assert(sizeof(float32_t) == 4, "Requirement: float must be 32bit");
static_assert(sizeof(float64_t) == 8, "Requirement: double must be 64bit");
static_assert(std::numeric_limits<float32_t>::is_iec559, "Requirement: float must be IEEE754 format");
static_assert(std::numeric_limits<float64_t>::is_iec559, "Requirement: double must be IEEE754 format");
}
}
using namespace FW_DETAIL::FW_EXPORT;
}
