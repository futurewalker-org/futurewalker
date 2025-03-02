// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Memory.Type.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Weak pointer class.
///
template <class T>
class Weak final
{
    std::weak_ptr<T> _weak;

    template <class U>
    friend class Weak;
    template <class U>
    friend class Tracked;

public:
    Weak() = default;
    Weak(const Weak&) = default;
    Weak& operator=(const Weak&) = default;

    ///
    /// @brief Construct from rvalue.
    ///
    Weak(Weak&& other) noexcept
    {
        Swap(other);
    }

    ///
    /// @brief Assign rvalue.
    ///
    Weak& operator=(Weak&& other) noexcept
    {
        auto tmp = std::move(other);
        Swap(tmp);
        return *this;
    }

    ///
    /// @brief Construct from nullptr.
    ///
    Weak(std::nullptr_t) noexcept
      : _weak {}
    {
    }

    ///
    /// @brief Copy from other weak.
    ///
    template <class U>
    Weak(const Weak<U>& other)
      : _weak {other._weak}
    {
    }

    ///
    /// @brief Assign other weak.
    ///
    template <class U>
    Weak& operator=(const Weak<U>& other)
    {
        _weak = other._weak;
        return *this;
    }

    ///
    /// @brief Construct from shared<T>.
    ///
    template <class U>
    Weak(const Shared<U>& u)
      : _weak {u._ptr}
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
    void Swap(Weak& other) noexcept
    {
        if (this != &other)
        {
            std::ranges::swap(_weak, other._weak);
        }
    }

    ///
    /// @brief Compare based on ownership.
    ///
    template <class U>
    Bool IsOwnerBefore(const Weak<U>& other) const
    {
        return _weak.owner_before(other._weak);
    }

    ///
    /// @brief Upgrade weak reference to shared reference.
    ///
    [[nodiscard]] auto Lock() const -> Shared<T>
    {
        return Shared(_weak.lock());
    }
};

template <class T>
Weak(Unique<T>) -> Weak<T>;

template <class T>
Weak(Shared<T>) -> Weak<T>;
}
}