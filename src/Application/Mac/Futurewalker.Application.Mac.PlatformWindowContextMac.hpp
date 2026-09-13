// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformWindowContextMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformViewLayerVisualContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformWindowMacType.hpp"
#include "Futurewalker.Application.PlatformWindowContext.hpp"
#include "Futurewalker.Application.PlatformInputMethodContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.PassKey.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief macOS implementation of the window context.
///
class PlatformWindowContextMac : public PlatformWindowContext
{
public:
    static auto Make(Shared<PlatformInputMethodContext> inputMethodContext) -> Shared<PlatformWindowContextMac>;

    PlatformWindowContextMac(PassKey<PlatformWindowContextMac>, Shared<PlatformInputMethodContext> inputMethodContext);

    auto MakePlatformWindow(PlatformWindowOptions const& options, PlatformWindow::Delegate const& delegate) -> Shared<PlatformWindow> override;

    auto SetApplicationActive(Bool const active) -> void;
    auto SetWindowActive(Shared<PlatformWindowMac> const& window, Bool const active) -> void;
    auto UpdateWindowLevel() -> void;

private:
    auto GetSelf() -> Shared<PlatformWindowContextMac>;

    auto AssignWindowLevelRecursive(Shared<PlatformWindowMac> const& window, NSWindowLevel const baseWindowLevel) -> NSWindowLevel;
    auto CollapseWindowLevelRecursive(Shared<PlatformWindowMac> const& window, NSWindowLevel const targetWindowLevel, NSInteger const ownerWindowNumber) -> void;

private:
    Weak<PlatformWindowContextMac> _self;
    Shared<PlatformInputMethodContext> _inputMethodContext;
    Shared<PlatformViewLayerVisualContextMac> _visualContext;
    std::vector<Weak<PlatformWindowMac>> _toplevelWindows;
    Bool _applicationActive = false;
};

template <>
struct Locator::Resolver<PlatformWindowContextMac>
{
    using Interface = PlatformWindowContext;
    static auto Resolve() -> Shared<PlatformWindowContextMac>;
};
}
}
