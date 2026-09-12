// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformApplicationContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationWin.hpp"
#include "Futurewalker.Application.Win.PlatformWindowContextWin.hpp"
#include "Futurewalker.Application.PlatformApplicationEvent.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.Win.PlatformWindowMessageWin.hpp"

namespace FW_DETAIL_NS
{
namespace
{
auto constexpr windowClassName = L"08aa6e42-7ed9-4163-8da3-bf535d93733f";
auto constexpr windowName = L"";

struct MessageWindowCreateParams
{
    Pointer<PlatformApplicationContextWin> context;
};

auto GetContextPointerFromHwnd(HWND wnd) -> Pointer<PlatformApplicationContextWin>
{
    const auto data = ::GetWindowLongPtrW(wnd, GWLP_USERDATA);
    return reinterpret_cast<PlatformApplicationContextWin*>(data);
}

auto SetContextPointerToHwnd(HWND wnd, Pointer<PlatformApplicationContextWin> ptr) -> void
{
    ::SetWindowLongPtrW(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(static_cast<PlatformApplicationContextWin*>(ptr)));
}

auto RegisterMessageWindowClass(HINSTANCE instance, WNDPROC windowProcedure) -> ATOM
{
    const auto wndClass = WNDCLASSEXW {
        .cbSize = sizeof(WNDCLASSEXW),
        .lpfnWndProc = windowProcedure,
        .hInstance = instance,
        .lpszClassName = windowClassName,
    };

    if (auto atom = ::RegisterClassExW(&wndClass))
    {
        return atom;
    }
    throw Exception(ErrorCode::Failure, "Failed to register window class");
}

auto UnregisterMessageWindowClass(ATOM classAtom, HINSTANCE instance) -> void
{
    if (!::UnregisterClassW(MAKEINTATOM(classAtom), instance))
    {
        throw Exception(ErrorCode::Failure, "Failed to unregister window class");
    }
}

auto CreateMessageWindowHandle(ATOM classAtom, HINSTANCE instance, MessageWindowCreateParams& params) -> HWND
{
    return ::CreateWindowExW(0, MAKEINTATOM(classAtom), windowName, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, instance, &params);
}
}

///
/// @brief Make application context.
///
/// @param[in] eventLoopContext Non-null event loop context.
/// @param[in] mainThread Non-null main thread context.
///
auto PlatformApplicationContextWin::Make(
  Shared<PlatformInstanceHandleWin> instanceHandle,
  Shared<PlatformThreadRuntimeContextWin> mainThreadRuntimeContext,
  Shared<PlatformMainThread> mainThread,
  Shared<PlatformScreenContext> screenContext,
  Shared<PlatformWindowContext> windowContext,
  Shared<PlatformApplicationThemeContext> themeContext) -> Shared<PlatformApplicationContextWin>
{
    auto key = PassKey<PlatformApplicationContextWin>();
    auto context = Shared<PlatformApplicationContextWin>::Make(key, instanceHandle, mainThreadRuntimeContext, mainThread, screenContext, windowContext, themeContext);
    context->_self = context;
    context->Initialize();
    return context;
}

///
/// @brief Constructor.
///
/// @param[in] eventLoopContext Non-null event loop context.
/// @param[in] mainThread Non-null main thread context.
///
PlatformApplicationContextWin::PlatformApplicationContextWin(
  PassKey<PlatformApplicationContextWin>,
  Shared<PlatformInstanceHandleWin> instanceHandle,
  Shared<PlatformThreadRuntimeContextWin> mainThreadRuntimeContext,
  Shared<PlatformMainThread> mainThread,
  Shared<PlatformScreenContext> screenContext,
  Shared<PlatformWindowContext> windowContext,
  Shared<PlatformApplicationThemeContext> themeContext)
  : _instanceHandle {instanceHandle}
  , _mainThreadRuntimeContext {mainThreadRuntimeContext}
  , _mainThread {mainThread}
  , _screenContext {screenContext}
  , _windowContext {windowContext}
  , _themeContext {themeContext}
{
}

///
/// @brief
///
PlatformApplicationContextWin::~PlatformApplicationContextWin()
{
    if (ThisThread::GetScheduler() == _mainThreadScheduler)
    {
        ThisThread::SetScheduler(nullptr);
    }
    UnregisterMessageWindowClass(_classAtom, _instanceHandle->GetInstanceHandle());
}

///
/// @brief
///
auto PlatformApplicationContextWin::MakeApplication(PlatformApplication::Delegate delegate) -> Shared<PlatformApplication>
{
    if (_currentApplication.IsExpired())
    {
        auto application = PlatformApplicationWin::Make(delegate, GetSelf());
        _currentApplication = application;
        return application;
    }
    throw Exception(ErrorCode::InvalidOperation, "Attempted to create multiple Application instances");
}

///
/// @brief
///
auto PlatformApplicationContextWin::GetCurrentApplication() -> Shared<PlatformApplication>
{
    return _currentApplication.Lock();
}

///
/// @brief
///
auto PlatformApplicationContextWin::RunApplication(Shared<PlatformApplication> app, Function<void()> cleanup) -> Async<void>
{
    auto appWin = app.TryAs<PlatformApplicationWin>();
    if (!appWin || appWin != _currentApplication.Lock())
    {
        FW_DEBUG_ASSERT(false);
        throw Exception(ErrorCode::InvalidOperation, "Application instance is not valid");
    }

    if (!IsMainThread())
    {
        FW_DEBUG_ASSERT(false);
        throw Exception(ErrorCode::InvalidOperation, "Application must run on main thread");
    }

    if (std::exchange(_running, true))
    {
        FW_DEBUG_LOG_ERROR("PlatformApplicationWin::Run: Event loop must run on the thread it belongs to");
        FW_DEBUG_ASSERT(false);
        co_return;
    }

    if (!CreateMessageWindow())
    {
        FW_DEBUG_LOG_ERROR("PlatformEventLoopWin::Run: Failed to create message window");
        FW_DEBUG_ASSERT(false);
        co_return;
    }

    try
    {
        FW_DEBUG_LOG_INFO("Starting event loop on thread {}", std::this_thread::get_id());

        BeginRunning(appWin);

        while (true)
        {
            auto msg = MSG();
            const auto result = ::GetMessageW(&msg, NULL, 0, 0);

            if (result == -1)
            {
                break;
            }

            // WM_QUIT or other messages.
            if (!TranslateAndDispatchMessage(appWin, msg, cleanup))
            {
                break;
            }
        }

        FW_DEBUG_LOG_INFO("Exiting event loop on thread {}", std::this_thread::get_id());
    }
    catch (...)
    {
        FW_DEBUG_LOG_ERROR("PlatformEventLoopWin::Run: Unhandled exception in event loop");
        FW_DEBUG_ASSERT(false);
    }

    DestroyMessageWindow();
    _running = false;
}

///
/// @brief
///
auto PlatformApplicationContextWin::PostQuitMessage() -> void
{
    if (IsMainThread() && IsRunning())
    {
        FW_DEBUG_LOG_INFO("PlatformEventLoopWin::RequestQuit: Called on thread {}", std::this_thread::get_id());
        ::PostQuitMessage(0);
    }
}

///
/// @brief Schedule task to event loop.
///
auto PlatformApplicationContextWin::Schedule() -> AsyncTask<void>
{
    struct Awaitable
    {
        Weak<PlatformApplicationContextWin> weakSelf;

        auto await_ready() const noexcept -> bool
        {
            return false;
        }

        auto await_suspend(std::coroutine_handle<> c) -> void
        {
            if (auto self = weakSelf.Lock())
            {
                return self->Post([](std::coroutine_handle<> c) -> LazyTask<void> { co_return c.resume(); }(c));
            }
            throw Exception(ErrorCode::Failure);
        }

        auto await_resume() -> void
        {
        }
    };
    co_await Awaitable {GetSelf()};
}

///
/// @brief Schedule task to event loop after delay.
///
/// @param delay Delay
///
auto PlatformApplicationContextWin::ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>
{
    auto self = GetSelf();

    co_await _threadPool->ScheduleAfter(delay);

    if (auto app = GetSelf())
    {
        co_return co_await app->Schedule();
    }
    throw Exception(ErrorCode::Failure);
}

auto PlatformApplicationContextWin::IsMainThread() const -> Bool
{
    return std::this_thread::get_id() == _mainThreadId;
}

auto PlatformApplicationContextWin::IsRunning() const -> Bool
{
    return _running;
}

auto PlatformApplicationContextWin::Initialize() -> void
{
    if (!_mainThreadRuntimeContext || !_mainThread || !_instanceHandle || !_screenContext || !_windowContext)
    {
        throw Exception(ErrorCode::InvalidArgument);
    }

    _threadPool = Locator::ResolveWithDefault<ThreadPool>();

    _classAtom = RegisterMessageWindowClass(_instanceHandle->GetInstanceHandle(), &PlatformApplicationContextWin::MessageWindowProcedure);

    _screenContext->Refresh();

    class ThisThreadScheduler final : public ThisThread::Scheduler
    {
        Weak<PlatformApplicationContextWin> _app;

    public:
        ThisThreadScheduler(Weak<PlatformApplicationContextWin> app)
          : _app {app}
        {
        }

        auto Schedule() -> AsyncTask<void> override
        {
            if (auto const app = _app.Lock())
            {
                co_return co_await app->Schedule();
            }
            throw Exception(ErrorCode::InvalidOperation);
        }

        auto ScheduleAfter(std::chrono::nanoseconds const delay) -> AsyncTask<void> override
        {
            if (auto const app = _app.Lock())
            {
                co_return co_await app->ScheduleAfter(delay);
            }
            throw Exception(ErrorCode::InvalidOperation);
        }
    };
    _mainThreadId = std::this_thread::get_id();
    _mainThreadScheduler = Shared<ThisThreadScheduler>::Make(GetSelf());
    ThisThread::SetScheduler(_mainThreadScheduler);
}

auto PlatformApplicationContextWin::GetSelf() -> Shared<PlatformApplicationContextWin>
{
    return _self.Lock();
}

auto PlatformApplicationContextWin::GetSelf() const -> Shared<PlatformApplicationContextWin const>
{
    return _self.Lock();
}

///
/// @brief Create message window for the application.
///
auto PlatformApplicationContextWin::CreateMessageWindow() -> Bool
{
    if (!_messageWindow)
    {
        auto createParams = MessageWindowCreateParams {
            .context = this,
        };
        _messageWindow = CreateMessageWindowHandle(_classAtom, _instanceHandle->GetInstanceHandle(), createParams);
    }
    return _messageWindow != nullptr;
}

///
/// @brief Destroy message window for the application.
///
auto PlatformApplicationContextWin::DestroyMessageWindow() -> void
{
    if (_messageWindow)
    {
        ::DestroyWindow(_messageWindow);
        _messageWindow = nullptr;
    }
}

///
/// @brief Translate and dispatch window message.
///
Bool PlatformApplicationContextWin::TranslateAndDispatchMessage(Shared<PlatformApplicationWin> const& app, MSG const& msg, Function<void()> const& cleanup)
{
    if (msg.message == WM_QUIT)
    {
        EndRunning(app);

        if (cleanup)
        {
            cleanup();
        }

        ::PostQuitMessage(static_cast<int>(msg.wParam));

        if (HasTask())
        {
            return true;
        }
        return false;
    }

    ::TranslateMessage(&msg);
    ::DispatchMessageW(&msg);

    return true;
}

///
/// @brief
///
auto PlatformApplicationContextWin::HandlePostedEvent(Bool& callDefaultProcedure, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    (void)wParam;
    (void)lParam;
    if (auto task = PopTask())
    {
        AsyncFunction::Spawn(std::move(*task)).Detach();
    }
    callDefaultProcedure = false;
    return 0;
}

auto PlatformApplicationContextWin::BeginRunning(Shared<PlatformApplicationWin> const& app) -> void
{
    if (auto windowContext = _windowContext.TryAs<PlatformWindowContextWin>())
    {
        windowContext->SetRunning(true);
    }

    if (app)
    {
        app->SetStarted();
    }
}

auto PlatformApplicationContextWin::EndRunning(Shared<PlatformApplicationWin> const& app) -> void
{
    // Notify application.
    if (app)
    {
        app->SetQuitting();
    }

    // Pre-quit cleanup for contexts.
    if (auto windowContext = _windowContext.TryAs<PlatformWindowContextWin>())
    {
        windowContext->SetRunning(false);
    }
}

///
/// @brief
///
auto PlatformApplicationContextWin::Post(LazyTask<void> task) -> void
{
    std::unique_lock lock(_mutex);

    if (_messageWindow)
    {
        _tasks.push_front(std::move(task));

        if (!::PostMessageW(_messageWindow, WM_FW_EVENT_QUEUE_POST, 0, 0))
        {
            FW_DEBUG_LOG_ERROR("PostMessage for WM_FW_EVENT_QUEUE_POST failed");
            FW_DEBUG_ASSERT(false);
        }
    }
    else
    {
        throw Exception(ErrorCode::Failure, "Thread was already exited");
    }
}

///
/// @brief
///
auto PlatformApplicationContextWin::PopTask() -> Optional<LazyTask<void>>
{
    std::unique_lock lock(_mutex);
    if (!_tasks.empty())
    {
        auto task = std::move(_tasks.back());
        _tasks.pop_back();
        return task;
    }
    return {};
}

///
/// @brief
///
auto PlatformApplicationContextWin::HasTask() -> Bool
{
    std::unique_lock lock(_mutex);
    return !_tasks.empty();
}

///
/// @brief Window procedure of message window.
///
/// @param wnd
/// @param msg
/// @param wParam
/// @param lParam
///
auto CALLBACK PlatformApplicationContextWin::MessageWindowProcedure(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_NCCREATE)
    {
        const auto createStruct = reinterpret_cast<CREATESTRUCTW const*>(lParam);
        const auto createParams = reinterpret_cast<MessageWindowCreateParams*>(createStruct->lpCreateParams);
        SetContextPointerToHwnd(wnd, createParams->context);
        return TRUE;
    }

