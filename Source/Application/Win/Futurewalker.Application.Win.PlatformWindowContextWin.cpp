// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformWindowContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformWindowWin.hpp"
#include "Futurewalker.Application.Win.PlatformVsyncProviderWin.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWin.hpp"
#include "Futurewalker.Application.Win.PlatformKeyboardLayoutWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodContextWin.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"
#include "Futurewalker.Base.Win.PlatformInstanceHandleWin.hpp"

#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.StringFunction.hpp"
#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

namespace FW_DETAIL_NS
{
namespace
{
struct PlatformToplevelWindowCreateParams
{
    PlatformWindowWin& window;
};

struct PlatformLevelWindowCreateParams
{
};

///
/// @brief
///
/// @param wnd
///
auto GetWindowPointer(HWND wnd) -> PlatformWindowWin*
{
    const auto data = ::GetWindowLongPtrW(wnd, GWLP_USERDATA);
    return reinterpret_cast<PlatformWindowWin*>(data);
}

///
/// @brief
///
/// @param wnd
/// @param ptr
///
void SetWindowPointer(HWND wnd, PlatformWindowWin* ptr)
{
    ::SetWindowLongPtrW(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptr));
}

///
/// @brief
///
/// @param className
///
auto MakeWindowClassName(StringView className) -> std::wstring
{
    const auto postfix = reinterpret_cast<uintptr_t>(&MakeWindowClassName);
    const auto name = StringFunction::Format(u8"{}_{:x}", className, postfix);
    return PlatformStringFunctionWin::Utf8ToWide(name);
}

///
/// @brief
///
/// @param options
///
auto GetWindowStyle(PlatformWindowOptions const& options) -> DWORD
{
    auto style = DWORD(0);

    if (options.behavior == WindowBehavior::Normal)
    {
        style |= WS_OVERLAPPED;
    }
    else if (options.behavior == WindowBehavior::Dialog)
    {
        style |= WS_OVERLAPPED;
    }
    else if (options.behavior == WindowBehavior::Popup)
    {
        style |= WS_POPUP;
    }
    else if (options.behavior == WindowBehavior::Tooltip)
    {
        style |= WS_POPUP;
    }

    if (options.hasTitleBar)
    {
        style |= WS_CAPTION;
    }

    style |= WS_SYSMENU;

    if (options.minimizable)
    {
        style |= WS_MINIMIZEBOX;
    }

    if (options.resizable)
    {
        style |= WS_SIZEBOX;
        style |= WS_MAXIMIZEBOX;
    }

    return style;
}

///
/// @brief
///
/// @param options
///
/// @return
///
auto GetWindowExStyle(PlatformWindowOptions const& options) -> DWORD
{
    if (options.backgroundStyle == WindowBackgroundStyle::System || options.backgroundStyle == WindowBackgroundStyle::Transparent)
    {
        // Remove redirection bitmap for DGI rendering.
        // hbrBackground in WNDCLASSEX is ignored when WS_EX_NOREDIRECTIONBITMAP is set.
        return WS_EX_NOREDIRECTIONBITMAP;
    }
    return 0;
}

///
/// @brief
///
auto RegisterWindowClass(LPCWSTR className, WNDPROC windowProc, HINSTANCE instance) -> ATOM
{
    const auto wndClass = WNDCLASSEXW {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = windowProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = instance,
        .hIcon = ::LoadIconW(NULL, IDI_APPLICATION),
        .hCursor = ::LoadCursorW(NULL, IDC_ARROW),
        .hbrBackground = ::GetSysColorBrush(COLOR_WINDOW),
        .lpszMenuName = NULL,
        .lpszClassName = className,
        .hIconSm = NULL,
    };

    if (auto atom = ::RegisterClassExW(&wndClass))
    {
        return atom;
    }
    throw Exception(ErrorCode::Failure, "Failed to register window class");
}

///
/// @brief
///
auto UnregisterWindowClass(ATOM windowClass, HINSTANCE instance) -> void
{
    ::UnregisterClassW(MAKEINTATOM(windowClass), instance);
}

///
/// @brief
///
template <class CreateParams>
auto CreateWindowHandle(ATOM classAtom, HWND parent, DWORD style, DWORD exStyle, LPCWSTR title, HINSTANCE instance, CreateParams& params) -> HWND
{
    return ::CreateWindowExW(exStyle, MAKEINTATOM(classAtom), title, style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, NULL, instance, &params);
}

///
/// @brief Get topmost owned window.
///
auto GetTopmostOwnedWindow(HWND const hWnd) -> HWND
{
    struct Params
    {
        HWND parent = NULL;
        HWND result = NULL;
    } params {.parent = hWnd};

    ::EnumWindows(
      [](HWND hWnd, LPARAM lParam) -> BOOL {
          auto params = reinterpret_cast<Params*>(lParam);
          const auto owner = ::GetWindow(hWnd, GW_OWNER);
          if (owner && owner == params->parent)
          {
              params->result = hWnd;
              return FALSE;
          }
          return TRUE;
      },
      reinterpret_cast<LPARAM>(&params));

    return params.result;
}
}

