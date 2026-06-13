// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Signal.SignalType.hpp"
#include "Futurewalker.Signal.Slot.hpp"
#include "Futurewalker.Signal.SignalConnection.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.SharedArray.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Signal class.
///
/// @tparam Combiner A signal combiner type.
/// @tparam R Result type of signal.
/// @tparam ArgTypes Argument types of signal.
///
template <template <class> class Combiner, class R, class... ArgTypes>
class Signal<R(ArgTypes...), Combiner> final : NonCopyable
{
public:
    ///
    /// @brief Default constructor.
    ///
    Signal() = default;

    ///
    /// @brief Move constructor.
    ///
    Signal(Signal&& other) noexcept
      : _slots(std::move(other._slots))
    {
    }

    ///
    /// @brief Move assign operator.
    ///
    Signal& operator=(Signal&& other) noexcept
    {
        if (&other != this)
        {
            _slots = std::move(other._slots);
        }
        return *this;
    }

    ///
    /// @brief Swap signals.
    ///
    void Swap(Signal& other) noexcept
    {
        std::swap(_slots, other._slots);
    }

    ///
    /// @brief Disconnect all slots.
    ///
    void DisconnectAll()
    {
        _slots.Clear();
    }

    ///
    /// @brief Has no slots connected?
    ///
    auto IsEmpty() const -> Bool
    {
        for (auto const& slot : _slots.GetValues())
        {
            if (slot.IsConnected())
            {
                return false;
            }
        }
        return true;
    }

    ///
    /// @brief Get number of slots.
    ///
    auto GetSlotCount() const -> SInt64
    {
        auto count = SInt64(0);
        for (auto const& slot : _slots.GetValues())
        {
            if (slot.IsConnected())
            {
                ++count;
            }
        }
        return count;
    }

    ///
    /// @brief Callable operator.
    ///
    auto operator()(ArgTypes const&... args) -> typename Combiner<R>::ResultType
    {
        struct Collector
        {
            Signal& _self;

            Collector(Signal& self) noexcept
              : _self {self}
            {
                ++_self._emissionDepth;
            }

            ~Collector() noexcept
            {
                if (--_self._emissionDepth == 0U)
                {
                    _self._slots.EraseIf([](auto const& slot) { return !slot.IsConnected(); });
                }
            }
        };
        auto collector = Collector(*this);
        auto slots = _slots;
        return Combiner<R>()(slots.GetValues(), args...);
    }

    ///
    /// @brief Connect slot.
    ///
    template <class F>
    auto Connect(F&& f) -> SignalConnection
    {
        return SignalConnection(_slots.PushBack(Slot<R(ArgTypes...)>::Make(std::forward<F>(f))).GetImplBase());
    }

    ///
    /// @brief Connect slot.
    ///
    template <class F>
    auto ConnectScoped(F&& f) -> ScopedSignalConnection
    {
        return SignalConnection(_slots.PushBack(Slot<R(ArgTypes...)>::Make(std::forward<F>(f))).GetImplBase());
    }

    ///
    /// @brief Connect with automatic tracking.
    ///
    template <class F>
    auto Connect(Weak<void const> w, F&& f) -> SignalConnection
    {
        return SignalConnection(_slots.PushBack(Slot<R(ArgTypes...)>::Make(std::move(w), std::forward<F>(f))).GetImplBase());
    }

    ///
    /// @brief Connect with automatic tracking.
    ///
    template <class F>
    auto ConnectScoped(Weak<void const> w, F&& f) -> ScopedSignalConnection
    {
        return SignalConnection(_slots.PushBack(Slot<R(ArgTypes...)>::Make(std::move(w), std::forward<F>(f))).GetImplBase());
    }

    ///
    /// @brief Connect with automatic tracking.
    ///
    template <class T, class F>
    auto Connect(Weak<void const> w, T& t, F&& f) -> SignalConnection
    {
        return SignalConnection(_slots.PushBack(Slot<R(ArgTypes...)>::Make(std::move(w), t, std::forward<F>(f))).GetImplBase());
    }

    ///
    /// @brief Connect with automatic tracking.
    ///
    template <class T, class F>
    auto ConnectScoped(Weak<void const> w, T& t, F&& f) -> ScopedSignalConnection
    {
        return SignalConnection(_slots.PushBack(Slot<R(ArgTypes...)>::Make(std::move(w), t, std::forward<F>(f))).GetImplBase());
    }

private:
    SharedArray<Slot<R(ArgTypes...)>> _slots;
    UInt64 _emissionDepth = 0U;
};
}
}