    if (auto _this = GetContextPointerFromHwnd(wnd))
    {
        auto callDefaultProcedure = True;
        auto result = LRESULT();
        try
        {
            if (msg == WM_FW_EVENT_QUEUE_POST)
            {
                result = _this->HandlePostedEvent(callDefaultProcedure, wParam, lParam);
            }
        }
        catch (...)
        {
            FW_DEBUG_LOG_ERROR("Unhandled exception in window procedure");
            FW_DEBUG_ASSERT(false);
        }

        if (callDefaultProcedure)
        {
            return ::DefWindowProcW(wnd, msg, wParam, lParam);
        }
        return result;
    }
    return ::DefWindowProcW(wnd, msg, wParam, lParam);
}

///
/// @brief
///
auto Locator::Resolver<PlatformApplicationContextWin>::Resolve() -> Shared<PlatformApplicationContextWin>
{
    auto mainThreadRuntimeContext = Shared<PlatformThreadRuntimeContextWin>::Make(PlatformThreadRuntimeInitTypeWin::SingleThreaded);
    auto mainThread = Locator::Resolve<PlatformMainThread>();
    auto instanceHandle = Locator::Resolve<PlatformInstanceHandleWin>();
    auto screenContext = Locator::Resolve<PlatformScreenContext>();
    auto windowContext = Locator::Resolve<PlatformWindowContext>();
    auto themeContext = Locator::Resolve<PlatformApplicationThemeContext>();
    return PlatformApplicationContextWin::Make(instanceHandle, mainThreadRuntimeContext, mainThread, screenContext, windowContext, themeContext);
}
}
