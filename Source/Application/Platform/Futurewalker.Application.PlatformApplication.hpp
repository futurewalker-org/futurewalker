// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformApplicationType.hpp"

#include "Futurewalker.Async.Async.hpp" 

#include "Futurewalker.Event.EventType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Platform application.
///
class PlatformApplication : NonCopyable
{
public:
    struct Delegate
    {
        EventFunction sendApplicationEvent;
    };
    PlatformApplication(Delegate const& delegate);

    virtual ~PlatformApplication() = 0;
    virtual auto Run() -> Async<void> = 0;
    virtual auto Exit() -> void = 0;
    virtual auto IsRunning() -> Bool = 0;
    virtual auto IsActive() -> Bool = 0;
    virtual auto IsForeground() -> Bool = 0;

protected:
    virtual auto Initialize() -> void = 0;

    auto SendApplicationEvent(Event& event) -> Async<Bool>;
    auto SendApplicationEventDetached(Event const& event) -> void;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

private:
    Weak<PlatformApplication> _self;
    Delegate _delegate;
};

///
/// @brief Get self.
///
template <class Self>
auto PlatformApplication::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, PlatformApplication>>(self)._self.Lock().template Assume<Self>();
}

///
/// @brief Make derived instance.
///
template <class Derived, class... Args>
auto PlatformApplication::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto key = PassKey<PlatformApplication>();
    auto view = Shared<Derived>::Make(key, std::forward<Args>(args)...);
    static_cast<PlatformApplication&>(*view)._self = view;
    static_cast<PlatformApplication&>(*view).Initialize();
    return view;
}
}
