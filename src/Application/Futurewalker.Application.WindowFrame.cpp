// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.WindowFrame.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.ViewAttribute.hpp"
#include "Futurewalker.Application.ViewAreaManager.hpp"
#include "Futurewalker.Application.ViewAreaManagerEvent.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"
#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Attribute.AttributeObserver.hpp"

#include "Futurewalker.Graphics.Scene.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make title bar layout.
///
auto WindowFrame::Make() -> Shared<WindowFrame>
{
    return View::MakeDerived<WindowFrame>();
}

///
/// @brief Constructor.
///
WindowFrame::WindowFrame(PassKey<View> key)
  : View(key)
{
}

///
/// @brief Initialize.
///
auto WindowFrame::Initialize() -> void
{
    _titleBackground = ContainerView::Make();
    _titleContent = ContainerView::Make();
    _background = ContainerView::Make();
    _content = ContainerView::Make();

    AddChildBack(_background);
    AddChildBack(_titleBackground);
    AddChildBack(_titleContent);
    AddChildBack(_content);

    EventReceiver::Connect(*this, *this, &WindowFrame::ReceiveEvent);

    _areaManager.BindAndConnectAttribute(*this, &WindowFrame::ReceiveAttributeEvent, ViewAttribute::AreaManager);

    UpdateAreaManager();
}

///
/// @brief Measure.
///
auto WindowFrame::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();  
    auto const& width = parameter.GetWidthConstraints();
    auto const& height = parameter.GetHeightConstraints();

    auto const titleBarInsets = GetTitleBarInsets();
    auto const contentWidth = width;
    auto const contentHeight = AxisConstraints::Offset(height, -titleBarInsets.GetTop());
    auto const contentSize = scope.MeasureChild(_content, contentWidth, contentHeight);

    auto const measuredWidth = AxisConstraints::Constrain(width, contentSize.width);
    auto const measuredHeight = AxisConstraints::Constrain(height, contentSize.height + titleBarInsets.GetTop());
    auto const measuredSize = Size<Dp>(measuredWidth, measuredHeight);
    scope.MeasureChild(_background, AxisConstraints::MakeExact(measuredSize.width), AxisConstraints::MakeExact(measuredSize.height));
    scope.MeasureChild(_titleBackground, AxisConstraints::MakeExact(measuredSize.width), AxisConstraints::MakeExact(titleBarInsets.GetTop()));

    if (width.IsBounded() && height.IsBounded())
    {
        auto const titleBarContentRect = GetTitleBarContentRect(measuredSize);
        scope.MeasureChild(_titleContent, AxisConstraints::MakeExact(titleBarContentRect.GetWidth()), AxisConstraints::MakeExact(titleBarContentRect.GetHeight()));
    }
    else
    {
        scope.MeasureChild(_titleContent, contentWidth, AxisConstraints::MakeExact(titleBarInsets.GetTop()));
    }
    scope.SetMeasuredSize(measuredSize);
}

///
/// @brief
///
/// @param size
///
auto WindowFrame::GetTitleBarContentRect(Size<Dp> const& size) const -> Rect<Dp>
{
    auto const titleBarInsets = GetTitleBarInsets();
    auto boundingRects = GetTitleBarBoundingRects();
    std::sort(boundingRects.begin(), boundingRects.end(), [](auto const& lhs, auto const& rhs) { return lhs.GetLeft() < rhs.GetLeft(); });
    boundingRects.push_back(Rect<Dp>(size.width, Dp(0), size.width, titleBarInsets.GetTop()));

    auto maxWidth = Dp(0);
    auto maxWidthLeft = Dp(0);
    auto maxWidthRight = Dp(0);
    auto lastRight = Dp(0);
    for (auto const& rect : boundingRects)
    {
        auto const gap = rect.GetLeft() - lastRight;
        if (gap > maxWidth)
        {
            maxWidth = gap;
            maxWidthLeft = lastRight;
            maxWidthRight = rect.GetLeft();
        }
        lastRight = rect.GetRight();
    }
    return Rect<Dp>(maxWidthLeft, Dp(0), maxWidthRight, titleBarInsets.GetTop());
}

