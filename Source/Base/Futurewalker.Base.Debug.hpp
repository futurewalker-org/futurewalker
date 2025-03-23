// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.DebugType.hpp"
#include "Futurewalker.Base.DebugMacro.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.StringFunction.hpp"
#include "Futurewalker.Core.BuildConfig.hpp"
#include "Futurewalker.Core.Stacktrace.hpp"

#include <cstdlib>
#include <source_location>
#include <format>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Debug utility functions.
///
class Debug : NonConstructible
{
public:
    static inline auto Print(std::string_view const message) -> void;
    static inline auto Log(DebugLogLevel const level, std::string_view const message) -> void;
    static inline auto Assert(Bool const boolean, char const* message = "", std::source_location location = std::source_location::current(), Stacktrace stacktrace = Stacktrace::MakeCurrent()) -> void;
    static inline auto Break() -> void;

private:
    static auto PrintPriv(StringView const message) -> void;
    static auto BreakPriv() -> void;
};

///
/// @brief Print string to debug output.
///
/// @param[in] message ASCII string to print
///
inline auto Debug::Print(std::string_view const message) -> void
{
    Debug::PrintPriv(StringFunction::ConvertASCIIToString(message));
}

///
/// @brief Log string to debug output.
///
/// @param[in] level Debug log level
/// @param[in] message ASCII string to print
///
inline auto Debug::Log(DebugLogLevel const level, std::string_view const message) -> void
{
    auto const levelString = [&]() -> char const* {
        switch (level)
        {
            case DebugLogLevel::Info:
                return "I";
            case DebugLogLevel::Warning:
                return "W";
            case DebugLogLevel::Error:
                return "E";
        }
        return nullptr;
    }();

    if (levelString)
    {
        auto const formattedString = std::format("[{}]: {}", levelString, message);
        Debug::PrintPriv(StringFunction::ConvertASCIIToString(formattedString));
    }
}

///
/// @brief Assert result of given function in debug mode.
///
/// @tparam F A type of assertion functor.
///
/// @param[in] boolean Assertion function. Must return boolean-testable value
/// @param[in] message ASCII assertion message
/// @param[in] location Source location of caller
/// @param[in] stacktrace Stacktrace of caller
///
inline auto Debug::Assert(Bool const boolean, char const* message, std::source_location location, Stacktrace stacktrace) -> void
{
    if (!boolean)
    {
        auto const function = location.function_name();
        auto const file = location.file_name();
        auto const trace = stacktrace.GetCString();
        Debug::Log(DebugLogLevel::Error, std::format("Assertion failed: {} {} {}:{}:{}", message, function, file, location.line(), location.column()));
        Debug::Log(DebugLogLevel::Error, std::format("Stacktrace:\n{}", trace));
        std::abort();
    }
}

///
/// @brief Inserts debug break.
///
inline auto Debug::Break() -> void
{
    Debug::BreakPriv();
}
}
}
