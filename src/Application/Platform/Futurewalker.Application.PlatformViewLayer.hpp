// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayerType.hpp"
#include "Futurewalker.Application.PlatformViewLayerControlType.hpp"
#include "Futurewalker.Application.PlatformViewLayerContextType.hpp"
#include "Futurewalker.Application.ViewType.hpp"

#include "Futurewalker.Graphics.DisplayListType.hpp"
#include "Futurewalker.Graphics.Path.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.TypeTraits.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.Flags.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform specific interface of composition layer.
///
class PlatformViewLayer : NonCopyable
{
public:
    PlatformViewLayer(PassKey<PlatformViewLayer>);

    virtual ~PlatformViewLayer() = 0;

    auto GetId() const -> PlatformViewLayerId;

    auto AddChild(Shared<PlatformViewLayer> child, Shared<PlatformViewLayer> after) -> void;
    auto RemoveFromParent() -> void;

    auto IsRoot() const -> Bool;
    auto GetRoot() -> Shared<PlatformViewLayer>;
    auto GetRoot() const -> Shared<PlatformViewLayer const>;

    auto GetParent() -> Shared<PlatformViewLayer>;
    auto GetParent() const -> Shared<PlatformViewLayer const>;

    auto GetChildren() -> std::vector<Shared<PlatformViewLayer>>;
    auto GetChildren() const -> std::vector<Shared<PlatformViewLayer const>>;

    auto GetOffset() const -> Vector<Dp>;
    auto SetOffset(Vector<Dp> const& pos) -> void;

    auto GetSize() const -> Size<Dp>;
    auto SetSize(Size<Dp> const& size) -> void;

    auto GetClipPath() const -> Optional<Graphics::Path>;
    auto SetClipPath(Optional<Graphics::Path> const& path) -> void;

    auto GetClipMode() const -> ViewClipMode;
    auto SetClipMode(ViewClipMode const clipMode) -> void;

    auto GetOpacity() const -> Float64;
    auto SetOpacity(Float64 const opacity) -> void;

    auto GetRenderFlags() const -> Flags<PlatformViewLayerRenderFlag>;
    auto SetRenderFlags(Flags<PlatformViewLayerRenderFlag> const renderFlags) -> void;

    auto GetDisplayList() const -> Shared<Graphics::DisplayList>;
    auto SetDisplayList(Shared<Graphics::DisplayList> const& displayList) -> void;

    auto GetDisplayListOffset() const -> Vector<Dp>;
    auto SetDisplayListOffset(Vector<Dp> const& offset) -> void;

    auto GetDisplayScale() const -> DisplayScale;
    auto GetBackingScale() const -> BackingScale;

    auto NotifyRootChanged() -> void;

public:
    virtual auto ShouldRasterize() const -> Bool;
    virtual auto GetControl() -> Shared<PlatformViewLayerControl>;

protected:
    virtual auto Initialize() -> void = 0;
    virtual auto OnAttach() -> void = 0;
    virtual auto OnDetach() -> void = 0;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

private:
    virtual auto RootGetDisplayScale() const -> DisplayScale;
    virtual auto RootGetBackingScale() const -> BackingScale;
    virtual auto RootOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void;
    virtual auto RootSizeChanged(Shared<PlatformViewLayer> const& layer) -> void;
    virtual auto RootClipPathChanged(Shared<PlatformViewLayer> const& layer) -> void;
    virtual auto RootClipModeChanged(Shared<PlatformViewLayer> const& layer) -> void;
    virtual auto RootOpacityChanged(Shared<PlatformViewLayer> const& layer) -> void;
    virtual auto RootRenderFlagsChanged(Shared<PlatformViewLayer> const& layer) -> void;
    virtual auto RootDisplayListChanged(Shared<PlatformViewLayer> const& layer) -> void;
    virtual auto RootDisplayListOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void;
    virtual auto RootChildAdded(Shared<PlatformViewLayer> const& child) -> void;
    virtual auto RootChildRemoved(Shared<PlatformViewLayer> const& parent) -> void;

private:
    auto InitializeSelf() -> void;
    auto InternalAttach() -> void;
    auto InternalDetach() -> void;
    auto NotifyRootDisplayScaleChanged(DisplayScale const rootDisplayScale) -> void;
    auto NotifyRootBackingScaleChanged(BackingScale const rootBackingScale) -> void;

private:
    Shared<PlatformViewLayerContext> _context;
    Weak<PlatformViewLayer> _self;
    Weak<PlatformViewLayer> _parent;
    std::list<Shared<PlatformViewLayer>> _children;
    Flags<PlatformViewLayerRenderFlag> _renderFlags = PlatformViewLayerRenderFlag::None;
    Size<Dp> _size;
    Vector<Dp> _offset;
    Optional<Graphics::Path> _clipPath;
    ViewClipMode _clipMode = ViewClipMode::None;
    Float64 _opacity = 1.0;
    Shared<Graphics::DisplayList> _displayList;
    Vector<Dp> _displayListOffset;
    DisplayScale _displayScale = 1.0;
    BackingScale _backingScale = 1.0;
    PlatformViewLayerId _id = 0U;
};

///
/// @brief
///
template <class Self>
auto PlatformViewLayer::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, PlatformViewLayer>>(self)._self.Lock().template UnsafeAs<Self>();
}

///
/// @brief
///
template <class Derived, class... Args>
auto PlatformViewLayer::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto key = PassKey<PlatformViewLayer>();
    auto view = Shared<Derived>::Make(key, std::forward<Args>(args)...);
    static_cast<PlatformViewLayer&>(*view)._self = view;
    static_cast<PlatformViewLayer&>(*view).InitializeSelf();
    static_cast<PlatformViewLayer&>(*view).Initialize();
    return view;
}
}
}