///
/// @brief
///
/// @return
///
auto PlatformWindowContextWin::Make(
  Shared<PlatformInstanceHandleWin> instanceHandle,
  Shared<PlatformInputMethodContextWin> inputMethodContext,
  Shared<PlatformDCompositionDeviceWin> compositionDevice,
  Shared<PlatformVsyncProviderWin> vsyncProvider,
  Shared<PlatformScreenContext> screenContext) -> Shared<PlatformWindowContextWin>
{
    auto key = PassKey<PlatformWindowContextWin>();
    auto context = Shared<PlatformWindowContextWin>::Make(key, instanceHandle, inputMethodContext, compositionDevice, vsyncProvider, screenContext);
    context->_self = context;
    return context;
}

///
/// @brief
///
/// @param
///
PlatformWindowContextWin::PlatformWindowContextWin(
  PassKey<PlatformWindowContextWin>,
  Shared<PlatformInstanceHandleWin> instanceHandle,
  Shared<PlatformInputMethodContextWin> inputMethodContext,
  Shared<PlatformDCompositionDeviceWin> compositionDevice,
  Shared<PlatformVsyncProviderWin> vsyncProvider,
  Shared<PlatformScreenContext> screenContext)
  : PlatformWindowContext()
  , _instanceHandle {instanceHandle}
  , _inputMethodContext {inputMethodContext}
  , _compositionDevice {compositionDevice}
  , _vsyncProvider {vsyncProvider}
  , _screenContext {screenContext}
{
    const auto instance = _instanceHandle->GetInstanceHandle();

    const auto toplevelWindowClassName = MakeWindowClassName(u8"Futurewalker.Toplevel_Window_Class");
    _toplevelWindowClass = RegisterWindowClass(toplevelWindowClassName.c_str(), &PlatformWindowContextWin::ToplevelWindowProcedure, instance);

    const auto levelWindowClassName = MakeWindowClassName(u8"Futurewalker.Level_Window_Class");
    _levelWindowClass = RegisterWindowClass(levelWindowClassName.c_str(), &PlatformWindowContextWin::LevelWindowProcedure, instance);

    ::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    _keyboardLayout = Shared<PlatformKeyboardLayoutWin>::Make();
}

///
/// @brief
///
PlatformWindowContextWin::~PlatformWindowContextWin()
{
    const auto instance = _instanceHandle->GetInstanceHandle();
    UnregisterWindowClass(_toplevelWindowClass, instance);
    UnregisterWindowClass(_levelWindowClass, instance);
}

///
/// @brief
///
/// @param options
/// @param delegate
///
//
auto PlatformWindowContextWin::MakePlatformWindow(PlatformWindowOptions const& options, PlatformWindow::Delegate const& delegate) -> Shared<PlatformWindow>
{
    return PlatformWindowWin::Make(_self.Lock(), _compositionDevice, options, delegate);
}

