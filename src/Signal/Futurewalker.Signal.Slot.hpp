// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Signal.SlotType.hpp"
#include "Futurewalker.Signal.SignalType.hpp"
#include "Futurewalker.Signal.SignalConnectionType.hpp"

#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.Blank.hpp"

#include <functional>

namespace FW_DETAIL_NS
{
struct SlotImplBase : NonCopyable
{
    virtual ~SlotImplBase() = default;
    virtual auto IsConnected() const noexcept -> Bool = 0;
    virtual auto Disconnect() noexcept -> void = 0;
};

template <class R, class... Args>
class SlotImpl<R(Args...)> final : public SlotImplBase
{
public:
    explicit SlotImpl(std::function<R(Args...)> func)
      : _func {std::move(func)}
    {
    }

    explicit SlotImpl(Weak<void const> tracker, std::function<R(Args...)> func)
      : _tracked {true}
      , _tracker {std::move(tracker)}
      , _func {std::move(func)}
    {
    }

    auto Invoke(Args const&... args) const -> Optional<R>
    requires (!Concepts::SameAs<R, void>)
    {
        if (_func && IsConnected())
        {
            return _func(args...);
        }
        return {};
    }

    auto Invoke(Args const&... args) const -> Bool
    requires Concepts::SameAs<R, void>
    {
        if (_func && IsConnected())
        {
            _func(args...);
            return true;
        }
        return false;
    }

    auto IsConnected() const noexcept -> Bool override
    {
        if (_disconnected)
        {
            return false;
        }
        if (_tracked && _tracker.IsExpired())
        {
            return false;
        }
        return true;
    }

    auto Disconnect() noexcept -> void override
    {
        _disconnected = true;
    }

private:
    Bool _tracked = false;
    Bool _disconnected = false;
    Weak<void const> _tracker;
    std::function<R(Args...)> _func;
};

namespace FW_EXPORT
{
template <class R, class... Args>
class Slot<R(Args...)>
{
public:
    template <class F>
    static auto Make(F&& f) -> Slot
    {
        return Slot(Shared<SlotImpl<R(Args...)>>::Make(std::forward<F>(f)));
    }

    template <class F>
    static auto Make(Weak<void const> w, F&& f) -> Slot
    {
        return Slot(Shared<SlotImpl<R(Args...)>>::Make(std::move(w), std::forward<F>(f)));
    }

    template <class T, class F>
    static auto Make(Weak<void const> w, T& t, F&& f) -> Slot
    {
        return Slot(Shared<SlotImpl<R(Args...)>>::Make(std::move(w), [&t, f = std::forward<F>(f)](Args... args) { return std::invoke(f, t, std::forward<Args>(args)...); }));
    }

    auto operator()(Args const&... args) const
    {
        return _impl->Invoke(args...);
    }

    auto IsConnected() const noexcept
    {
        return _impl->IsConnected();
    }

    auto GetImplBase() const noexcept -> Weak<SlotImplBase>
    {
        return _impl;
    }

private:
    explicit Slot(Shared<SlotImpl<R(Args...)>> impl)
      : _impl(std::move(impl))
    {
    }

    Shared<SlotImpl<R(Args...)>> _impl;
};
}
}
