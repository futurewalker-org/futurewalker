// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformApplicationThemeContextType.hpp"
#include "Futurewalker.Application.PlatformApplicationTheme.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformApplicationThemeContext : NonCopyable
{
public:
    PlatformApplicationThemeContext(PassKey<PlatformApplicationThemeContext>);

    virtual ~PlatformApplicationThemeContext() = 0;
    virtual auto MakeApplicationTheme(PlatformApplicationTheme::Delegate const& delegate) -> Shared<PlatformApplicationTheme> = 0;

protected:
    virtual void Initialize() = 0;

    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

private:
    Weak<PlatformApplicationThemeContext> _self;
};

template <class Derived, class... Args>
auto PlatformApplicationThemeContext::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto key = PassKey<PlatformApplicationThemeContext>();
    auto view = Shared<Derived>::Make(key, std::forward<Args>(args)...);
    static_cast<PlatformApplicationThemeContext&>(*view)._self = view;
    static_cast<PlatformApplicationThemeContext&>(*view).Initialize();
    return view;
}

template <class Self>
auto PlatformApplicationThemeContext::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, PlatformApplicationThemeContext>>(self)._self.Lock().template UnsafeAs<Self>();
}
}
}