///
/// @brief
///
/// @param window
/// @param options
///
auto PlatformWindowContextWin::InitializeWindow(Shared<PlatformWindowWin> const& window, PlatformWindowOptions const& options) -> void
{
    const auto instance = _instanceHandle->GetInstanceHandle();
    const auto owner = options.owner ? options.owner.As<PlatformWindowWin>()->GetNativeHandle() : NULL;
    const auto style = GetWindowStyle(options);
    const auto exStyle = GetWindowExStyle(options);
    const auto title = PlatformStringFunctionWin::Utf8ToWide(options.title);

    auto createParams = PlatformToplevelWindowCreateParams {
        .window = *window,
    };

    if (owner && !::IsWindowEnabled(owner))
    {
        throw Exception(ErrorCode::InvalidArgument, "Tried to set disabled window as owner");
    }

    const auto hwnd = CreateWindowHandle(_toplevelWindowClass, owner, style, exStyle, title.c_str(), instance, createParams);

    FW_DEBUG_LOG_INFO("CreateWindowHandle(): hwnd=0x{:x}", (uintptr_t)hwnd);

    if (const auto ownerWindow = options.owner.Maybe<PlatformWindowWin>())
    {
        ownerWindow->AddOwnedWindow(window);
    }

    if (!::IsWindow(hwnd))
    {
        throw Exception(ErrorCode::Failure, "Failed to create window");
    }

    if (options.maximized)
    {
        ::ShowWindow(hwnd, SW_MAXIMIZE);
    }

    if (options.minimized)
    {
        ::ShowWindow(hwnd, SW_MINIMIZE);
    }

    ::ShowWindow(hwnd, options.visible ? SW_SHOW : SW_HIDE);
}

///
/// @brief
///
auto PlatformWindowContextWin::RequestFrame(Weak<PlatformWindowWin> window, PlatformVsyncCallbackFunction callback) -> void
{
    if (_vsyncProvider)
    {
        _vsyncProvider->PostFrameCallback(window, callback);
    }
}

///
/// @brief
///
auto PlatformWindowContextWin::GetFrameTime(PlatformWindowWin const& window) const -> MonotonicTime
{
    (void)window;

    if (_vsyncProvider)
    {
        return _vsyncProvider->GetCurrentFrameTime();
    }
    FW_DEBUG_ASSERT(false);
    return {};
}

///
/// @brief
///
auto PlatformWindowContextWin::RefreshScreens() -> void
{
    if (_screenContext)
    {
        _screenContext->Refresh();
    }
}

///
/// @brief
///
auto PlatformWindowContextWin::GetKeyboardLayout() -> PlatformKeyboardLayoutWin&
{
    return *_keyboardLayout;
}

///
/// @brief
///
auto PlatformWindowContextWin::GetInputMethodContext() -> PlatformInputMethodContextWin&
{
    return *_inputMethodContext;
}

///
/// @brief Window procedure of toplevel windows.
///
auto CALLBACK PlatformWindowContextWin::ToplevelWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    const auto key = PassKey<PlatformWindowContextWin>();

    // We use CreateWindowEx() thus WM_NCCREATE will be sent before WM_CREATE.
    // NOTE: WM_GETMINMAXINFO can be sent event before WM_NCCREATE.
    if (msg == WM_NCCREATE)
    {
        const auto createStruct = reinterpret_cast<CREATESTRUCTW const*>(lParam);
        const auto createParams = reinterpret_cast<PlatformToplevelWindowCreateParams*>(createStruct->lpCreateParams);
        createParams->window.SetNativeHandle(key, hWnd);
        SetWindowPointer(hWnd, &createParams->window);
        return createParams->window.WindowProcedure(key, hWnd, msg, wParam, lParam);
    }

    if (const auto window = GetWindowPointer(hWnd))
    {
        return window->WindowProcedure(key, hWnd, msg, wParam, lParam);
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Window procedure of level windows.
///
auto CALLBACK PlatformWindowContextWin::LevelWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_NCHITTEST)
    {
        return HTNOWHERE;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief
///
auto Locator::Resolver<PlatformWindowContextWin>::Resolve() -> Shared<PlatformWindowContextWin>
{
    auto instanceHandle = Locator::Resolve<PlatformInstanceHandleWin>();
    auto inputMethodContext = Locator::ResolveWithDefault<PlatformInputMethodContext, PlatformInputMethodContextWin>().As<PlatformInputMethodContextWin>();
    auto compositionDevice = Locator::ResolveWithDefault<PlatformDCompositionDeviceWin>();
    auto vsyncProvider = Locator::ResolveWithDefault<PlatformVsyncProviderWin>();
    auto screenContext = Locator::Resolve<PlatformScreenContext>();
    return PlatformWindowContextWin::Make(instanceHandle, inputMethodContext, compositionDevice, vsyncProvider, screenContext);
}
}
