// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.DisclosureTextButton.hpp"
#include "Futurewalker.Component.Lamp.DisclosureView.hpp"
#include "Futurewalker.Component.Lamp.TextButton.hpp"
#include "Futurewalker.Component.Lamp.TextView.hpp"
#include "Futurewalker.Component.Lamp.IconView.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"

#include "Futurewalker.Application.FlexLayout.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.ClipView.hpp"
#include "Futurewalker.Application.TransformView.hpp"
#include "Futurewalker.Application.PaddingView.hpp"

#include "Futurewalker.Resource.ResourceManager.hpp"
#include "Futurewalker.Resource.Resource.hpp"

#include "Futurewalker.Graphics.SvgDocument.hpp"

#include "Resource/Futurewalker.Component.Lamp.hpp"
#include "Resource/Futurewalker.Component.Lamp.Icon.hpp"

#include "Futurewalker.Geometry.Matrix.hpp"

#include "Futurewalker.Unit.UnitFunction.hpp"

#include "Futurewalker.Animation.AnimationInterpolation.hpp"  

namespace FW_LAMP_DETAIL_NS
{
auto DisclosureTextButton::Make() -> Shared<DisclosureTextButton>
{
    return View::MakeDerived<DisclosureTextButton>();
}
auto DisclosureTextButton::MakeWithText(AttributeArg<String> const& text) -> Shared<DisclosureTextButton>
{
    auto button = Make();
    button->SetText(text);
    return button;
}

auto DisclosureTextButton::MakeWithTextAndIcon(AttributeArg<String> const& text, AttributeArg<Icon> const& icon) -> Shared<DisclosureTextButton>
{
    auto button = Make();
    button->SetText(text);
    button->SetIcon(icon);
    return button;
}

DisclosureTextButton::DisclosureTextButton(PassKey<View> key)
  : View(key)
  , _openRatioAnimator(0.0)
{
}

auto DisclosureTextButton::SetText(AttributeArg<String> const& text) -> void
{
    _buttonText->SetText(text);
}

auto DisclosureTextButton::SetIcon(AttributeArg<Icon> const& icon) -> void
{
    _buttonIcon->SetIcon(icon);
}

auto DisclosureTextButton::SetContent(Shared<View> content) -> void
{
    _disclosure->SetContent(content);
}

auto DisclosureTextButton::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Float64, OpenRatioAttribute, 0.0);
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(Matrix3x3<Dp>, IconTransformAttribute, [](Float64 const& openRatio) {
        auto const angle = static_cast<Degree>(openRatio * -180.0);
        return Matrix3x3<Dp>::MakeRotation(UnitFunction::ConvertDegreeToRadian(angle));
    }, OpenRatioAttribute);
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(EdgeInsets, AttributeIconMargin, [](Dp const& s) { return EdgeInsets(0, 0, s, 0); }, TextButtonStyle::IconSpace);

    auto loadIcon = [](auto iconId) {
        auto const resource = ResourceManager::GetResource(M::Futurewalker::Component::Lamp);
        auto const file = resource->LoadFile(iconId);
        return Icon::MakeFromSvgDocument(Graphics::SvgDocument::LoadFromStream(file));
    };

    auto iconView = IconView::Make();
    auto textView = TextView::Make();
    FlexLayout::SetChildGrowFactor(textView, 1);
    FlexLayout::SetChildShrinkFactor(textView, 1);
    FlexLayout::SetChildMainAxisFlexibility(textView, FlexLayoutMainAxisFlexibility::Expand);

    auto trailingIconView = IconView::MakeWithIcon(loadIcon(R::Futurewalker::Component::Lamp::Icon::ChevronDown));
    AttributeNode::SetReference<IconViewStyle::Size>(*trailingIconView, Style::IconSizeExtraSmall);
    auto trailingIconViewTransform = TransformView::MakeWithContent(trailingIconView);
    trailingIconViewTransform->SetTransform(IconTransformAttribute);

    auto buttonFlex = FlexLayout::Make();
    buttonFlex->SetDirection(FlexLayoutDirection::Row);
    buttonFlex->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    buttonFlex->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    buttonFlex->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
    buttonFlex->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
    buttonFlex->AddChild(PaddingView::MakeWithPaddingAndContent(AttributeIconMargin, iconView));
    buttonFlex->AddChild(textView);
    buttonFlex->AddChild(trailingIconViewTransform);

    auto button = TextButton::MakeWithContent(buttonFlex);
    auto disclosure = DisclosureView::Make();
    disclosure->SetOpenRatio(OpenRatioAttribute);

    AddChildBack(button);
    AddChildBack(disclosure);

    _buttonIcon = iconView;
    _buttonText = textView;
    _button = button;
    _disclosure = disclosure;

    _openRatioAnimator.BindAnimationTicker(*this);
    _openRatioAnimator.SetDuration(TimeInterval::MakeFromMilliseconds(200));
    _openRatioAnimator.SetInterpolation(AnimationInterpolation::MakeLinear());
    _openRatioAnimator.SetUpdateCallback([&](auto const& openRatio) { AttributeNode::SetValue<OpenRatioAttribute>(*this, openRatio); });

    EventReceiver::Connect(*_button, *this, &DisclosureTextButton::ReceiveEvent);
}

auto DisclosureTextButton::Measure(MeasureScope& scope) -> void
{
    auto const widthConstraints = scope.GetParameter().GetWidthConstraints();
    auto const heightConstraints = scope.GetParameter().GetHeightConstraints();
    auto const buttonSize = scope.MeasureChild(_button, widthConstraints, AxisConstraints::MakeUnconstrained());
    auto const contentWidthConstraints = AxisConstraints::MakeExact(buttonSize.width);
    auto const contentHeightConstraints = AxisConstraints::Offset(heightConstraints, -buttonSize.height);
    auto const contentSize = scope.MeasureChild(_disclosure, contentWidthConstraints, contentHeightConstraints);
    auto const measuredHeight = AxisConstraints::Constrain(heightConstraints, contentSize.height + buttonSize.height);
    scope.SetMeasuredSize(buttonSize.width, measuredHeight);
}

auto DisclosureTextButton::Arrange(ArrangeScope& scope) -> void
{
    auto const buttonSize = scope.GetMeasuredSize(_button);
    scope.ArrangeChild(_button, Point<Dp>());
    scope.ArrangeChild(_disclosure, Point<Dp>(0, buttonSize.height));
}

auto DisclosureTextButton::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<TextButtonEvent>())
    {
        if (event.Is<TextButtonEvent::Press>())
        {
            _open = !_open;
            _openRatioAnimator.AnimateTo(_open ? 1.0 : 0.0);
        }
        return true;
    }
    return false;
}
}
