// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TextEditType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.InputMethodType.hpp"
#include "Futurewalker.Application.InputMethodEditableType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Range.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TextEdit : public View
{
public:
    static auto Make() -> Shared<TextEdit>;

    TextEdit(PassKey<View>);

protected:
    auto Initialize() -> void override;
    auto Draw(DrawScope& scope) -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Arrange(ArrangeScope& scope) -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveInputEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveKeyEvent(Event<>& event) -> Async<Bool>;
    auto ReceivePointerEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveFocusEvent(Event<>& event) -> Async<Bool>;

private:
    auto InternalSetInputMethod(Shared<InputMethod> const& context) -> void;
    auto InternalInsertText(String const& text, CodePoint newSelection) -> void;
    auto InternalGetSelectedRange() -> Range<CodePoint>;
    auto InternalSetSelectedRange(Range<CodePoint> range) -> void;
    auto InternalGetComposingRange() -> Range<CodePoint>;
    auto InternalSetComposingRange(Range<CodePoint> range) -> void;
    auto InternalGetText(Range<CodePoint> range) -> String;
    auto InternalGetTextRange() const -> Range<CodePoint>;
    auto InternalDeleteBackward() -> void;
    auto InternalDeleteForward() -> void;

private:
    class Impl;
    Shared<InputMethod> _inputMethod;
    Shared<InputMethodEditable> _inputMethodEditable;
};
}
}
