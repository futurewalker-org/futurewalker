// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Signal.Tracker.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make Tracker reference.
///
auto Tracker::Make() -> Shared<Tracker>
{
    auto const tracker = Shared<Tracker>::Make(PassKey<Tracker>());
    tracker->SetSelf(tracker);
    return tracker;
}

///
/// @brief Constructor.
///
Tracker::Tracker(PassKey<Tracker>)
{
}

///
/// @brief Get Tracker reference.
///
auto Tracker::GetTracker() -> Tracker&
{
    return *this;
}

///
/// @brief Get Tracker reference.
///
auto Tracker::GetTracker() const -> Tracker const&
{
    return *this;
}

///
/// @brief Set self.
///
void Tracker::SetSelf(Weak<Tracker> self)
{
    _self = self;
}

///
/// @brief Get self.
///
auto Tracker::GetSelf() -> Shared<Tracker>
{
    return _self.Lock();
}

///
/// @brief Get self.
///
auto Tracker::GetSelf() const -> Shared<Tracker const>
{
    return _self.Lock();
}
}
