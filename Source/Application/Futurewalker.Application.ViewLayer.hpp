// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.ViewLayerType.hpp"
#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"
#include "Futurewalker.Application.PlatformViewLayerControlType.hpp"
#include "Futurewalker.Application.PlatformDrawableViewLayerContextType.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.ReferenceArg.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Composition layer class.
///
/// Cross platform abstraction for system compositor APIs.
/// It is also used for embedding platform views into view hierarchy.
///
/// ### Creating new layer
///
/// In most cases, you should go through ViewLayerManager to create new instance of ViewLayer.
/// To embed your layer into view hierarchy, use View::AddLayer() which automatically manages lifecycle of embedded layers.
///
/// ### Performance considerations
///
/// ViewLayer generally does not consume much memory when it doesn't have any content attached to it.
/// For layers with visible contents, setting smaller clip rectangle can reduce resource usage.
///
class ViewLayer : NonCopyable
{
public:
    static auto Make(Shared<PlatformViewLayer> const& platformLayer) -> Shared<ViewLayer>;

public:
    ViewLayer(PassKey<ViewLayer>, Shared<PlatformViewLayer> const& platformLayer);

    virtual ~ViewLayer() = default;

    auto AddChild(Shared<ViewLayer> layer, Pointer<ViewLayer const> after) -> void;
    auto RemoveChild(Shared<ViewLayer> layer) -> void;

    auto GetChildIndex(ReferenceArg<ViewLayer const> layer) const -> Optional<SInt64>;
    auto GetChildAt(SInt64 const index) -> Shared<ViewLayer>;

    auto GetOffset() const -> Offset<Dp>;
    auto SetOffset(Offset<Dp> const& offset) -> void;

    auto GetSize() const -> Size<Dp>;
    auto SetSize(Size<Dp> const& size) -> void;

    auto GetClipMode() const -> ViewClipMode;
    auto SetClipMode(ViewClipMode const clipMode) -> void;

    auto SetOpacity() const -> Float64;
    auto SetOpacity(Float64 const opacity) -> void;

private:
    auto GetSelf() -> Shared<ViewLayer>;
    auto GetSelf() const -> Shared<ViewLayer const>;
    auto SetSelf(Shared<ViewLayer> const& layer) -> void;
    auto GetParent() -> Shared<ViewLayer>;
    auto GetParent() const -> Shared<ViewLayer const>;
    auto SetParent(Shared<ViewLayer> const& layer) -> void;

    auto IsRoot() const -> Bool;
    auto GetRoot() -> Shared<ViewLayer>;

    auto RemoveFromParent() -> void;

protected:
    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args)
    {
        auto key = PassKey<ViewLayer>();
        auto layer = Shared<Derived>::Make(key, std::forward<Args>(args)...);
        static_cast<ViewLayer&>(*layer).SetSelf(layer);
        return layer;
    }

private:
    Weak<ViewLayer> _self;
    Weak<ViewLayer> _parent;
    ViewLayerList _children;
    Shared<PlatformViewLayer> _platformLayer;
    Offset<Dp> _offset;
    Size<Dp> _size;
    Float64 _opacity = 1.0;
    ViewClipMode _clipMode = ViewClipMode::None;
};

}
}
