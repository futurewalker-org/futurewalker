// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Signal.TrackerType.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Tracker object.
///
class Tracker final : NonCopyable
{
public:
    static auto Make() -> Shared<Tracker>;

    template <class T>
    static auto Track(T& t) -> Weak<Tracker>;

    template <class T>
    static auto Track(const T& t) -> Weak<const Tracker>;

public:
    Tracker(PassKey<Tracker>);

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> const Tracker&;

private:
    void SetSelf(Weak<Tracker> self);
    auto GetSelf() -> Shared<Tracker>;
    auto GetSelf() const -> Shared<const Tracker>;

private:
    Weak<Tracker> _self;
};

///
/// @brief Get Weak from trackable reference.
///
/// @param[in] t Reference of trackable object.
///
template <class T>
auto Tracker::Track(T& t) -> Weak<Tracker>
{
    auto& tracker = t.GetTracker();
    return tracker.GetSelf();
}

///
/// @brief Get Weak from trackable reference.
///
/// @param[in] t Reference of trackable object.
///
template <class T>
auto Tracker::Track(const T& t) -> Weak<const Tracker>
{
    auto& tracker = t.GetTracker();
    return tracker.GetSelf();
}

}
}
