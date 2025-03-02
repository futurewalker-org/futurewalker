// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Signal.SignalType.hpp"
#include "Futurewalker.Signal.SignalConnection.hpp"
#include "Futurewalker.Signal.SignalCombiner.hpp"
#include "Futurewalker.Signal.Tracker.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

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
/// This class wraps boost::signals2 to provide signal-slot features for trackable objects.
///
template <template <class> class Combiner, class R, class... ArgTypes>
class Signal<R(ArgTypes...), Combiner> final : NonCopyable
{
    using ResultType = R;
    using CombinerType = boost::signals2::keywords::combiner_type<Combiner<ResultType>>;
    using SignatureType = boost::signals2::keywords::signature_type<R(ArgTypes...)>;
    using SignalType = boost::signals2::signal_type<SignatureType, CombinerType>::type;
    using SlotFunctionType = boost::function<R(ArgTypes...)>;
    using SlotType = boost::signals2::slot<R(ArgTypes...), SlotFunctionType>;

public:
    ///
    /// @brief Default constructor.
    ///
    Signal() = default;

    ///
    /// @brief Move constructor.
    ///
    Signal(Signal&& other) noexcept
    {
        _signal.swap(other._signal);
    }

    ///
    /// @brief Move assign operator.
    ///
    Signal& operator=(Signal&& other) noexcept
    {
        auto tmp = Signal(std::move(other));
        Swap(tmp);
        return *this;
    }

    ///
    /// @brief Swap signals.
    ///
    void Swap(Signal& other) noexcept
    {
        _signal.swap(other._signal);
    }

    ///
    /// @brief Disconnect all slots.
    ///
    void DisconnectAll()
    {
        return _signal.disconnect_all_slots();
    }

    ///
    /// @brief Has no slots connected?
    ///
    auto IsEmpty() const -> Bool
    {
        return _signal.empty();
    }

    ///
    /// @brief Get number of slots.
    ///
    auto GetSlotCount() const -> SInt64
    {
        return SInt64(_signal.num_slots());
    }

    ///
    /// @brief Callable operator.
    ///
    template <class... Args>
    auto operator()(Args&&... args)
    {
        return _signal(std::forward<Args>(args)...);
    }

    ///
    /// @brief Connect slot.
    ///
    auto Connect(SlotFunctionType slot, SignalConnectPosition pos) -> SignalConnection
    {
        return SignalConnection(_signal.connect(std::move(slot), InternalGetBoostConnectPosition(pos)));
    }

    ///
    /// @brief Connect slot.
    ///
    auto ConnectScoped(SlotFunctionType slot, SignalConnectPosition pos) -> ScopedSignalConnection
    {
        return ScopedSignalConnection(Connect(std::move(slot), pos));
    }

    ///
    /// @brief Connect with automatic tracking.
    ///
    auto Connect(Weak<Tracker> t, SlotFunctionType slot, SignalConnectPosition pos) -> SignalConnection
    {
        return SignalConnection(_signal.connect(SlotType(std::move(slot)).track_foreign(SignalWeakWrapper(t)), InternalGetBoostConnectPosition(pos)));
    }

    ///
    /// @brief Connect with automatic tracking.
    ///
    auto Connect(Weak<const Tracker> t, SlotFunctionType slot, SignalConnectPosition pos) -> SignalConnection
    {
        return SignalConnection(_signal.connect(SlotType(std::move(slot)).track_foreign(SignalWeakWrapper(t)), InternalGetBoostConnectPosition(pos)));
    }

private:
    auto InternalGetBoostConnectPosition(const SignalConnectPosition pos) const -> boost::signals2::connect_position
    {
        switch (pos)
        {
            case SignalConnectPosition::front:
                return boost::signals2::connect_position::at_front;
            case SignalConnectPosition::back:
                return boost::signals2::connect_position::at_back;
        }
        return boost::signals2::connect_position::at_back;
    }

private:
    SignalType _signal;
};

}
}
