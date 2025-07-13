// SPDX-License-Identifier: MPL-2.0
#include "Futurewalker.Application.Application.hpp"
#include "Futurewalker.Application.ApplicationEvent.hpp"
#include "Futurewalker.Application.ApplicationContext.hpp"
#include "Futurewalker.Application.ApplicationTheme.hpp"
#include "Futurewalker.Application.MainThread.hpp"
#include "Futurewalker.Application.ViewLayerManager.hpp"

#include "Futurewalker.Application.PlatformApplication.hpp"
#include "Futurewalker.Application.PlatformApplicationEvent.hpp"
#include "Futurewalker.Application.PlatformApplicationContext.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Async.ThreadPool.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Constructor.
///
Application::Application(PassKey<Application>, ApplicationOptions const& options)
  : _id {options.identifier}
{
    EventReceiver::Delegate delegate = {
      .dispatchEvent = [&](Event<>& event, EventFunction const& dispatch) -> Async<Bool> { co_return co_await DispatchEvent(event, dispatch); },
    };
    _eventReceiver = EventReceiver::Make(delegate);
    _propertyStore = Unique<PropertyStore>::Make();
    _platformContext = Locator::Resolve<PlatformApplicationContext>();
    _platformObject = _platformContext->MakePlatformApplication({.sendApplicationEvent = [&](Event<>& event) -> Async<Bool> { co_return co_await HandlePlatformApplicationEvent(event); }});
    _context = Locator::ResolveWithDefault<ApplicationContext>();
    _theme = Locator::ResolveWithDefault<ApplicationTheme>();
    _threadPool = Locator::ResolveWithDefault<ThreadPool>();
    _viewLayerManager = Locator::ResolveWithDefault<ViewLayerManager>();
}

///
/// @brief Run application's main event loop.
///
/// This function awaits until event loop exits by Exit().
///
/// @note This function must be called from main thread.
/// @note This function cannot be nested.
///
auto Application::Run() -> Async<void>
{
    if (MainThread::IsMainThread())
    {
        co_return co_await _platformObject->Run();
    }
    FW_DEBUG_ASSERT(false);
    co_return;
}

///
/// @brief Requests the application to exit.
///
/// @note This function must be called from main thread.
///
/// @return True if the application exited successfully.
///
auto Application::Exit() -> Async<Bool>
{
    if (MainThread::IsMainThread())
    {
        if (_platformObject->IsRunning())
        {
            auto cancelled = Bool(false);
            auto event = Event<>(Event<ApplicationEvent::ExitRequested>());
            if (co_await SendEvent(event))
            {
                if (event.Is<ApplicationEvent::ExitRequested>())
                {
                    cancelled = event.As<ApplicationEvent::ExitRequested>()->IsCancelled();
                }
            }

            if (!cancelled)
            {
                _platformObject->Exit();
            }
            co_return !cancelled;
        }
        co_return true;
    }
    FW_DEBUG_ASSERT(false);
    co_return false;
}

///
/// @brief Returns whether the application is active or not.
///
auto Application::IsActive() const -> Bool
{
    return _platformObject->IsActive(); 
}

///
/// @brief Returns whether the application is foreground or not.
///
auto Application::IsForeground() const -> Bool
{
    return _platformObject->IsForeground();
}

///
/// @brief Send event to Application.
///
auto Application::SendEvent(Event<>& event) -> Async<Bool>
{
    co_return co_await GetEventReceiver().SendEvent(event);
}

///
/// @brief Get tracker.
///
auto Application::GetTracker() -> Tracker&
{
    return GetEventReceiver().GetTracker();
}

///
/// @brief Get tracker.
///
auto Application::GetTracker() const -> Tracker const&
{
    return GetEventReceiver().GetTracker();
}

///
/// @brief Get event receiver.
///
auto Application::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

///
/// @brief Get event receiver.
///
auto Application::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}

///
/// @brief Get property store.
///
auto Application::GetPropertyStore() -> PropertyStore&
{
    return *_propertyStore;
}

///
/// @brief Get property store.
///
auto Application::GetPropertyStore() const -> PropertyStore const&
{
    return *_propertyStore;
}

///
/// @brief Get attribute node.
///
auto Application::GetAttributeNode() -> AttributeNode&
{
    return _context->GetAttributeNode();
}

///
/// @brief Get attribute node.
///
auto Application::GetAttributeNode() const -> AttributeNode const&
{
    return _context->GetAttributeNode();
}

///
/// @brief
///
auto Application::Initialize() -> void
{
}

///
/// @brief
///
auto Application::GetSelfBase() -> Shared<Application>
{
    return _self.Lock();
}

///
/// @brief
///
auto Application::GetSelfBase() const -> Shared<Application const>
{
    return _self.Lock();
}

///
/// @brief
///
auto Application::SetSelfBase(Shared<Application> const& self) -> void
{
    _self = self;
}

///
/// @brief
///
/// @param event
/// @param dispatch
///
auto Application::DispatchEvent(Event<>& event, EventFunction const& dispatch) -> Async<Bool>
{
    co_return co_await dispatch(event);
}

///
/// @brief
///
/// @param event
///
/// @return
///
auto Application::HandlePlatformApplicationEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PlatformApplicationEvent::Started>())
    {
        auto e = Event<>(Event<ApplicationEvent::Started>());
        co_return co_await SendEvent(e);
    }
    else if (event.Is<PlatformApplicationEvent::Exiting>())
    {
        auto e = Event<>(Event<ApplicationEvent::ExitRequested>());
        co_return co_await SendEvent(e);
    }
    else if (event.Is<PlatformApplicationEvent::Exited>())
    {
        auto e = Event<>(Event<ApplicationEvent::Exited>());
        co_return co_await SendEvent(e);
    }
    else if (event.Is<PlatformApplicationEvent::ActiveChanged>())
    {
        const auto parameter = event.As<PlatformApplicationEvent::ActiveChanged>();
        auto applicationEventParameter = Event<ApplicationEvent::ActiveChanged>::Make();
        applicationEventParameter->SetActive(parameter->IsActive());
        auto applicationEvent = Event<>(applicationEventParameter);
        co_await SendEvent(applicationEvent);
    }
    else if (event.Is<PlatformApplicationEvent::ForegroundChanged>())
    {
        const auto parameter = event.As<PlatformApplicationEvent::ForegroundChanged>();
        auto applicationEventParameter = Event<ApplicationEvent::ForegroundChanged>::Make();
        applicationEventParameter->SetForeground(parameter->IsForeground());
        auto applicationEvent = Event<>(applicationEventParameter);
        co_await SendEvent(applicationEvent);
    }
    co_return false;
}
}
