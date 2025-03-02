// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.ExceptionType.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.ErrorCode.hpp"
#include "Futurewalker.Core.BuildConfig.hpp"
#include "Futurewalker.Core.Stacktrace.hpp"

#include <source_location>
#include <string>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Exception class.
///
class Exception final : public std::exception
{
public:
    Exception(
      const ErrorCode errorCode = ErrorCode::Failure,
      const std::string_view message = std::string_view(),
      const std::source_location location = BuildConfig::IsDebug() ? std::source_location::current() : std::source_location(),
      const Stacktrace stacktrace = BuildConfig::IsDebug() ? Stacktrace::MakeCurrent() : Stacktrace());

    auto GetErrorCode() const -> ErrorCode;
    auto GetSourceLocation() const -> std::source_location;
    auto GetStacktrace() const -> Stacktrace;

public:
    auto what() const noexcept -> const char* override;

private:
    ErrorCode _errorCode;
    std::source_location _location;
    Stacktrace _stacktrace;
    std::string _what;
};
}
}