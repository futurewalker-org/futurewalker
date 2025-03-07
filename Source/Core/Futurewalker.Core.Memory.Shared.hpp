// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Memory.Type.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.DynamicCastFunction.hpp"
#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.ExceptionFunction.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Shared pointer class.
///
template <class T>
class Shared final
{
    std::shared_ptr<T> _ptr;

    template <class U>
    friend class Shared;
    template <class U>
    friend class Weak;
    template <class U>
    friend class Tracked;

    auto GetWeak() const
    {
        return std::weak_ptr(_ptr);
    }

public:
    ///
    /// @brief Allocate and construct Shared<T>.
    ///
    template <class... Args>
    static auto Make(Args&&... args) -> Shared<T>
    {
        return Shared(std::make_shared<T>(std::forward<Args>(args)...));
    }

public:
    Shared() = default;
    Shared(Shared const&) = default;
    Shared& operator=(Shared const&) = default;

    ///
    /// @brief Construct from std::shared_ptr.
    ///
    /// @param[in] sp Shared pointer.
    ///
    /// @note Exists for interop with STL.
    ///
    explicit Shared(std::shared_ptr<T> sp) noexcept
      : _ptr {std::move(sp)}
    {
    }

    ///
    /// @brief Move constructor.
    ///
    Shared(Shared&& other) noexcept
      : _ptr {std::move(other._ptr)}
    {
    }

    ///
    /// @brief Move assignment operator.
    ///
    Shared& operator=(Shared&& other) noexcept
    {
        auto tmp = std::move(other);
        Swap(tmp);
        return *this;
    }

    ///
    /// @brief Construct from other shared.
    ///
    template <class U>
    Shared(Shared<U> other) noexcept
      : _ptr {std::move(other._ptr)}
    {
    }

    ///
    /// @brief Construct by aliasing pointer.
    ///
    /// @param[in] r Reference to refcounted object.
    /// @param[in] p Pointer to alias.
    ///
    template <class U>
    Shared(Shared<U> r, T* p) noexcept
      : _ptr {std::move(r._ptr), p}
    {
    }

    ///
    /// @brief Assign from other.
    ///
    template <class U>
    Shared& operator=(Shared<U> other) noexcept
    {
        _ptr = std::move(other._ptr);
        return *this;
    }

    ///
    /// @brief Construct from other shared.
    ///
    template <class U>
    Shared(Unique<U>&& other) noexcept
      : _ptr {std::move(other._ptr)}
    {
    }

    ///
    /// @brief Assign other shared.
    ///
    template <class U>
    Shared& operator=(Unique<U>&& other) noexcept
    {
        _ptr = std::move(other._ptr);
        return *this;
    }

    ///
    /// @brief Construct from nullptr_t.
    ///
    Shared(std::nullptr_t) noexcept
      : _ptr {nullptr}
    {
    }

    ///
    /// @brief Assign nullptr_t.
    ///
    Shared& operator=(std::nullptr_t) noexcept
    {
        _ptr = nullptr;
        return *this;
    }

    ///
    /// @brief Get pointer.
    ///
    auto GetPointer() const noexcept -> Pointer<T>
    {
        return _ptr.get();
    }

    ///
    /// @brief Dereference.
    ///
    template <Concepts::NonVoid U = T>
    auto operator*() const -> U&
    {
        if (!_ptr)
        {
            ExceptionFunction::ThrowNullDereferenceException();
        }
        return _ptr.operator*();
    }

    ///
    /// @brief Access member of contained value.
    ///
    auto operator->() const -> T*
    {
        if (!_ptr)
        {
            ExceptionFunction::ThrowNullDereferenceException();
        }
        return _ptr.operator->();
    }

    ///
    /// @brief Convert to boolean.
    ///
    explicit operator bool() const noexcept
    {
        return _ptr.operator bool();
    }

    ///
    /// @brief Get reference count.
    ///
    auto GetUseCount() const noexcept -> SInt64
    {
        return _ptr.use_count();
    }

    ///
    /// @brief Reset.
    ///
    auto Reset() noexcept -> void
    {
        _ptr.reset();
    }

    ///
    /// @brief Swap.
    ///
    auto Swap(Shared& other) noexcept -> void
    {
        if (this != &other)
        {
            std::ranges::swap(_ptr, other._ptr);
        }
    }

    ///
    /// @brief Compare based on ownership.
    ///
    template <class U>
    auto IsOwnerBefore(Shared<U> const& other) const -> Bool
    {
        return _ptr.owner_before(other._ptr);
    }

    ///
    /// @brief Check if the holding object is instance of a type.
    ///
    /// @tparam U A type to check.
    ///
    /// @note Null returns false for all type of U.
    ///
    template <class U>
    auto Is() const noexcept -> Bool
    {
        return DynamicCastFunction::Is<U>(_ptr);
    }

    ///
    /// @brief Cast the pointer into another type.
    ///
    /// @tparam U A type to cast into.
    ///
    /// @return
    ///
    /// @throw Exception when owning object is not an instance of U.
    ///
    template <class U>
    auto As() const -> Shared<U>
    {
        return Shared<U>(DynamicCastFunction::As<U>(_ptr));
    }

    ///
    /// @brief Cast the pointer into another type, or return null on failure.
    ///
    /// @tparam U
    ///
    /// @return
    ///
    template <class U>
    auto Maybe() const noexcept -> Shared<U>
    {
        return Shared<U>(DynamicCastFunction::Maybe<U>(_ptr));
    }

    ///
    /// @brief Cast the pointer into another type without any runtime check.
    ///
    /// @tparam U
    ///
    /// @return
    ///
    template <class U>
    auto Assume() const noexcept -> Shared<U>
    {
        return Shared<U>(DynamicCastFunction::Assume<U>(_ptr));
    }

    template <class U, class V>
    friend constexpr auto operator<=>(Shared<U> const& lhs, Shared<V> const& rhs) -> std::strong_ordering;

    template <class U, class V>
    friend constexpr auto operator==(Shared<U> const& lhs, Shared<V> const& rhs) -> bool;

    template <class U>
    friend constexpr auto operator==(Shared<U> const& lhs, std::nullptr_t rhs) -> bool;
};

///
/// @brief Three-way comparison.
///
template <class U, class V>
constexpr auto operator<=>(Shared<U> const& lhs, Shared<V> const& rhs) -> std::strong_ordering
{
    return lhs._ptr <=> rhs._ptr;
}

///
/// @brief Equality comparison.
///
template <class U, class V>
constexpr auto operator==(Shared<U> const& lhs, Shared<V> const& rhs) -> bool
{
    return lhs._ptr == rhs._ptr;
}

///
/// @brief  Equality comparison.
///
template <class U>
constexpr auto operator==(Shared<U> const& lhs, std::nullptr_t rhs) -> bool
{
    return lhs._ptr == rhs;
}

template <class T>
Shared(Unique<T>) -> Shared<T>;
}
}
