// SPDX-License-Identifier: MIT

#include "Futurewalker.Signal.SignalConnection.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Is connection active?
///
auto SignalConnection::IsConnected() const -> Bool
{
    return _connection.connected();
}

///
/// @brief Disconnection this connection.
///
auto SignalConnection::Disconnect() -> void
{
    _connection.disconnect();
}

///
/// @brief Swap connection.
///
auto SignalConnection::Swap(SignalConnection& other) noexcept -> void
{
    _connection.swap(other._connection);
}

///
/// @brief Construct from boost::signals2::connection.
///
SignalConnection::SignalConnection(boost::signals2::connection c)
  : _connection {c}
{
}

///
/// @brief Destructor.
///
ScopedSignalConnection::~ScopedSignalConnection() noexcept
{
    if (_connection.IsConnected())
    {
        _connection.Disconnect();
    }
}

///
/// @brief Construct from connection.
///
ScopedSignalConnection::ScopedSignalConnection(SignalConnection&& connection)
{
    _connection.Swap(connection);
}

///
/// @brief Construct from scoped connection.
///
ScopedSignalConnection::ScopedSignalConnection(ScopedSignalConnection&& other) noexcept
{
    _connection.Swap(other._connection);
}

///
/// @brief Move assign connection.
///
ScopedSignalConnection& ScopedSignalConnection::operator=(SignalConnection&& other) noexcept
{
    auto tmp = ScopedSignalConnection(std::move(other));
    Swap(tmp);
    return *this;
}

///
/// @brief Move assign connection.
///
ScopedSignalConnection& ScopedSignalConnection::operator=(ScopedSignalConnection&& other) noexcept
{
    auto tmp = ScopedSignalConnection(std::move(other));
    Swap(tmp);
    return *this;
}

///
/// @brief Is connection active?
///
auto ScopedSignalConnection::IsConnected() const -> Bool
{
  return _connection.IsConnected();
}

///
/// @brief Disconnect connection.
///
auto ScopedSignalConnection::Disconnect() -> void
{
    _connection.Disconnect();
}

///
/// @brief Swap connection.
///
auto ScopedSignalConnection::Swap(ScopedSignalConnection& other) noexcept -> void
{
    _connection.Swap(other._connection);
}
}
