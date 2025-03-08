// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.Win.Prelude.hpp"
#include "Futurewalker.Base.PlatformDebug.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform debug printer.
///
class PlatformDebugWin : public PlatformDebug
{
public:
    auto Print(StringView const message) -> void override;
    auto Break() -> void override;
};

template <>
struct Locator::Resolver<PlatformDebugWin>
{
    using Interface = PlatformDebug;
    static auto Resolve() -> Shared<PlatformDebugWin>;
};
}
}
