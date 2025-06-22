// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.WindowFrameView.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.WindowAttribute.hpp"
#include "Futurewalker.Application.WindowAreaManager.hpp"
#include "Futurewalker.Application.WindowAreaManagerEvent.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"
#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Attribute.AttributeObserver.hpp"

#include "Futurewalker.Graphics.Scene.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Make title bar layout.
///
auto WindowFrameView::Make() -> Shared<WindowFrameView>
{
    return View::MakeDerived<WindowFrameView>();
}

///
/// @brief Constructor.
///
WindowFrameView::WindowFrameView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief Initialize.
///
auto WindowFrameView::Initialize() -> void
{
    _windowAreaManager = AttributeNode::GetObserver<&WindowAttribute::AreaManager>(*this);
    _titleBackground = ContainerView::Make();
    _titleContent = ContainerView::Make();
    _background = ContainerView::Make();
    _content = ContainerView::Make();

    AddChildBack(_background);
    AddChildBack(_titleBackground);
    AddChildBack(_titleContent);
    AddChildBack(_content);

    EventReceiver::Connect(*_windowAreaManager, *this, &WindowFrameView::ReceiveAttributeEvent);
    EventReceiver::Connect(*this, *this, &WindowFrameView::ReceiveEvent);

    UpdateAreaManager();
}

///
/// @brief Measure.
///
auto WindowFrameView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();  
    auto const& width = parameter.GetWidthConstraints();
    auto const& height = parameter.GetHeightConstraints();

    auto measureSize = [](auto const& c) { return c.IsBounded() ? c.GetMax() : c.GetMin(); };
    auto const measuredWidth = measureSize(width);
    auto const measuredHeight = measureSize(height);

    scope.MeasureChild(_background, AxisConstraints::MakeExact(measuredWidth), AxisConstraints::MakeExact(measuredHeight));

    auto const titleBarAreaRect = GetTitleBarAreaRect();
    scope.MeasureChild(_titleBackground, AxisConstraints::MakeExact(titleBarAreaRect.GetWidth()), AxisConstraints::MakeExact(titleBarAreaRect.GetHeight()));

    auto const titleBarAvailableAreaRect = GetTitleBarAvailableAreaRect();
    scope.MeasureChild(_titleContent, AxisConstraints::MakeExact(titleBarAvailableAreaRect.GetWidth()), AxisConstraints::MakeExact(titleBarAvailableAreaRect.GetHeight()));

    auto const contentAreaRect = GetContentAreaRect();
    scope.MeasureChild(_content, AxisConstraints::MakeExact(contentAreaRect.GetWidth()), AxisConstraints::MakeExact(contentAreaRect.GetHeight()));

    scope.SetMeasuredSize(measuredWidth, measuredHeight);
}

///
/// @brief Arrange.
///
auto WindowFrameView::Arrange(ArrangeScope& scope) -> void
{
    scope.ArrangeChild(_background, Point<Dp>());

    auto const titleBarAreaRect = GetTitleBarAreaRect();
    scope.ArrangeChild(_titleBackground, titleBarAreaRect.GetPosition());

    auto const titleBarAvailableAreaRect = GetTitleBarAvailableAreaRect();
    scope.ArrangeChild(_titleContent, titleBarAvailableAreaRect.GetPosition());

    auto const contentAreaRect = GetContentAreaRect();
    scope.ArrangeChild(_content, contentAreaRect.GetPosition());
}

///
/// @brief 
///
auto WindowFrameView::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();

    scene.AddRect({
      .rect = GetContentRect(),
      .color = RGBAColor(1, 1, 1, 0.25),
      .drawStyle = Graphics::DrawStyle::Fill,
    });

    scene.AddRect({
      .rect = GetTitleBarAreaRect(),
      .color = RGBAColor(0, 1, 1, 0.25),
      .drawStyle = Graphics::DrawStyle::Fill,
    });
    scene.AddRect({
      .rect = GetTitleBarAreaRect(),
      .color = RGBAColor(0, 1, 0, 1),
      .drawStyle = Graphics::DrawStyle::Stroke,
      .strokeWidth = 1.0,
    });

    scene.AddRect({
      .rect = GetTitleBarAvailableAreaRect(),
      .color = RGBAColor(1, 0, 1, 0.25),
      .drawStyle = Graphics::DrawStyle::Fill,
    });
    scene.AddRect({
      .rect = GetTitleBarAvailableAreaRect(),
      .color = RGBAColor(0, 1, 0, 1),
      .drawStyle = Graphics::DrawStyle::Stroke,
      .strokeWidth = 1.0,
    });

    scene.AddRect({
      .rect = GetContentAreaRect(),
      .color = RGBAColor(1, 1, 0, 0.25),
      .drawStyle = Graphics::DrawStyle::Fill,
    });
    scene.AddRect({
      .rect = GetContentAreaRect(),
      .color = RGBAColor(0, 1, 0, 1),
      .drawStyle = Graphics::DrawStyle::Stroke,
      .strokeWidth = 1.0,
    });
}

