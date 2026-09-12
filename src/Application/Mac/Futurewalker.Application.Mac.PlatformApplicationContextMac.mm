// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformApplicationContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformApplicationMac.hpp"
#include "Futurewalker.Application.Mac.PlatformScreenContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformWindowContextMac.hpp"

#include "Futurewalker.Base.Debug.hpp"

@interface PlatformApplicationContextMacDelegate : NSObject
@property(assign, nonatomic) void* data;
@property(assign, nonatomic) void (*callbackOnScreenParametersChange)(void*);
@end
@implementation PlatformApplicationContextMacDelegate
- (instancetype)init
{
    if (self = [super init])
    {
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidChangeScreenParameters:) name:NSApplicationDidChangeScreenParametersNotification object:nil];
    }
    return self;
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:NSApplicationDidChangeScreenParametersNotification object:nil];
}

- (void)applicationDidChangeScreenParameters:(NSNotification*)notification
{
    if (self.callbackOnScreenParametersChange)
    {
        self.callbackOnScreenParametersChange(self.data);
    }
}
@end

namespace FW_DETAIL_NS
{
auto PlatformApplicationContextMac::Make(Shared<PlatformMainThread> mainThread, Shared<PlatformWindowContext> windowContext, Shared<PlatformScreenContext> screenContext) -> Shared<PlatformApplicationContextMac>
{
    auto const key = PassKey<PlatformApplicationContextMac>();
    auto context = Shared<PlatformApplicationContextMac>::Make(key, mainThread, windowContext, screenContext);
    context->_self = context;
    return context;
}

PlatformApplicationContextMac::PlatformApplicationContextMac(
  PassKey<PlatformApplicationContextMac>,
  Shared<PlatformMainThread> mainThread,
  Shared<PlatformWindowContext> windowContext,
  Shared<PlatformScreenContext> screenContext)
  : PlatformApplicationContext()
  , _mainThread {mainThread}
  , _windowContext {windowContext}
  , _screenContext {screenContext}
{
    @autoreleasepool
    {
        [NSApplication sharedApplication];

        _delegate = [PlatformApplicationContextMacDelegate new];
        _delegate.data = this;
        _delegate.callbackOnScreenParametersChange = [](void* data) {
            auto self = static_cast<PlatformApplicationContextMac*>(data);
            self->_screenContext->Refresh();
        };
    }
    _screenContext->Refresh();
}

PlatformApplicationContextMac::~PlatformApplicationContextMac()
{
    _delegate.data = nullptr;
    _delegate.callbackOnScreenParametersChange = nullptr;
    _delegate = nil;
}

auto PlatformApplicationContextMac::MakeApplication(PlatformApplication::Delegate delegate) -> Shared<PlatformApplication>
{
    if (_currentApplication.IsExpired())
    {
        auto application = PlatformApplicationMac::Make(delegate, _self.Lock());
        _currentApplication = application;
        return application;
    }
    throw Exception(ErrorCode::InvalidOperation, "Attempted to create multiple Application instances");
}

auto PlatformApplicationContextMac::GetCurrentApplication() -> Shared<PlatformApplication>
{
    return _currentApplication.Lock();
}

auto PlatformApplicationContextMac::RunApplication(Shared<PlatformApplication> app, Function<void()> cleanup) -> Async<void>
{
    auto appMac = app.TryAs<PlatformApplicationMac>();
    if (!appMac || appMac != _currentApplication.Lock())
    {
        FW_DEBUG_ASSERT(false);
        throw Exception(ErrorCode::InvalidOperation, "Application instance is not valid");
    }

    if (!MainThread::IsMainThread())
    {
        FW_DEBUG_ASSERT(false);
        throw Exception(ErrorCode::InvalidOperation, "Application must run on main thread");
    }
    co_return co_await appMac->Run(cleanup);
}

auto PlatformApplicationContextMac::Schedule() -> AsyncTask<void>
{
    co_await _mainThread->Schedule();
}

auto PlatformApplicationContextMac::ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>
{
    co_await _mainThread->ScheduleAfter(delay);
}

auto PlatformApplicationContextMac::SetActive(Bool const active) -> void
{
    (void)active; // TODO
}

auto Locator::Resolver<PlatformApplicationContextMac>::Resolve() -> Shared<PlatformApplicationContextMac>
{
    auto mainThread = Locator::Resolve<PlatformMainThread>();
    auto windowContext = Locator::Resolve<PlatformWindowContext>();
    auto screenContext = Locator::Resolve<PlatformScreenContext>();
    return PlatformApplicationContextMac::Make(mainThread, windowContext, screenContext);
}
}
