// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformApplicationThemeType.hpp"
#include "Futurewalker.Application.PlatformApplicationThemeContextType.hpp"
#include "Futurewalker.Application.ThemeType.hpp"

#include "Futurewalker.Event.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformApplicationTheme : NonCopyable
{
public:
    struct Delegate
    {
        EventFunction sendThemeEvent;
    };
    PlatformApplicationTheme(PassKey<PlatformApplicationTheme>, Delegate const& delegate, Shared<PlatformApplicationThemeContext> const& context);

    virtual ~PlatformApplicationTheme() = 0;
    virtual auto GetSystemBrightness() const -> ThemeBrightness = 0;

protected:
    auto GetContext() -> Shared<PlatformApplicationThemeContext>;
    auto GetContext() const -> Shared<PlatformApplicationThemeContext const>;

    auto SendThemeEvent(Event<>& event) -> Async<Bool>;
    auto SendThemeEventDetached(Event<> const& event) -> void;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

private:
    Weak<PlatformApplicationTheme> _self;
    Delegate _delegate;
    Shared<PlatformApplicationThemeContext> _context;
};

template <class Self>
auto PlatformApplicationTheme::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, PlatformApplicationTheme>>(self)._self.Lock().template Assume<Self>();
}

template <class Derived, class... Args>
auto PlatformApplicationTheme::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto key = PassKey<PlatformApplicationTheme>();
    auto view = Shared<Derived>::Make(key, std::forward<Args>(args)...);
    static_cast<PlatformApplicationTheme&>(*view)._self = view;
    return view;
}

class PlatformApplicationThemeEvent : public EventParameter
{
public:
    class SystemBrightnessChanged;
};

class PlatformApplicationThemeEvent::SystemBrightnessChanged : public PlatformApplicationThemeEvent
{
};
}
}
