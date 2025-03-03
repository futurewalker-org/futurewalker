// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.Win.Prelude.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Instance handle.
///
class PlatformInstanceHandleWin : NonCopyable
{
public:
    PlatformInstanceHandleWin(HINSTANCE instanceHandle);

    auto GetInstanceHandle() const -> HINSTANCE;

private:
    HINSTANCE _instanceHandle = nullptr;
};

template <>
struct Locator::Resolver<PlatformInstanceHandleWin>
{
    using Interface = PlatformInstanceHandleWin;
    static auto Resolve(HINSTANCE instanceHandle = ::GetModuleHandleW(nullptr)) -> Shared<PlatformInstanceHandleWin>;
};
}
}
