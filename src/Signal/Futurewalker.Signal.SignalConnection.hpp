// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Signal.SignalConnectionType.hpp"
#include "Futurewalker.Signal.Slot.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class SignalConnection
{
public:
    SignalConnection() = default;
    SignalConnection(SignalConnection const&) = default;
    auto operator=(SignalConnection const&) -> SignalConnection& = default;

    SignalConnection(SignalConnection&& other) noexcept
      : _slot {std::move(other._slot)}
    {
    }

    explicit SignalConnection(Weak<SlotImplBase> slot)
      : _slot {std::move(slot)}
    {
    }

    auto operator=(SignalConnection&& other) noexcept -> SignalConnection&
    {
        if (this != &other)
        {
            _slot = std::move(other._slot);
        }
        return *this;
    }

    auto IsConnected() const noexcept -> Bool
    {
        if (auto slot = _slot.Lock())
        {
            return slot->IsConnected();
        }
        return false;
    }

    auto Disconnect() noexcept -> void
    {
        if (auto slot = _slot.Lock())
        {
            return slot->Disconnect();
        }
    }

    auto Swap(SignalConnection& other) noexcept -> void
    {
        std::swap(_slot, other._slot);
    }

private:
    Weak<SlotImplBase> _slot;
};

///
/// @brief RAII wrapper for signal connection.
///
class ScopedSignalConnection final : NonCopyable
{
public:
    ScopedSignalConnection() = default;

    ~ScopedSignalConnection() noexcept
    {
        _connection.Disconnect();
    }

    ScopedSignalConnection(SignalConnection&& other)
      : _connection {std::move(other)}
    {
    }

    auto operator=(SignalConnection&& other) noexcept -> ScopedSignalConnection&
    {
        _connection.Disconnect();
        _connection = std::move(other);
        return *this;
    }

    ScopedSignalConnection(ScopedSignalConnection&& other) noexcept
      : _connection {std::move(other._connection)}
    {
    }

    auto operator=(ScopedSignalConnection&& other) noexcept -> ScopedSignalConnection&
    {
        if (this != &other)
        {
            _connection.Disconnect();
            _connection = std::move(other._connection);
        }
        return *this;
    }

    auto IsConnected() const noexcept -> Bool
    {
        return _connection.IsConnected();
    }

    auto Disconnect() noexcept -> void
    {
        _connection.Disconnect();
    }

    auto Swap(ScopedSignalConnection& other) noexcept -> void
    {
        _connection.Swap(other._connection);
    }

private:
    SignalConnection _connection;
};
}
}
