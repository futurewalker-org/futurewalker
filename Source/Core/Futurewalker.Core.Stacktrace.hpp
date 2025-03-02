// SPDX-License-Identifier: MIT
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
    Stacktrace(const Stacktrace& other);
    Stacktrace(Stacktrace&& other) noexcept;

    auto operator=(const Stacktrace& other) -> Stacktrace&;
    auto operator=(Stacktrace&& other) noexcept -> Stacktrace&;

    auto IsEmpty() const -> Bool;

    auto GetString() const -> std::string;

private:
    class Impl;
    Stacktrace(std::unique_ptr<Impl> impl);
    std::unique_ptr<Impl> _impl;
};
}
}