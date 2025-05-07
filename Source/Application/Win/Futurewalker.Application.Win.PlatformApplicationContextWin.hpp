// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.Prelude.hpp"
#include "Futurewalker.Application.PlatformApplicationContext.hpp"
#include "Futurewalker.Application.PlatformEventLoopContext.hpp"
#include "Futurewalker.Application.PlatformScreenContext.hpp"
#include "Futurewalker.Application.PlatformWindowContext.hpp"
#include "Futurewalker.Application.PlatformMainThread.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationWinType.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Win.PlatformThreadRuntimeContextWin.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class PlatformApplicationContextWin : public PlatformApplicationContext
{
public:
    static auto Make(
      Shared<PlatformEventLoopContext> eventLoopContext,
      Shared<PlatformThreadRuntimeContextWin> mainThreadRuntimeContext,
      Shared<PlatformMainThread> mainThread,
      Shared<PlatformScreenContext> screenContext,
      Shared<PlatformWindowContext> windowContext) -> Shared<PlatformApplicationContextWin>;

    PlatformApplicationContextWin(
      PassKey<PlatformApplicationContextWin>,
      Shared<PlatformEventLoopContext> eventLoopContext,
      Shared<PlatformThreadRuntimeContextWin> mainThreadRuntimeContext,
      Shared<PlatformMainThread> mainThread,
      Shared<PlatformScreenContext> screenContext,
      Shared<PlatformWindowContext> windowContext);

    auto MakePlatformApplication(PlatformApplication::Delegate delegate) -> Shared<PlatformApplication> override;

    auto GetCurrentApplication() -> Shared<PlatformApplicationWin>;

private:
    Weak<PlatformApplicationContextWin> _self;
    Shared<PlatformEventLoopContext> _eventLoopContext;
    Shared<PlatformThreadRuntimeContextWin> _mainThreadRuntimeContext;
    Shared<PlatformMainThread> _mainThread;
    Shared<PlatformScreenContext> _screenContext;
    Shared<PlatformWindowContext> _windowContext;
    Weak<PlatformApplicationWin> _currentApplication;
};

///
/// @brief
///
template <>
struct Locator::Resolver<PlatformApplicationContextWin>
{
    using Interface = PlatformApplicationContext;
    static auto Resolve() -> Shared<PlatformApplicationContextWin>;
};
}
}
