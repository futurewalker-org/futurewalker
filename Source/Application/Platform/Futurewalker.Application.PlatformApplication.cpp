// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.PlatformApplication.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Event.Event.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
PlatformApplication::~PlatformApplication() = default;

///
/// @brief 
///
/// @param delegate 
///
PlatformApplication::PlatformApplication(Delegate const& delegate)
  : _delegate {delegate}
{
}

///
/// @brief
///
/// @param event
///
auto PlatformApplication::SendApplicationEvent(Event& event) -> Async<Bool>
{
    if (_delegate.sendApplicationEvent)
    {
        co_return co_await _delegate.sendApplicationEvent(event);
    }
    co_return false;
}

///
/// @brief
///
/// @param event
///
auto PlatformApplication::SendApplicationEventDetached(Event const& event) -> void
{
    AsyncFunction::SpawnFn([self = GetSelf(), e = event]() mutable -> Task<void> {
        try
        {
            co_await self->SendApplicationEvent(e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    });
}
}
