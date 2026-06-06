// SPDX-License-Identifier: MPL-2.0
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
    Weak(Weak const&) = default;
    Weak& operator=(Weak const&) = default;

    ///
    /// @brief Construct from rvalue.
    ///
    inline Weak(Weak&& other) noexcept
    {
        Swap(other);
    }

    ///
    /// @brief Assign rvalue.
    ///
    inline auto operator=(Weak&& other) noexcept -> Weak&
    {
        auto tmp = std::move(other);
        Swap(tmp);
        return *this;
    }

    ///
    /// @brief Construct from nullptr.
    ///
    inline Weak(std::nullptr_t) noexcept
      : _weak {}
    {
    }

    ///
    /// @brief Copy from other weak.
    ///
    template <class U>
    inline Weak(Weak<U> const& other) noexcept
      : _weak {other._weak}
    {
    }

    ///
    /// @brief Assign other weak.
    ///
    template <class U>
    inline auto operator=(Weak<U> const& other) noexcept -> Weak&
    {
        _weak = other._weak;
        return *this;
    }

    ///
    /// @brief Construct from shared<T>.
    ///
    template <class U>
    inline Weak(Shared<U> const& u) noexcept
      : _weak {u._ptr}
    {
    }

    ///
    /// @brief Check if weak reference is invalid.
    ///
    inline auto IsExpired() const noexcept -> Bool
    {
        return _weak.expired();
    }

    ///
    /// @brief Reset.
    ///
    inline auto Reset() noexcept -> void
    {
        _weak.reset();
    }

    ///
    /// @brief Swap.
    ///
    inline auto Swap(Weak& other) noexcept -> void
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
    inline auto IsOwnerBefore(Weak<U> const& other) const -> Bool
    {
        return _weak.owner_before(other._weak);
    }

    ///
    /// @brief Upgrade weak reference to shared reference.
    ///
    [[nodiscard]] inline auto Lock() const -> Shared<T>
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