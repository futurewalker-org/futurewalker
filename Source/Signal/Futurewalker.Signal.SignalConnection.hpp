// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Signal.SignalConnectionType.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

#include <boost/signals2.hpp>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Signal connection handle.
///
class SignalConnection final
{
public:
    SignalConnection() = default;
    SignalConnection(SignalConnection const&) = default;
    SignalConnection& operator=(SignalConnection const&) = default;

    auto IsConnected() const -> Bool;
    auto Disconnect() -> void;
    auto Swap(SignalConnection& other) noexcept -> void;

private:
    template <class Signature, template <class> class Combiner>
    friend class Signal;

    SignalConnection(boost::signals2::connection c);

private:
    boost::signals2::connection _connection;
};

///
/// @brief RAII wrapper for signal connection.
///
class ScopedSignalConnection final : NonCopyable
{
public:
    ScopedSignalConnection() = default;
    ~ScopedSignalConnection() noexcept;
    ScopedSignalConnection(SignalConnection&& connection);
    ScopedSignalConnection(ScopedSignalConnection&& other) noexcept;
    ScopedSignalConnection& operator=(SignalConnection&& other) noexcept;
    ScopedSignalConnection& operator=(ScopedSignalConnection&& other) noexcept;

    auto IsConnected() const -> Bool;
    auto Disconnect() -> void;
    auto Swap(ScopedSignalConnection& other) noexcept -> void;

private:
    SignalConnection _connection;
};
}
}
