// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.WindowEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto WindowEvent::VisibleChanged::IsVisible() const -> Bool
{
    return _visible;
}

///
/// @brief
///
auto WindowEvent::VisibleChanged::SetVisible(Bool const visible) -> void
{
    _visible = visible;
}

///
/// @brief
///
auto WindowEvent::ActiveChanged::IsActive() const -> Bool
{
    return _active;
}

///
/// @brief
///
auto WindowEvent::ActiveChanged::SetActive(Bool const active) -> void
{
    _active = active;
}

///
/// @brief
///
auto WindowEvent::FocusedChanged::IsFocused() const -> Bool
{
    return _focused;
}

///
/// @brief
///
auto WindowEvent::FocusedChanged::SetFocused(Bool const focused) -> void
{
    _focused = focused;
}

///
/// @brief
///
auto WindowEvent::SizeChanged::GetSize() const -> Size<Vp>
{
    return _size;
}

///
/// @brief
///
auto WindowEvent::SizeChanged::SetSize(Size<Vp> const& size) -> void
{
    _size = size;
}

///
/// @brief
///
auto WindowEvent::PositionChanged::GetPosition() const -> Point<Vp>
{
    return _position;
}

///
/// @brief
///
auto WindowEvent::PositionChanged::SetPosition(Point<Vp> const& position) -> void
{
    _position = position;
}

///
/// @brief
///
auto WindowEvent::CloseRequested::IsCancelled() const -> Bool
{
    return _cancelled;
}

///
/// @brief
///
auto WindowEvent::CloseRequested::SetCancelled(Bool const cancelled) -> void
{
    _cancelled = cancelled;
}
}