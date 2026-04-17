// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ScrollView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.PointerEvent.hpp"
#include "Futurewalker.Application.KeyEvent.hpp"
#include "Futurewalker.Application.ViewAttribute.hpp"
#include "Futurewalker.Application.ViewLayoutFunction.hpp"

namespace FW_DETAIL_NS
{
auto ScrollView::Make() -> Shared<ScrollView>
{
    return View::MakeDerived<ScrollView>();
}

auto ScrollView::MakeWithContent(Shared<View> const& content) -> Shared<ScrollView>
{
    auto scrollView = Make();
    scrollView->SetContent(content);
    return scrollView;
}

ScrollView::ScrollView(PassKey<View> key)
  : View(key)
{
}

auto ScrollView::SetContent(Shared<View> const& content) -> void
{
    _containerView->SetContent(content);
}

auto ScrollView::GetContent() -> Shared<View>
{
    return _containerView->GetContent();
}

auto ScrollView::GetDirection() const -> Flags<ScrollViewDirection>
{
    return _direction;
}

auto ScrollView::SetDirection(Flags<ScrollViewDirection> const direction) -> void
{
    if (_direction != direction)
    {
        _direction = direction;
        InvalidateLayout();
    }
}

auto ScrollView::Initialize() -> void
{
    SetPointerTrackingFlags(ViewPointerTrackingFlag::All);

    _containerView = ContainerView::Make();
    AddChildBack(_containerView);

    _horizontalScrollFactor.BindAttribute(*this, ViewAttribute::HorizontalScrollFactor);
    _verticalScrollFactor.BindAttribute(*this, ViewAttribute::VerticalScrollFactor);

    EventReceiver::Connect(*this, *this, &ScrollView::ReceiveEvent);
}

auto ScrollView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();
    auto const width = parameter.GetWidthConstraints();
    auto const height = parameter.GetHeightConstraints();

    auto childWidth = parameter.GetWidthConstraints();
    if ((_direction & ScrollViewDirection::Horizontal) != ScrollViewDirection::None)
    {
        childWidth = AxisConstraints::MakeUnconstrained();
    }

    auto childHeight = parameter.GetHeightConstraints();
    if ((_direction & ScrollViewDirection::Vertical) != ScrollViewDirection::None)
    {
        childHeight = AxisConstraints::MakeUnconstrained();
    }

    auto childSize = Size<Dp>();
    ForEachVisibleChild([&](View& view) { childSize = Size<Dp>::Max(childSize, scope.MeasureChild(view, childWidth, childHeight)); });

    auto const measuredWidth = width.IsBounded() ? width.GetMax() : AxisConstraints::Constrain(width, childSize.width);
    auto const measuredHeight = height.IsBounded() ? height.GetMax() : AxisConstraints::Constrain(height, childSize.height);
    scope.SetMeasuredSize(measuredWidth, measuredHeight);
}

auto ScrollView::Arrange(ArrangeScope& scope) -> void
{
    auto childSize = Size<Dp>();
    ForEachVisibleChild([&](View& view) { childSize = Size<Dp>::Max(childSize, scope.GetMeasuredSize(view)); });

    auto const contentRect = GetContentRect();

    if ((_direction & ScrollViewDirection::Horizontal) != ScrollViewDirection::None)
    {
        auto const maxOffset = Dp::Max(0, childSize.width - contentRect.GetWidth());
        auto const deltaX = Dp::Min(Dp::Max(0, _offset.x), maxOffset);
        _offset.x = deltaX;
    }
    else if ((_direction & ScrollViewDirection::Vertical) != ScrollViewDirection::None)
    {
        auto const maxOffset = Dp::Max(0, childSize.height - contentRect.GetHeight());
        auto const deltaY = Dp::Min(Dp::Max(0, _offset.y), maxOffset);
        _offset.y = deltaY;
    }

    ForEachChild([&](View& view) {
        auto const x = ViewLayoutFunction::AlignToPixelGridByRound(-_offset.x, *this);
        auto const y = ViewLayoutFunction::AlignToPixelGridByRound(-_offset.y, *this);
        scope.ArrangeChild(view, {x, y});
    });
}

auto ScrollView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PointerEvent::Action::Scroll>())
    {
        auto const wheelEvent = event.As<PointerEvent::Action::Scroll>();
        auto const precision = wheelEvent->GetPrecision();
        auto const deltaX = wheelEvent->GetDeltaX();
        auto const deltaY = wheelEvent->GetDeltaY();

        auto const horizontalScrollFactor = _horizontalScrollFactor.GetValueOr(0.0);
        auto const verticalScrollFactor = _verticalScrollFactor.GetValueOr(0.0);

        if ((_direction & ScrollViewDirection::Horizontal) != ScrollViewDirection::None)
        {
            if (deltaX != 0)
            {
                auto const distance = precision == PointerScrollPrecision::Coarse ? deltaX * Dp(horizontalScrollFactor) : deltaX;
                _offset.x -= distance;
                InvalidateLayout();
            }
        }
        else if ((_direction & ScrollViewDirection::Vertical) != ScrollViewDirection::None)
        {
            if (deltaY != 0)
            {
                auto const distance = precision == PointerScrollPrecision::Coarse ? deltaY * Dp(verticalScrollFactor) : deltaY;
                _offset.y -= distance;
                InvalidateLayout();
            }
        }
        co_return true;
    }
    else if (event.Is<PointerEvent::Action::Swipe>())
    {
        auto const swipeEvent = event.As<PointerEvent::Action::Swipe>();
        auto const deltaX = swipeEvent->GetDeltaX();
        auto const deltaY = swipeEvent->GetDeltaY();

        if ((_direction & ScrollViewDirection::Horizontal) != ScrollViewDirection::None)
        {
            if (deltaX != 0)
            {
                _offset.x -= deltaX;
                InvalidateLayout();
            }
        }
        else if ((_direction & ScrollViewDirection::Vertical) != ScrollViewDirection::None)
        {
            if (deltaY != 0)
            {
                _offset.y -= deltaY;
                InvalidateLayout();
            }
        }
        co_return true;
    }
    co_return false;
}
}
