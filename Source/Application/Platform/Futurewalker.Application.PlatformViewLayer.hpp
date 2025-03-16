// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformViewLayerType.hpp"
#include "Futurewalker.Application.PlatformViewLayerControlType.hpp"
#include "Futurewalker.Application.ViewType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.TypeTraits.hpp"
#include "Futurewalker.Core.Memory.hpp"

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

    ///
    /// @brief Add child layer.
    ///
    virtual auto AddChild(Shared<PlatformViewLayer> child, Shared<PlatformViewLayer> after) -> void = 0;

    ///
    /// @brief Remove from parent layer.
    ///
    virtual auto RemoveFromParent() -> void = 0;

    ///
    /// @brief Get array of child layers.
    ///
    virtual auto GetChildren() -> PlatformViewLayerArray = 0;

    ///
    /// @brief Get control interface.
    ///
    virtual auto GetControl() -> Shared<PlatformViewLayerControl> = 0;

    ///
    /// @brief Set offset of this layer.
    ///
    virtual auto SetOffset(Offset<Dp> const& pos) -> void = 0;

    ///
    /// @brief Set size of this layer's bounds.
    ///
    virtual auto SetSize(Size<Dp> const& size) -> void = 0;

    ///
    /// @brief Set if the layer clips to bounds.
    ///
    virtual auto SetClipMode(ViewClipMode const clipMode) -> void = 0;

    ///
    /// @brief Set opacity of this layer.
    ///
    virtual auto SetOpacity(Float64 const opacity) -> void = 0;

protected:
    virtual auto Initialize() -> void;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

private:
    Weak<PlatformViewLayer> _self;
};

///
/// @brief
///
template <class Self>
auto PlatformViewLayer::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, PlatformViewLayer>>(self)._self.Lock().template Assume<Self>();
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
    static_cast<PlatformViewLayer&>(*view).Initialize();
    return view;
}
}
}
