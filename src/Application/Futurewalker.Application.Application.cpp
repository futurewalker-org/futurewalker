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
        .dispatchEvent = [&](Event<>& event, EventFunction const& dispatch) -> Bool { return DispatchEvent(event, dispatch); },
    };
    _eventReceiver = EventReceiver::Make(delegate);
    _propertyStore = Unique<PropertyStore>::Make();
    _platformContext = Locator::Resolve<PlatformApplicationContext>();
    _platformObject = _platformContext->MakePlatformApplication({.sendApplicationEvent = [&](Event<>& event) -> Bool { return HandlePlatformApplicationEvent(event); }});
    _context = Locator::ResolveWithDefault<ApplicationContext>();
    _theme = Locator::ResolveWithDefault<ApplicationTheme>();
    _threadPool = Locator::ResolveWithDefault<ThreadPool>();
    _viewLayerManager = Locator::ResolveWithDefault<ViewLayerManager>();
}

///
/// @brief Start running the application.
///
/// After calling this function, the application will send ApplicationEvent::Started event to itself.
/// On some platforms, this function will never return. Handle ApplicationEvent::Quitting to perform cleanup when application is exiting.
///
/// To manually request the application to exit, call Application::RequestQuit().
/// When the application is requested to exit, ApplicationEvent::QuitRequested will be sent to application.
/// You can cancel the request by calling ApplicationEvent::QuitRequested::SetCancelled(true).
/// Note that some platforms do not allow application to exit so RequestQuit() may immediately return false.
///
/// @note This function must be called from main thread.
/// @note This function cannot be called more than once.
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
/// Calling this function will send ApplicationEvent::QuitRequested on supported platforms.
/// The request may be cancelled by calling ApplicationEvent::QuitRequested::SetCancelled(true) in the event handler.
/// When the application is actually exiting, ApplicationEvent::Quitting will be sent to application.
///
/// @note This function must be called from main thread.
///
/// @return True if the application exited successfully.
///
auto Application::RequestQuit() -> void
{
    if (_platformObject)
    {
        _platformObject->RequestQuit();
    }
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
auto Application::SendEvent(Event<>& event) -> Bool
{
    return GetEventReceiver().SendEvent(event);
}

///
/// @brief Get tracker.
///
auto Application::GetTracker() -> Weak<void>
{
    return _self;
}

///
/// @brief Get tracker.
///
auto Application::GetTracker() const -> Weak<void const>
{
    return _self;
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
auto Application::SetMainMenu(Menu const& menu) -> void
{
    if (_platformObject)
    {
        _platformObject->SetMainMenu(menu);
    }
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
auto Application::DispatchEvent(Event<>& event, EventFunction const& dispatch) -> Bool
{
    return dispatch(event);
}

///
/// @brief
///
/// @param event
///
/// @return
///
auto Application::HandlePlatformApplicationEvent(Event<>& event) -> Bool
{
    if (event.Is<PlatformApplicationEvent::Started>())
    {
        auto e = Event<>(Event<ApplicationEvent::Started>());
        return SendEvent(e);
    }
    else if (event.Is<PlatformApplicationEvent::Quitting>())
    {
        auto e = Event<>(Event<ApplicationEvent::Quitting>());
        return SendEvent(e);
    }
    else if (event.Is<PlatformApplicationEvent::QuitRequested>())
    {
        auto parameter = event.As<PlatformApplicationEvent::QuitRequested>();
        auto applicationEventParameter = Event<ApplicationEvent::QuitRequested>();
        applicationEventParameter->SetCancelled(parameter->IsCancelled());
        auto applicationEvent = Event<>(std::move(applicationEventParameter));
        if (SendEvent(applicationEvent))
        {
            if (applicationEvent.Is<ApplicationEvent::QuitRequested>())
            {
                parameter->SetCancelled(applicationEvent.As<ApplicationEvent::QuitRequested>()->IsCancelled());
                event = parameter;
            }
            return true;
        }
    }
    else if (event.Is<PlatformApplicationEvent::ActiveChanged>())
    {
        auto const parameter = event.As<PlatformApplicationEvent::ActiveChanged>();
        auto applicationEventParameter = Event<ApplicationEvent::ActiveChanged>();
        applicationEventParameter->SetActive(parameter->IsActive());
        auto applicationEvent = Event<>(applicationEventParameter);
        SendEvent(applicationEvent);
    }
    else if (event.Is<PlatformApplicationEvent::ForegroundChanged>())
    {
        auto const parameter = event.As<PlatformApplicationEvent::ForegroundChanged>();
        auto applicationEventParameter = Event<ApplicationEvent::ForegroundChanged>();
        applicationEventParameter->SetForeground(parameter->IsForeground());
        auto applicationEvent = Event<>(applicationEventParameter);
        SendEvent(applicationEvent);
    }
    return false;
}
}
