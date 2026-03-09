// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.WindowType.hpp"

#include "Futurewalker.Event.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Window event.
///
class PlatformWindowEvent : public EventParameter
{
public:
    PlatformWindowEvent() = default;
    PlatformWindowEvent(PlatformWindowEvent const&) = default;
    PlatformWindowEvent& operator=(PlatformWindowEvent const&) = default;

public:
    class CloseRequested;
    class Closed;
    class DisplayScaleChanged;
    class BackingScaleChanged;
    class VisibleChanged;
    class ActiveChanged;
    class FocusedChanged;
    class SizeChanged;
    class PositionChanged;
    class AreaChanged;
};

///
/// @brief
///
class PlatformWindowEvent::CloseRequested final : public PlatformWindowEvent
{
public:
    auto IsCancelled() const -> Bool;
    auto SetCancelled(Bool const cancelled) -> void;

private:
    Bool _cancelled = false;
};

///
/// @brief
///
class PlatformWindowEvent::Closed final : public PlatformWindowEvent
{
};

///
/// @brief
///
class PlatformWindowEvent::DisplayScaleChanged final : public PlatformWindowEvent
{
};

///
/// @brief
///
class PlatformWindowEvent::BackingScaleChanged final : public PlatformWindowEvent
{
};

///
/// @brief
///
class PlatformWindowEvent::VisibleChanged final : public PlatformWindowEvent
{
public:
    auto IsVisible() const -> Bool;
    auto SetVisible(Bool const visible) -> void;

private:
    Bool _visible = false;
};

///
/// @brief
///
class PlatformWindowEvent::ActiveChanged final : public PlatformWindowEvent
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
class PlatformWindowEvent::FocusedChanged final : public PlatformWindowEvent
{
public:
    auto IsFocused() const -> Bool;
    auto SetFocused(Bool const focused) -> void;

private:
    Bool _focused = false;
};

///
/// @brief
///
class PlatformWindowEvent::SizeChanged final : public PlatformWindowEvent
{
};

///
/// @brief
///
class PlatformWindowEvent::PositionChanged final : public PlatformWindowEvent
{
};

///
/// @brief
///
class PlatformWindowEvent::AreaChanged final : public PlatformWindowEvent
{
};
}
}
