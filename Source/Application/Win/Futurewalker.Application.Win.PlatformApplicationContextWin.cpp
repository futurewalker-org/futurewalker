// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformApplicationContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationWin.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.Exception.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make application context.
///
/// @param[in] eventLoopContext Non-null event loop context.
/// @param[in] mainThread Non-null main thread context.
///
auto PlatformApplicationContextWin::Make(
  Shared<PlatformEventLoopContext> eventLoopContext,
  Shared<PlatformThreadRuntimeContextWin> mainThreadRuntimeContext,
  Shared<PlatformMainThread> mainThread,
  Shared<PlatformScreenContext> screenContext,
  Shared<PlatformWindowContext> windowContext) -> Shared<PlatformApplicationContextWin>
{
    auto key = PassKey<PlatformApplicationContextWin>();
    auto context = Shared<PlatformApplicationContextWin>::Make(key, eventLoopContext, mainThreadRuntimeContext, mainThread, screenContext, windowContext);
    context->_self = context;
    return context;
}

///
/// @brief Constructor.
///
/// @param[in] eventLoopContext Non-null event loop context.
/// @param[in] mainThread Non-null main thread context.
///
PlatformApplicationContextWin::PlatformApplicationContextWin(
  PassKey<PlatformApplicationContextWin>,
  Shared<PlatformEventLoopContext> eventLoopContext,
  Shared<PlatformThreadRuntimeContextWin> mainThreadRuntimeContext,
  Shared<PlatformMainThread> mainThread,
  Shared<PlatformScreenContext> screenContext,
  Shared<PlatformWindowContext> windowContext)
  : _eventLoopContext {eventLoopContext}
  , _mainThreadRuntimeContext {mainThreadRuntimeContext}
  , _mainThread {mainThread}
  , _screenContext {screenContext}
  , _windowContext {windowContext}
{
    if (!_mainThreadRuntimeContext || !_mainThread || !_eventLoopContext || !_screenContext || !_windowContext)
    {
        throw Exception(ErrorCode::InvalidArgument);
    }

    // TODO: Use mouse events directly.
    ::EnableMouseInPointer(TRUE);

    _screenContext->Refresh();
}

///
/// @brief
///
auto PlatformApplicationContextWin::MakePlatformApplication(PlatformApplication::Delegate delegate) -> Shared<PlatformApplication>
{
    if (_currentApplication.IsExpired())
    {
        auto eventLoop = _eventLoopContext->GetEventLoopForThread();
        auto mainThread = _mainThread;
        auto application = Shared<PlatformApplicationWin>::Make(delegate, _self.Lock(), eventLoop, mainThread);
        _currentApplication = application;
        return application;
    }
    throw Exception(ErrorCode::InvalidOperation, "Attempted to create multiple Application instances");
}

///
/// @brief
///
auto PlatformApplicationContextWin::GetCurrentApplication() -> Shared<PlatformApplicationWin>
{
    return _currentApplication.Lock();
}

///
/// @brief
///
auto Locator::Resolver<PlatformApplicationContextWin>::Resolve() -> Shared<PlatformApplicationContextWin>
{
    auto mainThreadRuntimeContext = Shared<PlatformThreadRuntimeContextWin>::Make(PlatformThreadRuntimeInitTypeWin::SingleThreaded);
    auto mainThread = Locator::Resolve<PlatformMainThread>();
    auto eventLoop = Locator::Resolve<PlatformEventLoopContext>();
    auto screenContext = Locator::Resolve<PlatformScreenContext>();
    auto windowContext = Locator::Resolve<PlatformWindowContext>();
    return PlatformApplicationContextWin::Make(eventLoop, mainThreadRuntimeContext, mainThread, screenContext, windowContext);
}
}
