// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformEventLoopContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformEventLoopWin.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Async.ThreadPool.hpp"

#include <winuser.h>
#include <winbase.h>

namespace FW_DETAIL_NS
{
namespace
{
auto constexpr windowClassName = L"08aa6e42-7ed9-4163-8da3-bf535d93733f";
auto constexpr windowName = L"";

struct MessageWindowCreateParams
{
    PlatformEventLoopWin& eventLoop;
};

auto GetEventLoopFromHwnd(HWND wnd) -> Pointer<PlatformEventLoopWin>
{
    const auto data = ::GetWindowLongPtrW(wnd, GWLP_USERDATA);
    return reinterpret_cast<PlatformEventLoopWin*>(data);
}

auto SetWindowPointer(HWND wnd, Pointer<PlatformEventLoopWin> ptr) -> void
{
    ::SetWindowLongPtrW(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(static_cast<PlatformEventLoopWin*>(ptr)));
}

auto RegisterMessageWindowClass(HINSTANCE instance, WNDPROC windowProcedure) -> ATOM
{
    const auto wndClass = WNDCLASSEXW {
      .cbSize = sizeof(WNDCLASSEXW),
      .lpfnWndProc = windowProcedure,
      .hInstance = instance,
      .lpszClassName = windowClassName,
    };

    if (auto atom = RegisterClassExW(&wndClass))
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
/// @brief Make new context.
///
/// @param instanceHandle Instance handle
/// @param threadPool Thread pool
///
auto PlatformEventLoopContextWin::Make(Shared<PlatformInstanceHandleWin> instanceHandle, Shared<ThreadPool> threadPool) -> Shared<PlatformEventLoopContextWin>
{
    auto key = PassKey<PlatformEventLoopContextWin>();
    auto context = Shared<PlatformEventLoopContextWin>::Make(key, instanceHandle, threadPool);
    context->_self = context;
    return context;
}

///
/// @brief Constructor.
///
PlatformEventLoopContextWin::PlatformEventLoopContextWin(PassKey<PlatformEventLoopContextWin>, Shared<PlatformInstanceHandleWin> instanceHandle, Shared<ThreadPool> threadPool)
  : _instanceHandle {instanceHandle}
  , _threadPool {threadPool}
{
    _classAtom = RegisterMessageWindowClass(_instanceHandle->GetInstanceHandle(), &PlatformEventLoopContextWin::MessageWindowProcedure);
}

///
/// @brief Destructor.
///
PlatformEventLoopContextWin::~PlatformEventLoopContextWin()
{
    UnregisterMessageWindowClass(_classAtom, _instanceHandle->GetInstanceHandle());
}

///
/// @brief Obtain event loop for current thread.
///
auto PlatformEventLoopContextWin::GetEventLoopForThread() -> Shared<PlatformEventLoop>
{
    thread_local auto _eventLoop = Weak<PlatformEventLoopWin>();

    if (auto eventLoop = _eventLoop.Lock())
    {
        return eventLoop;
    }

    auto const threadInfo = PlatformEventLoopWin::ThreadInfo {
      .threadId = std::this_thread::get_id(),
    };
    auto self = _self.Lock();
    auto eventLoop = PlatformEventLoopWin::Make(self, threadInfo, _threadPool);
    _eventLoopMap.Add(threadInfo.threadId, eventLoop);
    _eventLoop = eventLoop;
    return eventLoop;
}

///
/// @brief
///
/// @param id
/// @param loop
///
auto PlatformEventLoopContextWin::EventLoopMap::Add(std::thread::id id, Shared<PlatformEventLoop> loop) -> void
{
    if (loop)
    {
        auto lock = std::unique_lock(_mtx);
        _map[id] = loop;
    }
}

///
/// @brief 
///
/// @param id 
///
auto PlatformEventLoopContextWin::EventLoopMap::Find(std::thread::id id) -> Shared<PlatformEventLoop>
{
    auto lock = std::unique_lock(_mtx);

    auto it = _map.find(id);
    if (it != _map.end())
    {
        return it->second.Lock();
    }
    return nullptr;
}

///
/// @brief
///
/// @param tid
///
auto PlatformEventLoopContextWin::GetEventLoopForThreadId(const std::thread::id& tid) -> Shared<PlatformEventLoop>
{
    return _eventLoopMap.Find(tid);
}

///
/// @brief
///
auto PlatformEventLoopContextWin::CreateMessageWindow(PassKey<PlatformEventLoopWin>, PlatformEventLoopWin& eventLoop) -> HWND
{
    auto createParams = MessageWindowCreateParams {
      .eventLoop = eventLoop,
    };
    auto const hwnd = CreateMessageWindowHandle(_classAtom, _instanceHandle->GetInstanceHandle(), createParams);
    eventLoop.SetNativeHandle({}, hwnd);
    return hwnd;
}

///
/// @brief
///
auto PlatformEventLoopContextWin::DestroyMessageWindow(PassKey<PlatformEventLoopWin>, HWND hwnd) -> void
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
auto CALLBACK PlatformEventLoopContextWin::MessageWindowProcedure(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_NCCREATE)
    {
        const auto createStruct = reinterpret_cast<CREATESTRUCTW const*>(lParam);
        const auto createParams = reinterpret_cast<MessageWindowCreateParams*>(createStruct->lpCreateParams);
        SetWindowPointer(wnd, &createParams->eventLoop);
        return TRUE;
    }

    if (auto _this = GetEventLoopFromHwnd(wnd))
    {
        auto passKey = PassKey<PlatformEventLoopContextWin>();
        return _this->MessageWindowProcedure(passKey, wnd, msg, wParam, lParam);
    }
    return ::DefWindowProcW(wnd, msg, wParam, lParam);
}

///
/// @brief
///
auto Locator::Resolver<PlatformEventLoopContextWin>::Resolve() -> Shared<PlatformEventLoopContextWin>
{
    auto instanceHandle = Locator::Resolve<PlatformInstanceHandleWin>();
    auto threadPool = Locator::ResolveWithDefault<ThreadPool>();
    return PlatformEventLoopContextWin::Make(instanceHandle, threadPool);
}
}
