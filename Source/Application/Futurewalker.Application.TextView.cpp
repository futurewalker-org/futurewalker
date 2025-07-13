// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TextView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.ViewLayoutFunction.hpp"

#include "Futurewalker.Attribute.AttributeObserver.hpp"

#include "Futurewalker.Graphics.FontManager.hpp"
#include "Futurewalker.Graphics.TextShaper.hpp"
#include "Futurewalker.Graphics.ShapedText.hpp"
#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.TextLayoutInfo.hpp"

namespace FW_DETAIL_NS
{
namespace
{
FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(String, TextViewAttributeText, String());
}

auto TextView::Make() -> Shared<TextView>
{
    return View::MakeDerived<TextView>();
}

void TextView::SetText(AttributeArg<String> text)
{
    _text.SetAttributeArg(text);
}

TextView::TextView(PassKey<View> key)
  : View(key)
{
}

auto TextView::Initialize() -> void
{
    _shaper = Graphics::TextShaper::Make();

    _text.BindAttribute(*this, TextViewAttributeText);
    _color.BindAttribute(*this, TextViewStyle::Color);
    _fontSize.BindAttribute(*this, TextViewStyle::FontSize);
    _fontWeight.BindAttribute(*this, TextViewStyle::FontWeight);
    _fontWidth.BindAttribute(*this, TextViewStyle::FontWidth);
    _fontSlant.BindAttribute(*this, TextViewStyle::FontSlant);
    _fontFamily.BindAttribute(*this, TextViewStyle::FontFamily);

    EventReceiver::Connect(_text, *this, &TextView::ReceiveEvent);
    EventReceiver::Connect(_color, *this, &TextView::ReceiveEvent);
    EventReceiver::Connect(_fontSize, *this, &TextView::ReceiveEvent);
    EventReceiver::Connect(_fontWeight, *this, &TextView::ReceiveEvent);
    EventReceiver::Connect(_fontWidth, *this, &TextView::ReceiveEvent);
    EventReceiver::Connect(_fontSlant, *this, &TextView::ReceiveEvent);
    EventReceiver::Connect(_fontFamily, *this, &TextView::ReceiveEvent);
    EventReceiver::Connect(_horizontalAlignment, *this, &TextView::ReceiveEvent);
    EventReceiver::Connect(_verticalAlignment, *this, &TextView::ReceiveEvent);
}

auto TextView::Measure(MeasureScope& scope) -> void
{
    const auto& parameter = scope.GetParameter();
    const auto& widthConstraints = parameter.GetWidthConstraints();
    const auto& heightConstraints = parameter.GetHeightConstraints();

    if (_shaper)
    {
        auto const text = _text.GetValueOrDefault();
        auto const typeface = GetTypeface();
        auto const size = GetFontSize();
        _shapedText = _shaper->Shape(text, typeface, size, widthConstraints.GetMax());
    }

    if (_shapedText)
    {
        const auto size = _shapedText->GetBounds().GetSize();
        const auto width = AxisConstraints::Constrain(widthConstraints, size.GetWidth());
        const auto height = AxisConstraints::Constrain(heightConstraints, size.GetHeight());
        scope.SetMeasuredSize(width, height);
    }
    else
    {
        scope.SetMeasuredSize(widthConstraints.GetMin(), heightConstraints.GetMin());
    }
}

auto TextView::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();

    const auto rect = GetContentRect();

    auto const color = _color.GetValueOrDefault();
    auto const isRTL = GetLayoutDirection() == LayoutDirection::RightToLeft;

    if (_shapedText)
    {
        const auto hAlign = _horizontalAlignment.GetValueOr(TextViewHorizontalAlignment::Center);
        const auto vAlign = _verticalAlignment.GetValueOr(TextViewVerticalAlignment::Middle);

        const auto size = _shapedText->GetLayoutInfo().GetSize();

        auto x = Dp(0);
        if ((hAlign == TextViewHorizontalAlignment::Leading && isRTL) || (hAlign == TextViewHorizontalAlignment::Trailing && !isRTL))
        {
            x = rect.GetLeft() + rect.GetWidth() - size.GetWidth();
        }
        else if (hAlign == TextViewHorizontalAlignment::Center)
        {
            x = rect.GetLeft() + (rect.GetWidth() - size.GetWidth()) / 2;
        }

        auto y = Dp(0);
        if (vAlign == TextViewVerticalAlignment::Bottom)
        {
            y = rect.GetTop() + rect.GetHeight() - size.GetHeight();
        }
        else if (vAlign == TextViewVerticalAlignment::Middle)
        {
            y = rect.GetTop() + (rect.GetHeight() - size.GetHeight()) / 2;
        }

        const auto alignedX = ViewLayoutFunction::AlignToPixelGridByRound(x, *this);
        const auto alignedY = ViewLayoutFunction::AlignToPixelGridByRound(y, *this);

        scene.PushTranslate({
          .x = alignedX,
          .y = alignedY,
        });
        scene.AddText({
          .shaped = _shapedText,
          .color = color,
        });
        scene.Pop({});
    }
}

auto TextView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateLayoutCache();
        InvalidateLayout();
        InvalidateVisual();
    }
    co_return false;
}

auto TextView::GetTypeface() const -> Shared<Graphics::Typeface>
{
    if (auto manager = Locator::GetInstance<Graphics::FontManager>())
    {
        auto const fontFamily = _fontFamily.GetValueOrDefault();
        auto const fontWeight = _fontWeight.GetValueOr(Graphics::FontWeight::Normal());
        auto const fontWidth = _fontWidth.GetValueOr(Graphics::FontWidth::Normal());
        auto const fontSlant = _fontSlant.GetValueOr(Graphics::FontSlant::Upright);
        return manager->FindTypefaceByFamilyAndStyle(fontFamily, Graphics::FontStyle(fontWeight, fontWidth, fontSlant));
    }
    return {};
}

auto TextView::GetFontSize() const -> Graphics::FontSize
{
    return _fontSize.GetValueOr(16);
}

auto TextView::InvalidateLayoutCache() -> void
{
    _shapedText = nullptr;
}
}
