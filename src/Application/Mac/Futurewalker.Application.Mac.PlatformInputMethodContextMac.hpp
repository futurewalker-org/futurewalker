// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformInputMethodContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformInputMethodContextMac : public PlatformInputMethodContext
{
public:
    auto MakeEditable(PlatformInputEditable::Delegate const& delegate) -> Shared<PlatformInputEditable> override;
};

template <>
struct Locator::Resolver<PlatformInputMethodContextMac>
{
    using Interface = PlatformInputMethodContext;
    static auto Resolve() -> Shared<PlatformInputMethodContextMac>;
};
}
}
