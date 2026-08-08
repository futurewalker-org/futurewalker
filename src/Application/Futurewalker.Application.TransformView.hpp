// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TransformViewType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.Alignment.hpp"

#include "Futurewalker.Geometry.Matrix.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A view that applies a transformation to its content.
///
/// @note Transform matrix is applied after layout, so it only affects the visual of the content.
///
class TransformView : public View
{
public:
    static auto Make() -> Shared<TransformView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<TransformView>;

    TransformView(PassKey<View> key);

    auto GetContent() -> Shared<View>;
    auto GetContent() const -> Shared<View const>;
    auto SetContent(Shared<View> const& content) -> void;

    auto GetTransform() const -> Matrix3x3<Dp>;
    auto SetTransform(AttributeArg<Matrix3x3<Dp>> const& transform) -> void;

    auto GetAnchorAlignment() const -> Alignment;
    auto SetAnchorAlignment(AttributeArg<Alignment> const& alignment) -> void;

    auto GetAnchorOffset() const -> Vector2<Dp>;
    auto SetAnchorOffset(AttributeArg<Vector2<Dp>> const& offset) -> void;

protected:
    auto Initialize() -> void override;
    auto Draw(DrawScope& scope) -> void override;

    auto ReceiveAttributeEvent(Event<>& event) -> Bool;

private:
    AttributeAccessor<Matrix3x3<Dp>> _transform;
    AttributeAccessor<Alignment> _anchorAlignment;
    AttributeAccessor<Vector2<Dp>> _anchorOffset;
};
}
}
