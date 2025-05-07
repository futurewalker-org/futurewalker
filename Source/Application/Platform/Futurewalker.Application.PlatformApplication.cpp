// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.PlatformApplication.hpp"

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
}
 