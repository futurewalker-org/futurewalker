// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TextViewType.hpp"
#include "Futurewalker.Application.TextViewStyle.hpp"
#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"

#include "Futurewalker.Graphics.ShapedTextType.hpp"
#include "Futurewalker.Graphics.TextShaperType.hpp"
#include "Futurewalker.Graphics.TypefaceType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A view that displays text.
///
class TextView final : public View
{
public:
    static auto Make() -> Shared<TextView>;

    auto GetText() const -> String;
    auto SetText(AttributeArg<String> const& text) -> void;

    TextView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Draw(DrawScope& scope) -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;

private:
    auto GetTypeface() const -> Shared<Graphics::Typeface>;
    auto GetFontSize() const -> Graphics::FontSize;
    auto InvalidateLayoutCache() -> void;

private:
    AttributeAccessor<String> _text;
    AttributeAccessor<RGBAColor> _color;
    AttributeAccessor<Graphics::FontSize> _fontSize;
    AttributeAccessor<Graphics::FontWeight> _fontWeight;
    AttributeAccessor<Graphics::FontWidth> _fontWidth;
    AttributeAccessor<Graphics::FontSlant> _fontSlant;
    AttributeAccessor<Graphics::FontFamily> _fontFamily;
    AttributeAccessor<TextViewHorizontalAlignment> _horizontalAlignment;
    AttributeAccessor<TextViewVerticalAlignment> _verticalAlignment;
    Shared<Graphics::TextShaper> _shaper;
    Shared<Graphics::ShapedText> _shapedText;
};
}
}
