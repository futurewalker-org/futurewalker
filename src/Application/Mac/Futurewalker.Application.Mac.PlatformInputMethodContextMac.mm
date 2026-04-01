// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformInputMethodContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformInputEditableMac.hpp"

namespace FW_DETAIL_NS
{
auto PlatformInputMethodContextMac::MakeEditable(PlatformInputEditable::Delegate const& delegate) -> Shared<PlatformInputEditable>
{
    return PlatformInputEditableMac::Make(delegate);
}

auto Locator::Resolver<PlatformInputMethodContextMac>::Resolve() -> Shared<PlatformInputMethodContextMac>
{
    return Shared<PlatformInputMethodContextMac>::Make();
}
}
