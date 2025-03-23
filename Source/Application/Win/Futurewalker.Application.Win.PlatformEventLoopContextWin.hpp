// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.PlatformEventLoopContextWinType.hpp"
#include "Futurewalker.Application.Win.PlatformEventLoopWinType.hpp"
#include "Futurewalker.Application.PlatformEventLoopContext.hpp"

#include "Futurewalker.Base.Win.PlatformInstanceHandleWin.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.HashMap.hpp"

#include "Futurewalker.Async.ThreadPoolType.hpp"

#include <exception>
#include <thread>
#include <mutex>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Event loop context.
///
class PlatformEventLoopContextWin : public PlatformEventLoopContext
{
public:
    static auto Make(Shared<PlatformInstanceHandleWin> instanceHandle, Shared<ThreadPool> threadPool) -> Shared<PlatformEventLoopContextWin>;

    PlatformEventLoopContextWin(PassKey<PlatformEventLoopContextWin>, Shared<PlatformInstanceHandleWin> instanceHandle, Shared<ThreadPool> threadPool);
    ~PlatformEventLoopContextWin();

    auto GetEventLoopForThread() -> Shared<PlatformEventLoop> override;
    auto GetEventLoopForThreadId(const std::thread::id& tid) -> Shared<PlatformEventLoop>;

    auto CreateMessageWindow(PassKey<PlatformEventLoopWin>, PlatformEventLoopWin& eventLoop) -> HWND;
    auto DestroyMessageWindow(PassKey<PlatformEventLoopWin>, HWND hwnd) -> void;

private:
    static auto CALLBACK MessageWindowProcedure(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

private:   
    Weak<PlatformEventLoopContextWin> _self;
    Shared<PlatformInstanceHandleWin> _instanceHandle;
    Shared<ThreadPool> _threadPool;
    ATOM _classAtom = 0;

private:
    struct EventLoopMap
    {
    public:
        auto Add(std::thread::id id, Shared<PlatformEventLoop> loop) -> void;
        auto Find(std::thread::id id) -> Shared<PlatformEventLoop>;

    private:
        std::mutex _mtx;
        HashMap<std::thread::id, Weak<PlatformEventLoop>> _map;
    } _eventLoopMap;
};

///
/// @brief
///
template <>
struct Locator::Resolver<PlatformEventLoopContextWin>
{
    using Interface = PlatformEventLoopContext; 
    static auto Resolve() -> Shared<PlatformEventLoopContextWin>;
};
}
}
