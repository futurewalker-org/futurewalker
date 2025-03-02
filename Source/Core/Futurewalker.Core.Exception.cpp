// SPDX-License-Identifier: MIT
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.StringFunction.hpp"

#include <format>

namespace FW_DETAIL_NS
{
///
/// @brief Initialize exception.
///
/// @param errorCode Error code.
/// @param message ASCII string which represents exception message.
/// @param location Source location.
/// @param stacktrace Stacktrace.
///
Exception::Exception(const ErrorCode errorCode, const std::string_view message, const std::source_location location, const Stacktrace stacktrace)
  : exception()
  , _errorCode {errorCode}
  , _location {location}
  , _stacktrace {stacktrace}
{
    const auto fileName = location.file_name();
    const auto functionName = location.function_name();
    const auto stacktraceString = _stacktrace.GetString();
    _what = std::format("{}({},{}) {}: code={} {}\n{}", fileName, location.line(), location.column(), functionName, std::to_underlying(errorCode), message, stacktraceString);
}

///
/// @brief Get error code.
///
auto Exception::GetErrorCode() const -> ErrorCode
{
    return _errorCode;
}

///
/// @brief Get source location.
///
auto Exception::GetSourceLocation() const -> std::source_location
{
    return _location;
}

///
/// @brief Get stacktrace.
///
auto Exception::GetStacktrace() const -> Stacktrace
{
    return _stacktrace;
}

///
/// @brief Returns the explanatory string.
///
auto Exception::what() const noexcept -> const char*
{
    return _what.c_str();
}
}
