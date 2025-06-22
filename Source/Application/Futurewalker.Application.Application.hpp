// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ApplicationType.hpp"
#include "Futurewalker.Application.ApplicationContextType.hpp"
#include "Futurewalker.Application.ViewLayerManagerType.hpp"

#include "Futurewalker.Application.PlatformApplicationType.hpp"
#include "Futurewalker.Application.PlatformApplicationContextType.hpp"

#include "Futurewalker.Attribute.AttributeNodeType.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PropertyStoreType.hpp"

#include "Futurewalker.Async.Async.hpp"
#include "Futurewalker.Async.ThreadPoolType.hpp"

#include "Futurewalker.Event.EventType.hpp"
#include "Futurewalker.Event.EventReceiverType.hpp"

#include "Futurewalker.Signal.TrackerType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Application.
///
class Application : NonCopyable
{
public:
    Application(PassKey<Application>, ApplicationOptions const& options);
    virtual ~Application() = default;

    auto SendEvent(Event<>& event) -> Async<Bool>;

    auto Run() -> Async<void>;
    auto Exit() -> Async<Bool>;

    auto IsActive() const -> Bool;
    auto IsForeground() const -> Bool;

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

    auto GetPropertyStore() -> PropertyStore&;
    auto GetPropertyStore() const -> PropertyStore const&;

    auto GetAttributeNode() -> AttributeNode&;
    auto GetAttributeNode() const -> AttributeNode const&;

protected:
    virtual auto Initialize() -> void;

    template <Concepts::DerivedFrom<Application> Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

private:
    auto GetSelfBase() -> Shared<Application>;
    auto GetSelfBase() const -> Shared<Application const>;
    auto SetSelfBase(Shared<Application> const& self) -> void;

    auto DispatchEvent(Event<>& event, EventFunction const& dispatch) -> Async<Bool>;
    auto HandlePlatformApplicationEvent(Event<>& event) -> Async<Bool>;

private:
    Weak<Application> _self;
    Unique<EventReceiver> _eventReceiver;
    Unique<PropertyStore> _propertyStore;
    Shared<PlatformApplicationContext> _platformContext;
    Shared<PlatformApplication> _platformObject;
    Shared<ApplicationContext> _context;
    Shared<ThreadPool> _threadPool;
    Shared<ViewLayerManager> _viewLayerManager;
    String _id;
};

///
/// @brief Make derived application object.
///
/// @param[in] args Arguments for constructor of application class.
///
template <Concepts::DerivedFrom<Application> Derived, class... Args>
auto Application::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto app = Shared<Derived>::Make(PassKey<Application>(), std::forward<Args>(args)...);
    static_cast<Application&>(*app).SetSelfBase(app);
    static_cast<Application&>(*app).Initialize();
    return app;
}

///
/// @brief 
///
/// @param self 
///
template <class Self>
auto Application::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, Application>>(self).GetSelfBase().template Assume<Self>();
}
}
}
