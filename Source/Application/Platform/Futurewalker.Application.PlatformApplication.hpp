// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformApplicationType.hpp"

#include "Futurewalker.Async.Async.hpp" 

#include "Futurewalker.Event.EventType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Function.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Platform application.
///
class PlatformApplication : NonCopyable
{
public:
    struct Delegate
    {
        EventFunction sendApplicationEvent;
    };
    PlatformApplication(Delegate const& delegate);

    virtual ~PlatformApplication() = 0;
    virtual auto Run() -> Async<void> = 0;
    virtual auto Exit() -> void = 0;
    virtual auto IsRunning() -> Bool = 0;
    virtual auto IsActive() -> Bool = 0;
    virtual auto IsForeground() -> Bool = 0;

protected:
    auto SendApplicationEvent(Event& event) -> Async<Bool>;

private:
    Delegate _delegate;
};
}
