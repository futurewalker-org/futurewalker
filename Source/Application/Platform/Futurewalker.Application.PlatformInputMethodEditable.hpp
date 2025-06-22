// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformInputMethodEditableType.hpp"

#include "Futurewalker.Event.hpp"

#include "Futurewalker.Graphics.TextLayoutInfo.hpp"

#include "Futurewalker.Geometry.Rect.hpp"

#include "Futurewalker.Unit.Unit.hpp" 

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Range.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformInputMethodEditable : NonCopyable
{
public:
    struct Delegate
    {
        EventFunction sendInputevent;
    };
    PlatformInputMethodEditable(Delegate const& delegate);

    virtual ~PlatformInputMethodEditable() = 0;

    virtual auto GetText() const -> String = 0;
    virtual auto GetText(Range<CodePoint> range) const -> String = 0;
    virtual auto GetTextRange() const -> Range<CodePoint> = 0;
    virtual auto SetText(String const& text) -> void = 0;

    virtual auto GetSelectedRange() const -> Range<CodePoint> = 0;
    virtual auto SetSelectedRange(Range<CodePoint> const& range) -> void = 0;

    virtual auto GetComposingRange() const -> Range<CodePoint> = 0;
    virtual auto SetComposingRange(Range<CodePoint> const& range) -> void = 0;

    virtual auto GetLayoutRect() const -> Rect<Dp> = 0;
    virtual auto SetLayoutRect(Rect<Dp> const& rect) -> void = 0;

    virtual auto GetLayoutInfo() const -> Graphics::TextLayoutInfo = 0;
    virtual auto SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void = 0;

    virtual auto InsertText(String const& text, CodePoint caretPosition) -> void = 0;
    virtual auto DeleteSurroundingText(CodePoint before, CodePoint after) -> void = 0;

protected:
    auto SendInputEvent(Event& event) -> Async<Bool>;

private:
    Delegate _delegate;
};
}
}
