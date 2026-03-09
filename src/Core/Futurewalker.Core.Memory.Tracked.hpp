// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Memory.Type.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Tracked pointer class.
///
/// Tracked<T> is (even more) weaker version of Weak<T>.
/// It only provides functionality to check liveness of tracked object.
/// Unlike Weak<T>, Tracked<T> can be constructed from Unique<T>.
///
template <class T>
class Tracked final
{
    std::weak_ptr<T> _weak;

    template <class U>
    friend class Tracked;

public:
    Tracked() = default;
    Tracked(Tracked const&) = default;
    Tracked& operator=(Tracked const&) = default;

    ///
    /// @brief Construct from rvalue.
    ///
    Tracked(Tracked&& other) noexcept
    {
        Swap(other);
    }

    ///
    /// @brief Assign rvalue.
    ///
    Tracked& operator=(Tracked&& other) noexcept
    {
        auto tmp = std::move(other);
        Swap(tmp);
        return *this;
    }

    ///
    /// @brief Construct from nullptr.
    ///
    Tracked(std::nullptr_t) noexcept
      : _weak {}
    {
    }

    ///
    /// @brief Copy from other weak.
    ///
    template <class U>
    Tracked(Tracked<U> const& other)
      : _weak {other._weak}
    {
    }

    ///
    /// @brief Assign other weak.
    ///
    template <class U>
    Tracked& operator=(Tracked<U> const& other)
    {
        _weak = other._weak;
        return *this;
    }

    ///
    /// @brief Construct from Unique<T>.
    ///
    template <class U>
    Tracked(Unique<U> const& u)
      : _weak {u._ptr}
    {
    }

    ///
    /// @brief Construct from Shared<T>.
    ///
    template <class U>
    Tracked(Shared<U> const& u)
      : _weak {u._ptr}
    {
    }

    ///
    /// @brief Construct from Weak<T>.
    ///
    template <class U>
    Tracked(Weak<U> const& u)
      : _weak {u._weak}
    {
    }

    ///
    /// @brief Check if weak reference is invalid.
    ///
    Bool IsExpired() const
    {
        return _weak.expired();
    }

    ///
    /// @brief Reset.
    ///
    void Reset() noexcept
    {
        _weak.reset();
    }

    ///
    /// @brief Swap.
    ///
    void Swap(Tracked& other) noexcept
    {
        if (this != &other)
        {
            std::ranges::swap(_weak, other._weak);
        }
    }
};

template <class T>
Tracked(Unique<T>) -> Tracked<T>;
template <class T>
Tracked(Shared<T>) -> Tracked<T>;
template <class T>
Tracked(Weak<T>) -> Tracked<T>;
}
}