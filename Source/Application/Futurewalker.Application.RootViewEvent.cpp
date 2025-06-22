// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.RootViewEvent.hpp"

namespace FW_DETAIL_NS
{
auto RootViewEvent::Owner::GetAttached() const -> Bool
{
    return _attached;
}

auto RootViewEvent::Owner::SetAttached(Bool const attached) -> void
{
    _attached = attached;
}

auto RootViewEvent::Owner::GetActive() const -> Bool
{
    return _active;
}

auto RootViewEvent::Owner::SetActive(Bool const active) -> void
{
    _active = active;
}

auto RootViewEvent::Owner::GetDisplayScale() const -> DisplayScale
{
    return _displayScale;
}

auto RootViewEvent::Owner::SetDisplayScale(DisplayScale const displayScale) -> void
{
    _displayScale = displayScale;
}

auto RootViewEvent::Owner::GetBackingScale() const -> BackingScale
{
    return _backingScale;
}

auto RootViewEvent::Owner::SetBackingScale(BackingScale const backingScale) -> void
{
    _backingScale = backingScale;
}

auto RootViewEvent::Owner::GetParentLayer() const -> Shared<ViewLayer>
{
    return _parentLayer;
}

auto RootViewEvent::Owner::SetParentLayer(Shared<ViewLayer> parentLayer) -> void
{
    _parentLayer = parentLayer;
}

auto RootViewEvent::Owner::GetParentAttributeNode() const -> Shared<AttributeNode>
{
    return _parentAttributeNode;
}

auto RootViewEvent::Owner::SetParentAttributeNode(Shared<AttributeNode> parentAttributeNode) -> void
{
    _parentAttributeNode = parentAttributeNode;
}

auto RootViewEvent::Owner::GetInputMethod() const -> Shared<InputMethod>
{
    return _inputMethod;
}

auto RootViewEvent::Owner::SetInputMethod(Shared<InputMethod> const& inputMethod) -> void
{
    _inputMethod = inputMethod;
}

auto RootViewEvent::Resize::GetSize() const -> Size<Dp>
{
    return _size;
}

auto RootViewEvent::Resize::SetSize(Size<Dp> const& size) -> void
{
    _size = size;
}

RootViewEvent::Pointer::Pointer(Event<>& event)
  : _event {event}
{
}

auto RootViewEvent::Pointer::GetEvent() -> Event<>&
{
    return _event;
}

RootViewEvent::Key::Key(Event<>& event)
  : _event {event}
{
}

auto RootViewEvent::Key::GetEvent() -> Event<>&
{
    return _event;
}

RootViewEvent::Input::Input(Event<>& event)
  : _event {event}
{
}

auto RootViewEvent::Input::GetEvent() -> Event<>&
{
    return _event;
}
}
