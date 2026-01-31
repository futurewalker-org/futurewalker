// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.ViewEvent.hpp"
#include "Futurewalker.Application.ViewDrawFunction.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

#include "Futurewalker.Graphics.Scene.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make ContainerView instance.
///
auto ContainerView::Make() -> Shared<ContainerView>
{
    return View::MakeDerived<ContainerView>();
}

///
/// @brief Make ContainerView instance with specified content.
///
/// @param[in] content Content of new instance.
///
auto ContainerView::MakeWithContent(Shared<View> const& content) -> Shared<ContainerView>
{
    auto view = Make();
    view->SetContent(content);
    return view;
}

///
/// @brief Constructor.
///
ContainerView::ContainerView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief Get content view.
///
auto ContainerView::GetContent() -> Shared<View>
{
    return _content.Lock();
}

///
/// @brief Get content view.
///
auto ContainerView::GetContent() const -> Shared<View const>
{
    return _content.Lock();
}

///
/// @brief Set content view.
///
/// @param[in] view New content view.
///
auto ContainerView::SetContent(Shared<View> const& view) -> void
{
    if (const auto content = GetContent())
    {
        if (content == view)
        {
            return;
        }
        content->RemoveFromParent();
    }
    _content = view;
    AddChildFront(view);
}

///
/// @brief Set background color.
///
/// @param[in] color Background color.
///
auto ContainerView::SetBackgroundColor(AttributeArg<RGBAColor> color) -> void
{
    _backgroundColor.SetAttributeArg(color);
}

///
/// @brief Set border color.
///
/// @param[in] color Border color.
///
auto ContainerView::SetBorderColor(AttributeArg<RGBAColor> color) -> void
{
    _borderColor.SetAttributeArg(color);
}

///
/// @brief Set border width.
///
/// @param[in] width Border width.
///
auto ContainerView::SetBorderWidth(AttributeArg<Dp> width) -> void
{
    _borderWidth.SetAttributeArg(width);
}

///
/// @brief Set corner radius.
///
/// @param[in] radius Corner radius.
///
auto ContainerView::SetCornerRadius(AttributeArg<CornerRadius> radius) -> void
{
    _cornerRadius.SetAttributeArg(radius);
}

///
/// @brief Initialize.
///
auto ContainerView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBackgroundColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(CornerRadius, AttributeCornerRadius, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBorderColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeBorderWidth, {0});

    _backgroundColor.BindAndConnectAttributeWithDefaultValue(*this, &ContainerView::ReceiveEvent, AttributeBackgroundColor, {});
    _borderColor.BindAndConnectAttributeWithDefaultValue(*this, &ContainerView::ReceiveEvent, AttributeBorderColor, {});
    _borderWidth.BindAndConnectAttributeWithDefaultValue(*this, &ContainerView::ReceiveEvent, AttributeBorderWidth, {0});
    _cornerRadius.BindAndConnectAttributeWithDefaultValue(*this, &ContainerView::ReceiveEvent, AttributeCornerRadius, {});

    EventReceiver::Connect(*this, *this, &ContainerView::ReceiveEvent);
}

///
/// @brief Draw.
///
auto ContainerView::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();

    auto const backgroundColor = _backgroundColor.GetValueOrDefault();
    auto const borderColor = _borderColor.GetValueOrDefault();
    auto const borderWidth = _borderWidth.GetValueOr(0);
    auto const cornerRadius = _cornerRadius.GetValueOrDefault();

    if (cornerRadius != CornerRadius())
    {
        auto path = Graphics::Path();
        path.AddRoundRect(cornerRadius.GetRoundRect(GetContentRect(), GetLayoutDirection()));
        scope.SetClipPath(path);
    }
    else
    {
        scope.SetClipMode(ViewClipMode::Bounds);
    }

    ViewDrawFunction::DrawRoundRect(scene, GetContentRect(), cornerRadius, backgroundColor, GetLayoutDirection());
    ViewDrawFunction::DrawRoundRectBorder(scene, GetContentRect(), cornerRadius, borderColor, borderWidth, GetLayoutDirection());
}

///
/// @brief Handle event.
///
auto ContainerView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ViewEvent::ChildRemoved>())
    {
        _content.Reset();
    }
    else if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateVisual();
    }
    co_return false;
}
}
