// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformViewLayer.hpp"
#include "Futurewalker.Application.PlatformViewLayerContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
PlatformViewLayer::PlatformViewLayer(PassKey<PlatformViewLayer>)
{
}

///
/// @brief
///
PlatformViewLayer::~PlatformViewLayer() = default;

///
/// @brief Get unique identifier of this layer.
///
auto PlatformViewLayer::GetId() const -> PlatformViewLayerId
{
    return _id;
}

auto PlatformViewLayer::AddChild(Shared<PlatformViewLayer> child, Shared<PlatformViewLayer> after) -> void
{
    if (child)
    {
        child->RemoveFromParent();

        auto const it = std::find(_children.begin(), _children.end(), after);
        _children.insert(it, child);

        child->_parent = GetSelf();
        child->InternalAttach();

        GetRoot()->RootChildAdded(child);
    }
}

auto PlatformViewLayer::RemoveFromParent() -> void
{
    if (auto const parent = _parent.Lock())
    {
        InternalDetach();

        auto const it = std::remove(parent->_children.begin(), parent->_children.end(), GetSelf());
        parent->_children.erase(it, parent->_children.end());
        _parent = nullptr;

        parent->GetRoot()->RootChildRemoved(parent);
    }
}

auto PlatformViewLayer::IsRoot() const -> Bool
{
    return _parent.IsExpired();
}

auto PlatformViewLayer::GetRoot() -> Shared<PlatformViewLayer>
{
    if (auto parent = GetParent())
    {
        return parent->GetRoot();
    }
    return GetSelf();
}

auto PlatformViewLayer::GetRoot() const -> Shared<PlatformViewLayer const>
{
    if (auto parent = GetParent())
    {
        return parent->GetRoot();
    }
    return GetSelf();
}

auto PlatformViewLayer::GetParent() -> Shared<PlatformViewLayer>
{
    return _parent.Lock();
}

auto PlatformViewLayer::GetParent() const -> Shared<PlatformViewLayer const>
{
    return _parent.Lock();
}

auto PlatformViewLayer::GetChildren() -> std::vector<Shared<PlatformViewLayer>>
{
    return {_children.begin(), _children.end()};
}

auto PlatformViewLayer::GetChildren() const -> std::vector<Shared<PlatformViewLayer const>>
{
    return {_children.begin(), _children.end()};
}

auto PlatformViewLayer::GetOffset() const -> Offset<Dp>
{
    return _offset;
}

auto PlatformViewLayer::SetOffset(Offset<Dp> const& pos) -> void
{
    if (_offset != pos)
    {
        _offset = pos;
        GetRoot()->RootOffsetChanged(GetSelf());
    }
}

auto PlatformViewLayer::GetSize() const -> Size<Dp>
{
    return _size;
}

auto PlatformViewLayer::SetSize(Size<Dp> const& size) -> void
{
    if (_size != size)
    {
        _size = size;
        GetRoot()->RootSizeChanged(GetSelf());
    }
}

auto PlatformViewLayer::GetClipPath() const -> Optional<Graphics::Path>
{
    return _clipPath;
}

auto PlatformViewLayer::SetClipPath(Optional<Graphics::Path> const& path) -> void
{
    if (_clipPath != path)
    {
        _clipPath = path;
        GetRoot()->RootClipPathChanged(GetSelf());
    }
}

auto PlatformViewLayer::GetClipMode() const -> ViewClipMode
{
    return _clipMode;
}

auto PlatformViewLayer::SetClipMode(ViewClipMode const clipMode) -> void
{
    if (_clipMode != clipMode)
    {
        _clipMode = clipMode;
        GetRoot()->RootClipModeChanged(GetSelf());
    }
}

auto PlatformViewLayer::GetOpacity() const -> Float64
{
    return _opacity;
}

auto PlatformViewLayer::SetOpacity(Float64 const opacity) -> void
{
    if (_opacity != opacity)
    {
        _opacity = opacity;
        GetRoot()->RootOpacityChanged(GetSelf());
    }
}

auto PlatformViewLayer::GetRenderFlags() const -> PlatformViewLayerRenderFlags
{
    return _renderFlags;
}

auto PlatformViewLayer::SetRenderFlags(PlatformViewLayerRenderFlags const renderFlags) -> void
{
    if (_renderFlags != renderFlags)
    {
        _renderFlags = renderFlags;
        GetRoot()->RootRenderFlagsChanged(GetSelf());
    }
}

