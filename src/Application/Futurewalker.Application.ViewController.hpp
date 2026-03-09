// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ViewControllerType.hpp"
#include "Futurewalker.Event.EventReceiver.hpp"
#include "Futurewalker.Application.ViewType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief ViewController class.
///
template <class ViewType>
class ViewController : NonCopyable
{
public:
    ///
    /// @brief Constructor.
    ///
    ViewController(PassKey<ViewController>)
    {
        _eventReceiver = EventReceiver::Make();
    }

    ///
    /// @brief Get view.
    ///
    /// @note The view will be created when this function is called for the first time.
    ///
    auto GetView() -> Shared<ViewType>
    {
        if (!_view)
        {
            _view = MakeView();
        }
        return _view;
    }

    auto SendEvent(Event<>& event) -> Async<Bool>
    {
        co_return co_await GetEventReceiver().SendEvent(event);
    }

    auto SendEventDetached(Event<>& event) -> Bool
    {
        return GetEventReceiver().SendEventDetached(event);
    }

    auto GetEventReceiver() -> EventReceiver&
    {
        return *_eventReceiver;
    }

    auto GetEventReceiver() const -> EventReceiver const&
    {
        return *_eventReceiver;
    }

    auto GetTracker() -> Tracker&
    {
        return _eventReceiver->GetTracker();
    }

    auto GetTracker() const -> Tracker const&
    {
        return _eventReceiver->GetTracker();
    }

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>
    {
        return static_cast<TypeTraits::PropagateCVRef<Self&, ViewController>>(self)._self.Lock().template UnsafeAs<Self>();
    }

protected:
    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>
    {
        auto key = PassKey<ViewController>();
        auto controller = Shared<Derived>::Make(key, std::forward<Args>(args)...);
        static_cast<ViewController&>(*controller)._self = controller;
        static_cast<ViewController&>(*controller).Initialize();
        return controller;
    }

private:
    virtual auto Initialize() -> void {}
    virtual auto MakeView() -> Shared<ViewType> = 0;

private:
    Weak<ViewController> _self;
    Shared<ViewType> _view;
    Unique<EventReceiver> _eventReceiver;
};
}
}
