// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.ViewEvent.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

#include "Futurewalker.Graphics.Scene.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
namespace
{
auto const AttributeBackgroundColor = StaticAttribute<RGBAColor>::MakeWithDefaultValue(RGBAColor());
}

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
/// @brief Initialize.
///
auto ContainerView::Initialize() -> void
{
    _backgroundColor.BindAttributeWithDefault(*this, AttributeBackgroundColor, RGBAColor());

    EventReceiver::Connect(*this, *this, &ContainerView::ReceiveEvent);
    EventReceiver::Connect(_backgroundColor, *this, &ContainerView::ReceiveEvent);
}

///
/// @brief Draw.
///
auto ContainerView::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();

    auto const backgroundColor = _backgroundColor.GetValueOrDefault();

    scene.AddRect({
      .rect = GetContentRect(),
      .color = backgroundColor,
    });
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