auto PlatformViewLayer::GetDisplayList() const -> Shared<Graphics::DisplayList>
{
    return _displayList;
}

auto PlatformViewLayer::SetDisplayList(Shared<Graphics::DisplayList> const& displayList) -> void
{
    if (_displayList != displayList)
    {
        _displayList = displayList;
        GetRoot()->RootDisplayListChanged(GetSelf());
    }
}

auto PlatformViewLayer::GetDisplayListOffset() const -> Offset<Dp>
{
    return _displayListOffset;
}

auto PlatformViewLayer::SetDisplayListOffset(Offset<Dp> const& offset) -> void
{
    if (_displayListOffset != offset)
    {
        _displayListOffset = offset;
        GetRoot()->RootDisplayListOffsetChanged(GetSelf());
    }
}

auto PlatformViewLayer::GetDisplayScale() const -> DisplayScale
{
    if (IsRoot())
    {
        return RootGetDisplayScale();
    }
    return _displayScale;
}

auto PlatformViewLayer::GetBackingScale() const -> BackingScale
{
    if (IsRoot())
    {
        return RootGetBackingScale();
    }
    return _backingScale;
}

auto PlatformViewLayer::NotifyRootChanged() -> void
{
    if (IsRoot())
    {
        auto const root = GetRoot();
        NotifyRootDisplayScaleChanged(root->RootGetDisplayScale());
        NotifyRootBackingScaleChanged(root->RootGetBackingScale());
    }
}

///
/// @brief Determine whether the layer should be rasterized.
///
/// Override this method to provide custom rasterization behavior.
///
auto PlatformViewLayer::ShouldRasterize() const -> Bool
{
    auto const renderFlags = GetRenderFlags();
    auto const shouldRasterize = (renderFlags & PlatformViewLayerRenderFlags::Rasterize) != PlatformViewLayerRenderFlags::None;
    return IsRoot() || shouldRasterize;
}

///
/// @brief Get control interface.
///
/// Override this method to provide custom control interface.
///
auto PlatformViewLayer::GetControl() -> Shared<PlatformViewLayerControl>
{
    return nullptr;
}

auto PlatformViewLayer::RootGetDisplayScale() const -> DisplayScale
{
    return _displayScale;
}

auto PlatformViewLayer::RootGetBackingScale() const -> BackingScale
{
    return _backingScale;
}

auto PlatformViewLayer::RootOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
}

auto PlatformViewLayer::RootSizeChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
}

auto PlatformViewLayer::RootClipPathChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
}

auto PlatformViewLayer::RootClipModeChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
}

auto PlatformViewLayer::RootOpacityChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
}

auto PlatformViewLayer::RootRenderFlagsChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
}

auto PlatformViewLayer::RootDisplayListChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
}

auto PlatformViewLayer::RootDisplayListOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
}

auto PlatformViewLayer::RootChildAdded(Shared<PlatformViewLayer> const& child) -> void
{
    (void)child;
}

auto PlatformViewLayer::RootChildRemoved(Shared<PlatformViewLayer> const& parent) -> void
{
    (void)parent;
}

///
/// @brief
///
auto PlatformViewLayer::InitializeSelf() -> void
{
    _context = Locator::Resolve<PlatformViewLayerContext>();
    _id = _context->GetNextId();
}

auto PlatformViewLayer::InternalAttach() -> void
{
    auto root = GetRoot();
    NotifyRootDisplayScaleChanged(root->GetDisplayScale());
    NotifyRootBackingScaleChanged(root->GetBackingScale());
    OnAttach();
}

auto PlatformViewLayer::InternalDetach() -> void
{
    OnDetach();
    NotifyRootBackingScaleChanged(1.0);
    NotifyRootDisplayScaleChanged(1.0);
}

auto PlatformViewLayer::NotifyRootDisplayScaleChanged(DisplayScale const rootDisplayScale) -> void
{
    if (_displayScale != rootDisplayScale)
    {
        _displayScale = rootDisplayScale;

        for (auto const& child : _children)
        {
            child->NotifyRootDisplayScaleChanged(rootDisplayScale);
        }
    }
}

auto PlatformViewLayer::NotifyRootBackingScaleChanged(BackingScale const rootBackingScale) -> void
{
    if (_backingScale != rootBackingScale)
    {
        _backingScale = rootBackingScale;

        for (auto const& child : _children)
        {
            child->NotifyRootBackingScaleChanged(rootBackingScale);
        }
    }
}
}