///
/// @brief Arrange.
///
auto WindowFrame::Arrange(ArrangeScope& scope) -> void
{
    auto const titleBarInsets = GetTitleBarInsets();
    auto const titleBarContentRect = GetTitleBarContentRect(scope.GetMeasuredSize(*this));
    scope.ArrangeChild(_background, Point<Dp>());
    scope.ArrangeChild(_content, Point<Dp>(Dp(0), titleBarInsets.GetTop()));
    scope.ArrangeChild(_titleBackground, Point<Dp>());
    scope.ArrangeChild(_titleContent, titleBarContentRect.GetTopLeft());
}

///
/// @brief 
///
/// @param event 
///
auto WindowFrame::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ViewAreaManagerEvent::GeometryChanged>())
    {
        InvalidateLayout();
        InvalidateVisual();
    }
    co_return false;
}

///
/// @brief Handle attribute event.
///
auto WindowFrame::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
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
auto WindowFrame::GetBackground() -> Shared<View>
{
    return _background->GetContent();
}

///
/// @brief Set background of entire area.
///
auto WindowFrame::SetBackground(Shared<View> background) -> void
{
    _background->SetContent(background);
}

///
/// @brief Get background of content area.
///
auto WindowFrame::GetTitleBackground() -> Shared<View>
{
    return _titleBackground->GetContent();
}

///
/// @brief Set background of title area.
///
auto WindowFrame::SetTitleBackground(Shared<View> background) -> void
{
    _titleBackground->SetContent(background);
}

///
/// @brief Get background of content area.
///
auto WindowFrame::GetContentBackground() -> Shared<View>
{
    return _contentBackground->GetContent();
}

///
/// @brief
///
auto WindowFrame::SetContentBackground(Shared<View> background) -> void
{
    _contentBackground->SetContent(background);
}

///
/// @brief Get content of content area.
///
auto WindowFrame::GetContent() -> Shared<View>
{
    return _content->GetContent();
}

///
/// @brief Set content of content area.
///
auto WindowFrame::SetContent(Shared<View> content) -> void
{
    _content->SetContent(content);
}

///
/// @brief Get content of title area.
///
auto WindowFrame::GetTitleContent() -> Shared<View>
{
    return _titleContent->GetContent();
}

///
/// @brief Set content of title area.
///
auto WindowFrame::SetTitleContent(Shared<View> content) -> void
{
    _titleContent->SetContent(content);
}

///
/// @brief Get current area manager.
///
auto WindowFrame::GetAreaManager() -> Shared<ViewAreaManager>
{
    return _areaManager.GetValueOrDefault();
}

///
/// @brief Get current area manager.
///
auto WindowFrame::GetAreaManager() const -> Shared<const ViewAreaManager>
{
    return _areaManager.GetValueOrDefault();
}

///
/// @brief 
///
auto WindowFrame::GetTitleBarInsets() const -> EdgeInsets
{
    if (auto const manager = GetAreaManager())
    {
        return manager->GetAreaInsets(ViewArea::TitleBar);
    }
    return {};
}

///
/// @brief 
///
auto WindowFrame::GetTitleBarBoundingRects() const -> std::vector<Rect<Dp>>
{
    if (auto const manager = GetAreaManager())
    {
        return manager->GetAreaBounds(ViewArea::TitleBar);
    }
    return {};
}

///
/// @brief Update area manager.
///
auto WindowFrame::UpdateAreaManager() -> void
{
    _connection.Disconnect();

    if (auto const manager = GetAreaManager())
    {
        _connection = EventReceiver::Connect(*manager, *this, &WindowFrame::ReceiveEvent);
        InvalidateLayout();
        InvalidateVisual();
    }
}
}
