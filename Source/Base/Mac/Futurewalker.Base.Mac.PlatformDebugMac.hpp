// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.Mac.Prelude.hpp"
#include "Futurewalker.Base.PlatformDebug.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformDebugMac : public PlatformDebug
{
public:
    auto Print(StringView const message) -> void override;
    auto Break() -> void override;
};

template <>
struct Locator::Resolver<PlatformDebugMac>
{
    using Interface = PlatformDebug;
    static auto Resolve() -> Shared<PlatformDebugMac>;
};
}
}