auto WindowFrameView::ReceiveEvent(Event& event) -> Async<Bool>
{
    if (event.Is<WindowAreaManagerEvent::TitleBarAreaRectChanged>() ||
        event.Is<WindowAreaManagerEvent::TitleBarAvailableAreaRectChanged>() ||
        event.Is<WindowAreaManagerEvent::ContentAreaRectChanged>())
    {
        InvalidateLayout();
        InvalidateVisual();
    }
    co_return false;
}

///
/// @brief Handle attribute event.
///
auto WindowFrameView::ReceiveAttributeEvent(Event& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        UpdateAreaManager();
    }
    co_return false;
}

///
/// @brief Get background of entire area.
///
auto WindowFrameView::GetBackground() -> Shared<View>
{
    return _background->GetContent();
}

///
/// @brief Set background of entire area.
///
auto WindowFrameView::SetBackground(Shared<View> background) -> void
{
    _background->SetContent(background);
}

///
/// @brief Get background of content area.
///
auto WindowFrameView::GetTitleBackground() -> Shared<View>
{
    return _titleBackground->GetContent();
}

///
/// @brief Set background of title area.
///
auto WindowFrameView::SetTitleBackground(Shared<View> background) -> void
{
    _titleBackground->SetContent(background);
}

///
/// @brief Get background of content area.
///
auto WindowFrameView::GetContentBackground() -> Shared<View>
{
    return _contentBackground->GetContent();
}

///
/// @brief
///
auto WindowFrameView::SetContentBackground(Shared<View> background) -> void
{
    _contentBackground->SetContent(background);
}

///
/// @brief Get content of content area.
///
auto WindowFrameView::GetContent() -> Shared<View>
{
    return _content->GetContent();
}

///
/// @brief Set content of content area.
///
auto WindowFrameView::SetContent(Shared<View> content) -> void
{
    _content->SetContent(content);
}

///
/// @brief Get content of title area.
///
auto WindowFrameView::GetTitleContent() -> Shared<View>
{
    return _titleContent->GetContent();
}

///
/// @brief Set content of title area.
///
auto WindowFrameView::SetTitleContent(Shared<View> content) -> void
{
    _titleContent->SetContent(content);
}

///
/// @brief Get current area manager.
///
auto WindowFrameView::GetAreaManager() -> Shared<WindowAreaManager>
{
    if (_windowAreaManager)
    {
        if (auto const manager = _windowAreaManager->GetValue())
        {
            return *manager;
        }
    }
    return {};
}

///
/// @brief Get current area manager.
///
auto WindowFrameView::GetAreaManager() const -> Shared<const WindowAreaManager>
{
    if (_windowAreaManager)
    {
        if (auto const manager = _windowAreaManager->GetValue())
        {
            return *manager;
        }
    }
    return {};
}

///
/// @brief Get local rect of an area.
///
auto WindowFrameView::GetTitleBarAreaRect() const -> Rect<Dp>
{
    if (auto const areaManager = GetAreaManager())
    {
        return RootToLocalRect(areaManager->GetTitleBarAreaRect());
    }
    return {};
}

///
/// @brief Get local rect of an area.
///
auto WindowFrameView::GetTitleBarAvailableAreaRect() const -> Rect<Dp>
{
    if (auto const areaManager = GetAreaManager())
    {
        return RootToLocalRect(areaManager->GetTitleBarAvailableAreaRect());
    }
    return {};
}

///
/// @brief Get local rect of an area.
///
auto WindowFrameView::GetContentAreaRect() const -> Rect<Dp>
{
    if (auto const areaManager = GetAreaManager())
    {
        return RootToLocalRect(areaManager->GetContentAreaRect());
    }
    return {};
}

///
/// @brief Update area manager.
///
auto WindowFrameView::UpdateAreaManager() -> void
{
    _connection.Disconnect();

    if (auto const manager = GetAreaManager())
    {
        _connection = EventReceiver::Connect(*manager, *this, &WindowFrameView::ReceiveEvent);
        InvalidateLayout();
        InvalidateVisual();
    }
}
}
}
