// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.ViewLayer.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"
#include "Futurewalker.Application.PlatformDrawableViewLayerContext.hpp"
#include "Futurewalker.Application.PlatformDrawableViewLayerControl.hpp"

#include "Futurewalker.Base.Debug.hpp"
#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.UniqueIdentifier.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Create new platform layer.
///
/// @param platformLayer Platform layer object. Cannot be null.
///
auto ViewLayer::Make(Shared<PlatformViewLayer> const& platformLayer) -> Shared<ViewLayer>
{
    return ViewLayer::MakeDerived<ViewLayer>(platformLayer);
}

///
/// @brief
///
/// @param platformLayer
///
ViewLayer::ViewLayer(PassKey<ViewLayer>, Shared<PlatformViewLayer> const& platformLayer)
  : _platformLayer {platformLayer}
{
    FW_DEBUG_ASSERT(_platformLayer);
    _platformLayer->SetOffset(_offset);
    _platformLayer->SetSize(_size);
    _platformLayer->SetOpacity(_opacity);
    _platformLayer->SetClipMode(_clipMode);
}

///
/// @brief Insert child layer.
///
/// @param layer A layer to insert.
/// @param after A child layer which should come right after inserted layer.
///
/// @note `layer` will be removed from current parent.
/// @note When `after` is not a child of this layer, `layer` will become last (topmost) child of this layer.
///
auto ViewLayer::AddChild(Shared<ViewLayer> layer, Pointer<ViewLayer const> after) -> void
{
    if (layer)
    {
        layer->RemoveFromParent();
        layer->SetParent(GetSelf());
        const auto it = std::find_if(_children.begin(), _children.end(), [&](const auto& child) { return child.GetPointer() == after; });
        _platformLayer->AddChild(layer->_platformLayer, (it != _children.end()) ? (*it)->_platformLayer : nullptr);
        _children.insert(it, layer);
    }
}

///
/// @brief Remove child layer.
///
/// @param layer
///
auto ViewLayer::RemoveChild(Shared<ViewLayer> layer) -> void
{
    if (!layer)
    {
        return;
    }

    for (const auto& child : _children)
    {
        if (child == layer)
        {
            child->RemoveFromParent();
            return;
        }
    }
}

///
/// @brief Get number of children.
///
auto ViewLayer::GetChildCount() const -> SInt64
{
    return std::ssize(_children);
}

///
/// @brief Get index of child layer.
///
/// @param layer
///
auto ViewLayer::GetChildIndex(ReferenceArg<ViewLayer const> layer) const -> Optional<SInt64>
{
    auto index = SInt64(0);
    for (const auto& child : _children)
    {
        if (child.GetPointer() == layer.GetPointer())
        {
            return index;
        }
        ++index;
    }
    return {};
}

///
/// @brief Get child layer at index.
///
/// @param index
///
auto ViewLayer::GetChildAt(SInt64 const index) -> Shared<ViewLayer>
{
    if (0 <= index && index < std::ssize(_children))
    {
        return *std::next(_children.begin(), static_cast<ptrdiff_t>(index));
    }
    return {};
}

///
/// @brief 
///
auto ViewLayer::GetOffset() const -> Offset<Dp>
{
    return _offset;
}

///
/// @brief Set origin of this layer.
///
/// @param offset Offset of origin in parent's origin.
///
auto ViewLayer::SetOffset(Offset<Dp> const& offset) -> void
{
    if (_offset != offset)
    {
        _offset = offset;
        _platformLayer->SetOffset(_offset);
    }
}

///
/// @brief 
///
auto ViewLayer::GetSize() const -> Size<Dp>
{
    return _size;
}

///
/// @brief
///
/// @param size
///
auto ViewLayer::SetSize(Size<Dp> const& size) -> void
{
    if (size.IsFinite())
    {
        if (_size != size)
        {
            _size = size;
            _platformLayer->SetSize(_size);
        }
    }
}

///
/// @brief Returns true if contents of the layer will be clipped to bounds.
///
auto ViewLayer::GetClipMode() const -> ViewClipMode
{
    return _clipMode;
}

///
/// @brief Set clip mode of the layer.
///
/// @param clipMode
///
auto ViewLayer::SetClipMode(ViewClipMode const clipMode) -> void
{
    if (_clipMode != clipMode)
    {
        _clipMode = clipMode;
        _platformLayer->SetClipMode(_clipMode);
    }
}

///
/// @brief
///
auto ViewLayer::SetOpacity() const -> Float64
{
    return _opacity;
}

///
/// @brief Set opacity of the layer.
///
/// @param opacity
///
auto ViewLayer::SetOpacity(Float64 const opacity) -> void
{
    if (Float64::IsFinite(opacity))
    {
        if (_opacity != opacity)
        {
            _opacity = opacity;
            _platformLayer->SetOpacity(_opacity);
        }
    }
}

///
/// @brief
///
auto ViewLayer::GetSelf() -> Shared<ViewLayer>
{
    return _self.Lock();
}

///
/// @brief
///
auto ViewLayer::GetSelf() const -> Shared<ViewLayer const>
{
    return _self.Lock();
}

///
/// @brief
///
/// @param layer
///
auto ViewLayer::SetSelf(Shared<ViewLayer> const& layer) -> void
{
    _self = layer;
}

///
/// @brief
///
auto ViewLayer::GetParent() -> Shared<ViewLayer>
{
    return _parent.Lock();
}

///
/// @brief
///
auto ViewLayer::GetParent() const -> Shared<ViewLayer const>
{
    return _parent.Lock();
}

///
/// @brief
///
/// @param layer
///
auto ViewLayer::SetParent(Shared<ViewLayer> const& layer) -> void
{
    _parent = layer;
}

///
/// @brief 
///
auto ViewLayer::IsRoot() const -> Bool
{
    return !GetParent();
}

///
/// @brief 
///
auto ViewLayer::GetRoot() -> Shared<ViewLayer>
{
    auto layer = GetParent();
    while (layer)
    {
        layer = layer->GetParent();
    }
    return layer;
}

///
/// @brief Remove layer from parent.
///
auto ViewLayer::RemoveFromParent() -> void
{
    if (auto parent = _parent.Lock())
    {
        const auto self = GetSelf();
        const auto it = std::find(_children.begin(), _children.end(), self);
        if (it != _children.end())
        {
            parent->_children.erase(it);
            parent->_platformLayer->RemoveFromParent();
        }
        SetParent(nullptr);
    }
}
}
