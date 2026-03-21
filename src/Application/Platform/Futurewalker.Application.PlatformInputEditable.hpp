// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformInputEditableType.hpp"

#include "Futurewalker.Event.hpp"

#include "Futurewalker.Graphics.TextLayoutInfo.hpp"

#include "Futurewalker.Text.Text.hpp"

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
class PlatformInputEditable : NonCopyable
{
public:
    struct Delegate
    {
        EventFunction sendInputevent;
    };
    PlatformInputEditable(PassKey<PlatformInputEditable>, Delegate const& delegate);

    virtual ~PlatformInputEditable() = 0;

    virtual auto GetText() const -> Text = 0;
    virtual auto SetText(Text const& text) -> void = 0;

    virtual auto GetString(Range<CodePoint> range) const -> String = 0;
    virtual auto GetStringRange() const -> Range<CodePoint> = 0;

    virtual auto GetSelectedRange() const -> Range<CodePoint> = 0;
    virtual auto SetSelectedRange(Range<CodePoint> const& range) -> void = 0;

    virtual auto GetComposingRange() const -> Range<CodePoint> = 0;
    virtual auto SetComposingRange(Range<CodePoint> const& range) -> void = 0;

    virtual auto GetLayoutRect() const -> Rect<Dp> = 0;
    virtual auto SetLayoutRect(Rect<Dp> const& rect) -> void = 0;

    virtual auto GetLayoutInfo() const -> Graphics::TextLayoutInfo = 0;
    virtual auto SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void = 0;

    virtual auto InsertText(String const& text, CodePoint caretPosition) -> void = 0;
    virtual auto InsertLineBreak() -> void = 0;
    virtual auto DeleteSurroundingText(CodePoint before, CodePoint after) -> void = 0;


protected:
    auto SendInputEvent(Event<>& event) -> Async<Bool>;
    auto SendInputEventDetached(Event<>& event) -> Bool;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

private:
    Weak<PlatformInputEditable> _self;
    Delegate _delegate;
};

///
/// @brief Get self.
///
template <class Self>
auto PlatformInputEditable::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, PlatformInputEditable>>(self)._self.Lock().template UnsafeAs<Self>();
}

///
/// @brief Get derived instance.
///
template <class Derived, class... Args>
auto PlatformInputEditable::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto key = PassKey<PlatformInputEditable>();
    auto view = Shared<Derived>::Make(key, std::forward<Args>(args)...);
    static_cast<PlatformInputEditable&>(*view)._self = view;
    return view;
}
}
}
