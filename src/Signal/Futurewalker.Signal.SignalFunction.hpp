// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Signal.SignalConnection.hpp"
#include "Futurewalker.Signal.Signal.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_CONCEPTS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Concept of signal connectable member functions.
///
template <class T, class F, class R, class... Ts>
concept SignalConnectableMember = requires (T& t, F f, Ts&&... args)
{
  { t.GetTracker() };
  { std::invoke(f, t, std::forward<Ts>(args)...) } -> SameAs<R>;
};

///
/// @brief Concept of signal connectable free functions.
///
template <class T, class F, class R, class... Ts>
concept SignalConnectableFree = requires (T& t, F f, Ts&&...args)
{
  { t.GetTracker() };
  { std::invoke(f, std::forward<Ts>(args)...) } -> SameAs<R>;
};
}
}

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Signal utility functions.
///
class SignalFunction final : NonConstructible
{
public:
    ///
    /// @brief Connect signal and slot function.
    ///
    /// @param sig Signal to connect.
    /// @param f Slot function.
    ///
    template <class F, template <class> class Combiner, class R, class... ArgTypes>
    static auto Connect(Signal<R(ArgTypes...), Combiner>& sig, F&& f) -> SignalConnection
    {
        return sig.Connect(std::forward<F>(f));
    }

    ///
    /// @brief Connect signal and slot function.
    ///
    /// @param sig Signal to connect.
    /// @param f Slot function.
    /// @param pos Position of new connection.
    ///
    template <class F, template <class> class Combiner, class R, class... ArgTypes>
    static auto ConnectScoped(Signal<R(ArgTypes...), Combiner>& sig, F&& f) -> ScopedSignalConnection
    {
        return sig.ConnectScoped(std::forward<F>(f));
    }

    ///
    /// @brief Connect signal and slot function.
    ///
    /// @param sig Signal to connect.
    /// @param t Trackable object to bind connection.
    /// @param f Slot function.
    /// @param pos Position of new connection.
    ///
    template <class T, class F, template <class> class Combiner, class R, class... ArgTypes>
        requires Concepts::SignalConnectableMember<T, F, R, ArgTypes...>
    static auto Connect(Signal<R(ArgTypes...), Combiner>& sig, T& t, F&& f) -> SignalConnection
    {
        return sig.Connect(t.GetTracker(), t, std::forward<F>(f));
    }

    ///
    /// @brief Connect signal and slot function.
    ///
    /// @param sig Signal to connect.
    /// @param t Trackable object to bind connection.
    /// @param f Slot function.
    /// @param pos Position of new connection.
    ///
    template <class T, class F, template <class> class Combiner, class R, class... ArgTypes>
        requires Concepts::SignalConnectableFree<T, F, R, ArgTypes...>
    static auto Connect(Signal<R(ArgTypes...), Combiner>& sig, T& t, F&& f) -> SignalConnection
    {
        return sig.Connect(t.GetTracker(), std::forward<F>(f));
    }
};
}
}
