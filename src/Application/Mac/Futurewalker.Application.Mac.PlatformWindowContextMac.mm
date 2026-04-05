// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformWindowContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformWindowMac.hpp"
#include "Futurewalker.Application.Mac.PlatformViewLayerVisualContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformInputMethodContextMac.hpp"

namespace FW_DETAIL_NS
{
auto PlatformWindowContextMac::Make(Shared<PlatformInputMethodContext> inputMethodContext) -> Shared<PlatformWindowContextMac>
{
    auto context = Shared<PlatformWindowContextMac>::Make(PassKey<PlatformWindowContextMac>(), inputMethodContext);
    context->_self = context;
    return context;
}

PlatformWindowContextMac::PlatformWindowContextMac(PassKey<PlatformWindowContextMac>, Shared<PlatformInputMethodContext> inputMethodContext)
  : _inputMethodContext(inputMethodContext)
{
    _visualContext = PlatformViewLayerVisualContextMac::Make();
}

auto PlatformWindowContextMac::GetSelf() -> Shared<PlatformWindowContextMac>
{
    return _self.Lock();
}

auto PlatformWindowContextMac::MakePlatformWindow(PlatformWindowOptions const& options, PlatformWindow::Delegate const& delegate) -> Shared<PlatformWindow>
{
    return PlatformWindowMac::Make(GetSelf(), _visualContext, options, delegate);
}

auto Locator::Resolver<PlatformWindowContextMac>::Resolve() -> Shared<PlatformWindowContextMac>
{
    auto inputMethodContext = Locator::Resolve<PlatformInputMethodContext>().As<PlatformInputMethodContextMac>();
    return PlatformWindowContextMac::Make(inputMethodContext);
}
}
