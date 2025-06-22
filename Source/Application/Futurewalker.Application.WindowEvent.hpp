// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.WindowEventType.hpp"

#include "Futurewalker.Event.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Application event.
///
class WindowEvent : public EventParameter
{
public:
    WindowEvent() = default;
    WindowEvent(WindowEvent const&) = default;
    WindowEvent& operator=(WindowEvent const&) = default;

public:
    class VisibleChanged;
    class ActiveChanged;
    class FocusedChanged;
    class PositionChanged;
    class SizeChanged;
    class CloseRequested;
    class Closing;
    class Closed;
    class AreaChanged;
};

///
/// @brief
///
class WindowEvent::VisibleChanged final : public WindowEvent
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
class WindowEvent::ActiveChanged final : public WindowEvent
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
class WindowEvent::FocusedChanged final : public WindowEvent
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
class WindowEvent::SizeChanged final : public WindowEvent
{
public:
    auto GetSize() const -> Size<Dp>;
    auto SetSize(Size<Dp> const& size) -> void;

private:
    Size<Dp> _size;
};

///
/// @brief
///
class WindowEvent::PositionChanged final : public WindowEvent
{
public:
    auto GetPosition() const -> Point<Dp>;
    auto SetPosition(Point<Dp> const& position) -> void;

private:
    Point<Dp> _position;
};

///
/// @brief
///
class WindowEvent::CloseRequested final : public WindowEvent
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
class WindowEvent::Closed final : public WindowEvent
{
};

///
/// @brief
///
class WindowEvent::AreaChanged final : public WindowEvent
{
};
}
}
