// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayer.hpp"

#include <ranges>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TestPlatformViewLayer : public PlatformViewLayer
{
public:
    Weak<PlatformViewLayer> _parent;
    PlatformViewLayerArray _children;
    Offset<Dp> _pos;
    Size<Dp> _size;
    ViewClipMode _clipMode = ViewClipMode::None;
    Float64 _opacity = 1.0;

    auto GetId() const -> PlatformViewLayerId
    {
        static auto id = UInt64(0U);
        return PlatformViewLayerId(++id);
    }

    static auto Make() -> Shared<TestPlatformViewLayer>
    {
        return PlatformViewLayer::MakeDerived<TestPlatformViewLayer>();
    }

    TestPlatformViewLayer(PassKey<PlatformViewLayer> key)
      : PlatformViewLayer(key)
    {
    }

    auto AddChild(Shared<PlatformViewLayer> child, Shared<PlatformViewLayer> after) -> void override
    {
        if (!child || std::ranges::find(_children, child) != _children.end())
        {
            return;
        }

        child->RemoveFromParent();

        auto const it = std::ranges::find(_children, after);
        _children.insert(it, child);
        child.As<TestPlatformViewLayer>()->_parent = GetSelf();
    }

    auto RemoveFromParent() -> void override
    {
        if (auto const parent = _parent.Lock().As<TestPlatformViewLayer>())
        {
            auto const it = std::ranges::find(parent->_children, Shared<PlatformViewLayer>(GetSelf()));
            if (it != parent->_children.end())
            {
                parent->_children.erase(it);
                _parent = {};
            }
        }
    }

    auto GetChildren() -> PlatformViewLayerArray override
    {
        return _children;
    }

    auto GetControl() -> Shared<PlatformViewLayerControl> override
    {
        return {};
    }

    auto SetOffset(Offset<Dp> const& pos) -> void override
    {
        _pos = pos;
    }

    auto SetSize(Size<Dp> const& size) -> void override
    {
        _size = size;
    }

    auto SetClipMode(ViewClipMode const clipMode) -> void override
    {
        _clipMode = clipMode;
    }

    auto SetOpacity(Float64 const opacity) -> void override
    {
        _opacity = opacity;
    }

    auto SetRenderFlags(PlatformViewLayerRenderFlags const flags) -> void override
    {
        (void)flags;
    }

    auto SetDisplayList(Shared<Graphics::DisplayList> const& displayList) -> void override
    {
        (void)displayList;
    }

    auto SetDisplayListOffset(Offset<Dp> const& offset) -> void override
    {
        (void)offset;
    }
};
}
}
