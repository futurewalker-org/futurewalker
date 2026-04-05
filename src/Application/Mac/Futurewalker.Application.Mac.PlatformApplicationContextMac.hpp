// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.Prelude.hpp"
#include "Futurewalker.Application.PlatformApplicationContext.hpp"
#include "Futurewalker.Application.PlatformMainThreadType.hpp"
#include "Futurewalker.Application.PlatformWindowContextType.hpp"
#include "Futurewalker.Application.PlatformScreenContextType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.PassKey.hpp"

@class PlatformApplicationContextMacDelegate;

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief macOS implementation of the application context.
///
class PlatformApplicationContextMac : public PlatformApplicationContext
{
public:
    static auto Make(Shared<PlatformMainThread> mainThread, Shared<PlatformWindowContext> windowContext, Shared<PlatformScreenContext> screenContext) -> Shared<PlatformApplicationContextMac>;

    PlatformApplicationContextMac(PassKey<PlatformApplicationContextMac>, Shared<PlatformMainThread> mainThread, Shared<PlatformWindowContext> windowContext, Shared<PlatformScreenContext> screenContext);

    ~PlatformApplicationContextMac();

    auto MakePlatformApplication(PlatformApplication::Delegate delegate) -> Shared<PlatformApplication> override;

    auto Schedule() -> AsyncTask<void>;
    auto ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>;

private:
    Weak<PlatformApplicationContextMac> _self;
    Shared<PlatformMainThread> _mainThread;
    Shared<PlatformWindowContext> _windowContext;
    Shared<PlatformScreenContext> _screenContext;
    __strong PlatformApplicationContextMacDelegate* _delegate = nil;
};

template <>
struct Locator::Resolver<PlatformApplicationContextMac>
{
    using Interface = PlatformApplicationContext;
    static auto Resolve() -> Shared<PlatformApplicationContextMac>;
};
}
}
