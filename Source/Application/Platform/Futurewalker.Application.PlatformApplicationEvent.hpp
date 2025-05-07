// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformApplicationEventType.hpp"

#include "Futurewalker.Event.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Application event.
///
class PlatformApplicationEvent : public EventParameter
{
public:
    PlatformApplicationEvent() = default;
    PlatformApplicationEvent(PlatformApplicationEvent const&) = default;
    PlatformApplicationEvent& operator=(PlatformApplicationEvent const&) = default;

public:
    class Started;
    class Exiting;
    class Exited;
    class ActiveChanged;
    class ForegroundChanged;
};

///
/// @brief
///
class PlatformApplicationEvent::Started final : public PlatformApplicationEvent
{
};

///
/// @brief
///
class PlatformApplicationEvent::Exiting final : public PlatformApplicationEvent
{
};

///
/// @brief
///
class PlatformApplicationEvent::Exited final : public PlatformApplicationEvent
{
};

///
/// @brief
///
class PlatformApplicationEvent::ActiveChanged final : public PlatformApplicationEvent
{
public:
    auto IsActive() const -> Bool;
    auto SetActive(Bool const active) -> void;

private:
    Bool _active = false;
};

///
/// @brief
///
class PlatformApplicationEvent::ForegroundChanged final : public PlatformApplicationEvent
{
public:
    auto IsForeground() const -> Bool;
    auto SetForeground(Bool const foreground) -> void;

private:
    Bool _foreground = false;
};
}
}
