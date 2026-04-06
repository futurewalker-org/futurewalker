// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayerVisualType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Graphics.DisplayListType.hpp"
#include "Futurewalker.Graphics.Path.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.OrderedHashMap.hpp"
#include "Futurewalker.Core.Function.hpp"

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

    virtual ~PlatformViewLayerVisual() = 0;

    auto AddChild(Shared<PlatformViewLayerVisual> const& child) -> void;
    auto RemoveChild(Shared<PlatformViewLayerVisual> const& child) -> void;

    auto GetChildren() -> std::vector<Shared<PlatformViewLayerVisual>> const&;
    auto GetParent() -> Shared<PlatformViewLayerVisual>;

    auto GetBaseLayerId() const -> PlatformViewLayerId;
    auto SetBaseLayerId(PlatformViewLayerId const layerId) -> void;

    auto GetOffset() const -> Vector<Dp>;
    auto SetOffset(Vector<Dp> const& offset) -> void;

    auto GetClipRect() const -> Rect<Dp>;
    auto SetClipRect(Rect<Dp> const& clipRect) -> void;

    auto GetClipPaths() const -> std::vector<Graphics::Path> const&;
    auto SetClipPaths(std::vector<Graphics::Path> const& clipPaths) -> void;

    auto GetOpacity() const -> Float64;
    auto SetOpacity(Float64 const opacity) -> void;

    auto GetDisplayScale() const -> DisplayScale;
    auto SetDisplayScale(DisplayScale const displayScale) -> void;

    auto GetBackingScale() const -> BackingScale;
    auto SetBackingScale(BackingScale const backingScale) -> void;

    enum class FragmentType : int32_t
    {
        DisplayList,
        PushNode,
        PopNode,
    };

    struct FragmentInfo
    {
        PlatformViewLayerId layerId = PlatformViewLayerId(0U);
        FragmentType type = FragmentType::DisplayList;
        SInt32 index = 0;
    };

    struct DisplayListFragment
    {
        Shared<Graphics::DisplayList> displayList;
        Vector<Dp> displayListOffset;

        auto operator==(DisplayListFragment const& other) const -> bool = default;
        auto operator!=(DisplayListFragment const& other) const -> bool = default;
    };

    struct PushNodeFragment
    {
        Vector<Dp> offset;
        Rect<Dp> clipRect;
        Optional<Graphics::Path> clipPath;
        Float64 opacity = 1.0;

        auto operator==(PushNodeFragment const& other) const -> bool = default;
        auto operator!=(PushNodeFragment const& other) const -> bool = default;
    };

    struct PopNodeFragment
    {
        SInt32 pushNodeIndex = 0;
        auto operator==(PopNodeFragment const& other) const -> bool = default;
        auto operator!=(PopNodeFragment const& other) const -> bool = default;
    };

    auto AddDisplayListFragment(PlatformViewLayerId layerId, DisplayListFragment const& fragment) -> void;
    auto AddPushNodeFragment(PlatformViewLayerId layerId, PushNodeFragment const& fragment) -> void;
    auto AddPopNodeFragment(PlatformViewLayerId layerId) -> void;
    auto ReplaceDisplayListFragment(SInt32 const index, DisplayListFragment const& fragment) -> void;
    auto ReplacePushNodeFragment(SInt32 const index, PushNodeFragment const& fragment) -> void;
    auto ClearFragments() -> void;
    auto ForEachFragment(Function<void(FragmentInfo const&)> const& func) const -> void;
    auto GetDisplayListFragment(SInt32 const index) const -> Pointer<DisplayListFragment const>;
    auto GetPushNodeFragment(SInt32 const index) const -> Pointer<PushNodeFragment const>;
    auto GetPopNodeFragment(SInt32 const index) const -> Pointer<PopNodeFragment const>;
    auto GetFragmentCount() const -> SInt32;
    auto CalcFragmentBounds() const -> Rect<Dp>;

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
    virtual auto OnClipPathChanged() -> void = 0;
    virtual auto OnOpacityChanged() -> void = 0;
    virtual auto OnDisplayScaleChanged() -> void = 0;
    virtual auto OnBackingScaleChanged() -> void = 0;

private:
    Weak<PlatformViewLayerVisual> _self;
    Weak<PlatformViewLayerVisual> _parent;
    std::vector<Shared<PlatformViewLayerVisual>> _children;
    PlatformViewLayerId _baseLayerId = PlatformViewLayerId(0U);
    Vector<Dp> _offset;
    Rect<Dp> _clipRect;
    std::vector<Graphics::Path> _clipPaths;
    Float64 _opacity = 1.0;
    std::vector<FragmentInfo> _fragments;
    std::vector<DisplayListFragment> _displayListFragments;
    std::vector<PushNodeFragment> _pushNodeFragments;
    std::vector<PopNodeFragment> _popNodeFragments;
    std::vector<SInt32> _pushNodeIndexStack;
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
