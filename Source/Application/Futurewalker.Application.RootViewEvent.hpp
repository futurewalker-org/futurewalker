// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ViewLayerType.hpp"
#include "Futurewalker.Application.InputMethodType.hpp"

#include "Futurewalker.Event.EventType.hpp"
#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Attribute.AttributeNodeType.hpp"

#include "Futurewalker.Action.CommandNodeType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief RootView event.
///
class RootViewEvent : public EventParameter
{
public:
    RootViewEvent() = default;
    RootViewEvent(RootViewEvent const&) = default;
    RootViewEvent& operator=(RootViewEvent const&) = default;

public:
    class Owner;
    class Resize;
    class Frame;
    class Pointer;
    class Key;
    class Input;
    class HitTest;
};

class RootViewEvent::Owner final : public RootViewEvent
{
public:
    auto GetAttached() const -> Bool;
    auto SetAttached(Bool const attached) -> void;

    auto GetActive() const -> Bool;
    auto SetActive(Bool const active) -> void;

    auto GetDisplayScale() const -> DisplayScale;
    auto SetDisplayScale(DisplayScale const displayScale) -> void;

    auto GetBackingScale() const -> BackingScale;
    auto SetBackingScale(BackingScale const backingScale) -> void;

    auto GetParentLayer() const -> Shared<ViewLayer>;
    auto SetParentLayer(Shared<ViewLayer> parentLayer) -> void;

    auto GetParentAttributeNode() const -> Shared<AttributeNode>;
    auto SetParentAttributeNode(Shared<AttributeNode> parentAttributeNode) -> void;

    auto GetParentCommandNode() const -> Shared<CommandNode>;
    auto SetParentCommandNode(Shared<CommandNode> parentCommandNode) -> void;

    auto GetInputMethod() const -> Shared<InputMethod>;
    auto SetInputMethod(Shared<InputMethod> const& inputMethod) -> void;

private:
    Bool _attached = false;
    Bool _active = false;
    DisplayScale _displayScale = 1.0;
    BackingScale _backingScale = 1.0;
    Shared<ViewLayer> _parentLayer;
    Shared<AttributeNode> _parentAttributeNode;
    Shared<CommandNode> _parentCommandNode;
    Shared<InputMethod> _inputMethod;
};

class RootViewEvent::Resize final : public RootViewEvent
{
public:
    auto GetSize() const -> Size<Dp>;
    auto SetSize(Size<Dp> const& size) -> void;

private:
    Size<Dp> _size;
};

class RootViewEvent::Frame final : public RootViewEvent
{
public:
};

class RootViewEvent::Pointer final : public RootViewEvent
{
public:
    Pointer(Event<>& event);

    auto GetEvent() -> Event<>&;

private:
    Event<>& _event;
};

class RootViewEvent::Key final : public RootViewEvent
{
public:
    Key(Event<>& event);

    auto GetEvent() -> Event<>&;

private:
    Event<>& _event;
};

class RootViewEvent::Input final : public RootViewEvent
{
public:
    Input(Event<>& event);

    auto GetEvent() -> Event<>&;

private:
    Event<>& _event;
};

class RootViewEvent::HitTest final : public RootViewEvent
{
public:
    HitTest(Event<>& event);

    auto GetEvent() -> Event<>&;

private:
    Event<>& _event;
};
}
}
