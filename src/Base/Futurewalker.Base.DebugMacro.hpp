// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.Prelude.hpp"

#if FW_ENABLE_DEBUG
    #define FW_DEBUG_ASSERT(expr) do { if (!(expr)) Debug::Assert(false, #expr); } while (0)
    #define FW_DEBUG_PRINT(msg, ...) Debug::Print(std::format(msg __VA_OPT__(, ) __VA_ARGS__))
    #define FW_DEBUG_LOG_TRACE(msg, ...) Debug::Log(DebugLogLevel::Trace, std::format(msg __VA_OPT__(, ) __VA_ARGS__))
    #define FW_DEBUG_LOG_INFO(msg, ...) Debug::Log(DebugLogLevel::Info, std::format(msg __VA_OPT__(, ) __VA_ARGS__))
    #define FW_DEBUG_LOG_WARNING(msg, ...) Debug::Log(DebugLogLevel::Warning, std::format(msg __VA_OPT__(, ) __VA_ARGS__))
    #define FW_DEBUG_LOG_ERROR(msg, ...) Debug::Log(DebugLogLevel::Error, std::format(msg __VA_OPT__(, ) __VA_ARGS__))
    #define FW_DEBUG_BREAK() Debug::Break()
#else
    #define FW_DEBUG_ASSERT(expr)
    #define FW_DEBUG_PRINT(msg, ...)
    #define FW_DEBUG_LOG_TRACE(msg, ...)
    #define FW_DEBUG_LOG_INFO(msg, ...)
    #define FW_DEBUG_LOG_WARNING(msg, ...)
    #define FW_DEBUG_LOG_ERROR(msg, ...)
    #define FW_DEBUG_BREAK()
#endif
