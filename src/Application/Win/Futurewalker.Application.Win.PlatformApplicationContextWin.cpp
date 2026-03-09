// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformApplicationContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationWin.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.Exception.hpp"

namespace FW_DETAIL_NS
{
namespace
{
auto constexpr windowClassName = L"08aa6e42-7ed9-4163-8da3-bf535d93733f";
auto constexpr windowName = L"";

struct MessageWindowCreateParams
{
    PlatformApplicationWin& application;
};

auto GetEventLoopFromHwnd(HWND wnd) -> Pointer<PlatformApplicationWin>
{
    const auto data = ::GetWindowLongPtrW(wnd, GWLP_USERDATA);
    return reinterpret_cast<PlatformApplicationWin*>(data);
}

auto SetWindowPointer(HWND wnd, Pointer<PlatformApplicationWin> ptr) -> void
{
    ::SetWindowLongPtrW(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(static_cast<PlatformApplicationWin*>(ptr)));
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
    if (!_mainThreadRuntimeContext || !_mainThread || !_instanceHandle || !_screenContext || !_windowContext)
    {
        throw Exception(ErrorCode::InvalidArgument);
    }

    _classAtom = RegisterMessageWindowClass(_instanceHandle->GetInstanceHandle(), &PlatformApplicationContextWin::MessageWindowProcedure);

    _screenContext->Refresh();
}

///
/// @brief
///
PlatformApplicationContextWin::~PlatformApplicationContextWin()
{
    UnregisterMessageWindowClass(_classAtom, _instanceHandle->GetInstanceHandle());
}

///
/// @brief
///
auto PlatformApplicationContextWin::MakePlatformApplication(PlatformApplication::Delegate delegate) -> Shared<PlatformApplication>
{
    if (_currentApplication.IsExpired())
    {
        auto threadPool = Locator::ResolveWithDefault<ThreadPool>();
        auto application = PlatformApplicationWin::Make(delegate, _self.Lock(), threadPool);
        _currentApplication = application;
        return application;
    }
    throw Exception(ErrorCode::InvalidOperation, "Attempted to create multiple Application instances");
}

///
/// @brief
///
auto PlatformApplicationContextWin::GetCurrentApplication() -> Shared<PlatformApplicationWin>
{
    return _currentApplication.Lock();
}

///
/// @brief Create message window for the application.
///
auto PlatformApplicationContextWin::CreateMessageWindow(PassKey<PlatformApplicationWin>, PlatformApplicationWin& application) -> HWND
{
    auto createParams = MessageWindowCreateParams {
        .application = application,
    };
    auto const hwnd = CreateMessageWindowHandle(_classAtom, _instanceHandle->GetInstanceHandle(), createParams);
    application.SetNativeHandle({}, hwnd);
    return hwnd;
}

///
/// @brief Destroy message window for the application.
///
auto PlatformApplicationContextWin::DestroyMessageWindow(PassKey<PlatformApplicationWin>, HWND hwnd) -> void
{
    if (hwnd)
    {
        ::DestroyWindow(hwnd);
    }
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
        SetWindowPointer(wnd, &createParams->application);
        return TRUE;
    }

    if (auto _this = GetEventLoopFromHwnd(wnd))
    {
        auto passKey = PassKey<PlatformApplicationContextWin>();
        return _this->MessageWindowProcedure(passKey, wnd, msg, wParam, lParam);
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
