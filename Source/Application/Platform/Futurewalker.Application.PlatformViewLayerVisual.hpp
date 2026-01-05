// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayerVisualType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Graphics.DisplayListType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.OrderedHashMap.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief View layer visual.
///
/// Visual object stores contents of multiple view layers as fragments.
/// Parent-child relationship is only for retaining information of original view layer hierarchy.
///
/// There are visuals those that correspond to rasterizing view layers and those that do not.
/// A visual that correspond to rasterizing view layer includes a fragment for the base layer itself.
///
class PlatformViewLayerVisual : NonCopyable
{
public:
    PlatformViewLayerVisual(PassKey<PlatformViewLayerVisual>);

    auto AddChild(Shared<PlatformViewLayerVisual> const& child) -> void;
    auto RemoveChild(Shared<PlatformViewLayerVisual> const& child) -> void;

    auto GetChildren() -> std::vector<Shared<PlatformViewLayerVisual>> const&;
    auto GetParent() -> Shared<PlatformViewLayerVisual>;

    auto GetBaseLayerId() const -> PlatformViewLayerId;
    auto SetBaseLayerId(PlatformViewLayerId const layerId) -> void;

    auto GetOffset() const -> Offset<Dp>;
    auto SetOffset(Offset<Dp> const& offset) -> void;

    auto GetClipRect() const -> Rect<Dp>;
    auto SetClipRect(Rect<Dp> const& clipRect) -> void;

    auto GetOpacity() const -> Float64;
    auto SetOpacity(Float64 const opacity) -> void;

    auto GetDisplayScale() const -> DisplayScale;
    auto SetDisplayScale(DisplayScale const displayScale) -> void;

    auto GetBackingScale() const -> BackingScale;
    auto SetBackingScale(BackingScale const backingScale) -> void;

    struct Fragment
    {
        Offset<Dp> offset;
        Rect<Dp> clipRect;
        Float64 opacity = 1.0;
        Shared<Graphics::DisplayList> displayList;
        Offset<Dp> displayListOffset;
    };
    auto InsertFragment(const SInt64 index, PlatformViewLayerId layerId, Fragment const& fragment) -> void;
    auto ReplaceFragment(const SInt64 index, PlatformViewLayerId layerId, Fragment const& fragment) -> void;
    auto ReplaceFragment(PlatformViewLayerId layerId, Fragment const& fragment) -> void;
    auto RemoveFragment(const SInt64 index) -> void;
    auto ClearFragments() -> void;
    auto GetFragmentIndexByLayerId(PlatformViewLayerId const layerId) const -> Optional<SInt64>;
    auto GetFragment(SInt64 const index) const -> Optional<Fragment>;
    auto GetFragmentCount() const -> SInt64;

protected:
    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

protected:
    virtual auto Initialize() -> void = 0;
    virtual auto OnFragmentChanged() -> void = 0;
    virtual auto OnOffsetChanged() -> void = 0;
    virtual auto OnClipRectChanged() -> void = 0;
    virtual auto OnOpacityChanged() -> void = 0;
    virtual auto OnDisplayScaleChanged() -> void = 0;
    virtual auto OnBackingScaleChanged() -> void = 0;

private:
    Weak<PlatformViewLayerVisual> _self;
    Weak<PlatformViewLayerVisual> _parent;
    std::vector<Shared<PlatformViewLayerVisual>> _children;
    PlatformViewLayerId _baseLayerId = PlatformViewLayerId(0U);
    Bool _invalidated = true;
    Offset<Dp> _offset;
    Rect<Dp> _clipRect;
    Float64 _opacity = 1.0;
    OrderedHashMap<PlatformViewLayerId, Fragment> _fragments;
    DisplayScale _displayScale = 1.0;
    BackingScale _backingScale = 1.0;
};

///
/// @brief
///
template <class Self>
auto PlatformViewLayerVisual::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, PlatformViewLayerVisual>>(self)._self.Lock().template UnsafeAs<Self>();
}

///
/// @brief
///
template <class Derived, class... Args>
auto PlatformViewLayerVisual::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto key = PassKey<PlatformViewLayerVisual>();
    auto view = Shared<Derived>::Make(key, std::forward<Args>(args)...);
    static_cast<PlatformViewLayerVisual&>(*view)._self = view;
    static_cast<PlatformViewLayerVisual&>(*view).Initialize();
    return view;
}
}
}
