// SPDX-License-Identifier: MPL-2.0

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
auto PlatformApplication::SendApplicationEvent(Event<>& event) -> Bool
{
    if (_delegate.sendApplicationEvent)
    {
        return _delegate.sendApplicationEvent(event);
    }
    return false;
}
}
