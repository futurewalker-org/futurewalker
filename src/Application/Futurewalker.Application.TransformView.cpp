// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TransformView.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

namespace FW_DETAIL_NS
{
auto TransformView::Make() -> Shared<TransformView>
{
    return View::MakeDerived<TransformView>();
}

auto TransformView::MakeWithContent(Shared<View> const& content) -> Shared<TransformView>
{
    auto view = Make();
    view->SetContent(content);
    return view;
}

TransformView::TransformView(PassKey<View> key)
  : View(key)
{
}

auto TransformView::GetContent() -> Shared<View>
{
    return GetChildAt(0);
}

auto TransformView::GetContent() const -> Shared<View const>
{
    return GetChildAt(0);
}

auto TransformView::SetContent(Shared<View> const& content) -> void
{
    auto const child = GetChildAt(0);
    if (child != content)
    {
        if (child)
        {
            child->RemoveFromParent();
        }
        AddChildBack(content);
    }
}

auto TransformView::GetTransform() const -> Matrix3x3<Dp>
{
    return _transform.GetValueOrDefault();
}

auto TransformView::SetTransform(AttributeArg<Matrix3x3<Dp>> const& transform) -> void
{
    _transform.SetAttributeArg(transform);
}

auto TransformView::GetAnchorAlignment() const -> Alignment
{
    return _anchorAlignment.GetValueOrDefault();
}

///
/// @brief Set alignment of the anchor point.
///
/// @param[in] alignment Alignment of the anchor point, in normalized coordinates (-1.0 to 1.0).
///
auto TransformView::SetAnchorAlignment(AttributeArg<Alignment> const& alignment) -> void
{
    _anchorAlignment.SetAttributeArg(alignment);
}

auto TransformView::GetAnchorOffset() const -> Vector2<Dp>
{
    return _anchorOffset.GetValueOrDefault();
}

///
/// @brief Set offset of the anchor point.
///
/// @param[in] offset Offset of the anchor point, in Dp units.
///
/// @note The offset is applied to the anchor point calculated from the anchor alignment.
///
auto TransformView::SetAnchorOffset(AttributeArg<Vector2<Dp>> const& offset) -> void
{
    _anchorOffset.SetAttributeArg(offset);
}

auto TransformView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Matrix3x3<Dp>, TransformAttribute, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Alignment, AnchorAlignmentAttribute, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Vector2<Dp>, AnchorOffsetAttribute, {});

    _transform.BindAndConnectAttributeWithDefaultValue(*this, &TransformView::ReceiveAttributeEvent, TransformAttribute, Matrix3x3<Dp>::MakeIdentity());
    _anchorAlignment.BindAndConnectAttributeWithDefaultValue(*this, &TransformView::ReceiveAttributeEvent, AnchorAlignmentAttribute, Alignment::Center());
    _anchorOffset.BindAndConnectAttributeWithDefaultValue(*this, &TransformView::ReceiveAttributeEvent, AnchorOffsetAttribute, Vector2<Dp>(0, 0));
}

auto TransformView::Draw(DrawScope& scope) -> void
{
    auto const contentRect = GetContentRect();
    auto const alignment = _anchorAlignment.GetValueOrDefault();

    auto const reverse = GetLayoutDirection() == LayoutDirection::RightToLeft ? -1.0 : 1.0;
    auto const xRatio = (alignment.GetX() * reverse + 1) / 2;
    auto const yRatio = (alignment.GetY() + 1) / 2;

    auto const anchor = Vector2<Dp>(contentRect.x0 + contentRect.GetWidth() * Dp(xRatio), contentRect.y0 + contentRect.GetHeight() * Dp(yRatio)) + _anchorOffset.GetValueOrDefault();
    auto const transform = Matrix3x3<Dp>::MakeTranslation(anchor) * _transform.GetValueOrDefault() * Matrix3x3<Dp>::MakeTranslation(-anchor);
    scope.SetTransform(transform);
}

auto TransformView::ReceiveAttributeEvent(Event<>& event) -> Bool
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateVisual();
        return true;
    }
    return false;
}
}
