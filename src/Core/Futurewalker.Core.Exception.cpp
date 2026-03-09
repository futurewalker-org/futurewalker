// SPDX-License-Identifier: MPL-2.0
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
Exception::Exception(ErrorCode const errorCode, std::string_view const message, std::source_location const location, Stacktrace const stacktrace)
  : exception()
  , _errorCode {errorCode}
  , _location {location}
  , _stacktrace {stacktrace}
{
    auto const fileName = location.file_name();
    auto const functionName = location.function_name();
    auto const stacktraceString = _stacktrace.GetCString();
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
auto Exception::what() const noexcept -> char const*
{
    return _what.c_str();
}
}
