// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.Prelude.hpp"

#if FW_ENABLE_DEBUG
    #define FW_DEBUG_ASSERT(expr) Debug::Assert(!!(expr), "" #expr)
    #define FW_DEBUG_PRINT(msg, ...) Debug::Print("" msg __VA_OPT__(, ) __VA_ARGS__)
    #define FW_DEBUG_BREAK() Debug::Break()
#else
    #define FW_DEBUG_ASSERT(expr)
    #define FW_DEBUG_PRINT(msg, ...)
    #define FW_DEBUG_BREAK()
#endif
