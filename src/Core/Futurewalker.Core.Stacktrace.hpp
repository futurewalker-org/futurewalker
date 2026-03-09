// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.StacktraceType.hpp"
#include "Futurewalker.Core.Primitive.hpp"

#include <memory>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Stacktrace class.
///
/// @note Opaque implementation to avoid including windows.h in header.
///
class Stacktrace final
{
public:
    static auto MakeCurrent() -> Stacktrace;

    Stacktrace() noexcept;
    ~Stacktrace() noexcept;
    Stacktrace(Stacktrace const& other);
    Stacktrace(Stacktrace&& other) noexcept;

    auto operator=(Stacktrace const& other) -> Stacktrace&;
    auto operator=(Stacktrace&& other) noexcept -> Stacktrace&;

    auto IsEmpty() const -> Bool;

    auto GetCString() const -> std::string;

private:
    class Impl;
    Stacktrace(std::unique_ptr<Impl> impl);
    std::unique_ptr<Impl> _impl;
};
}
}