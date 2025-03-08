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
    template <class... Args>
    static inline auto Print(char const* message, Args&&... args) -> void;
    static inline auto Assert(Bool const boolean, char const* message = "", std::source_location location = std::source_location::current(), Stacktrace stacktrace = Stacktrace::MakeCurrent()) -> void;
    static inline auto Break() -> void;

private:
    static auto PrintPriv(StringView const message) -> void;
    static auto BreakPriv() -> void;
};

///
/// @brief Print formatted string to debug output.
///
/// @tparam Args Variadic types of format arguments.
///
/// @param[in] message ASCII format string to print.
/// @param[in] args Format arguments.
///
template <class... Args>
inline auto Debug::Print(char const* message, Args&&... args) -> void
{
    Debug::PrintPriv(StringFunction::Format(StringFunction::ConvertUtf8ToStringUnchecked(message), std::forward<Args>(args)...));
}

///
/// @brief Assert result of given function in debug mode.
///
/// @tparam F A type of assertion functor.
///
/// @param[in] boolean Assertion function. Must return boolean-testable value.
/// @param[in] message ASCII assertion message.
/// @param[in] location Source location of caller.
/// @param[in] stacktrace  stacktrace  stacktrace  stacktrace 
///
inline auto Debug::Assert(Bool const boolean, char const* message, std::source_location location, Stacktrace stacktrace) -> void
{
    if (!boolean)
    {
        auto const function = StringFunction::ConvertUtf8ToStringUnchecked(location.function_name());
        auto const file = StringFunction::ConvertUtf8ToStringUnchecked(location.file_name());
        auto const trace = StringFunction::ConvertUtf8ToStringUnchecked(stacktrace.GetCString());
        Debug::Print("Assertion failed: {} {} {}:{}:{}", StringFunction::ConvertUtf8ToStringUnchecked(message), function, file, location.line(), location.column());
        Debug::Print("{}", trace);
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
