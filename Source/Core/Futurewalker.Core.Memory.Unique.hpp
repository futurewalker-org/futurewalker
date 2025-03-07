// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Memory.Type.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.ExceptionFunction.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Unique pointer class.
///
template <class T>
class Unique final : NonCopyable
{
    std::shared_ptr<T> _ptr;

    template <class U>
    friend class Unique;
    template <class U>
    friend class Shared;
    template <class U>
    friend class Tracked;

    explicit Unique(std::shared_ptr<T>&& up) noexcept
      : _ptr {std::move(up)}
    {
    }

    auto GetWeak() const
    {
        return std::weak_ptr(_ptr);
    }
    
public:
    ///
    /// @brief Make unique pointer.
    ///
    /// @param[in] args Parameters for constructor of T.
    ///
    template <class... Args>
    static auto Make(Args&&... args) -> Unique<T>
    {
        return Unique(std::make_shared<T>(std::forward<Args>(args)...));
    }

public:
    ///
    /// @brief Default constructor.
    ///
    Unique() = default;

    ///
    /// @brief Construct from std::unique_ptr.
    ///
    explicit Unique(std::unique_ptr<T>&& up) noexcept
      : _ptr {std::move(up)}
    {
    }

    ///
    /// @brief Move constructor.
    ///
    Unique(Unique&& other) noexcept
    {
        Swap(other);
    }

    ///
    /// @brief Move assignment operator.
    ///
    Unique& operator=(Unique&& other) noexcept
    {
        auto tmp = std::move(other);
        Swap(tmp);
        return *this;
    }

    ///
    /// @brief Construct from other Unique.
    ///
    template <class U>
    Unique(Unique<U>&& other) noexcept
      : _ptr {std::move(other._ptr)}
    {
    }

    ///
    /// @brief Assign from other Unique.
    ///
    template <class U>
    Unique& operator=(Unique<U>&& other) noexcept
    {
        _ptr = std::move(other._ptr);
        return *this;
    }

    ///
    /// @brief Construct from nullptr_t.
    ///
    Unique(std::nullptr_t) noexcept
      : _ptr {nullptr}
    {
    }

    ///
    /// @brief Assign nullptr_t.
    ///
    Unique& operator=(std::nullptr_t) noexcept
    {
        _ptr = nullptr;
        return *this;
    }

    ///
    /// @brief Get pointer.
    ///
    auto GetPointer() const noexcept -> T*
    {
        return _ptr.get();
    }

    ///
    /// @brief Dereference.
    ///
    template <Concepts::NonVoid U = T>
    auto operator*() const noexcept -> U&
    {
        if (!_ptr)
        {
            ExceptionFunction::ThrowNullDereferenceException();
        }
        return _ptr.operator*();
    }

    ///
    /// @brief Access member of object.
    ///
    auto operator->() const noexcept -> T*
    {
        if (!_ptr)
        {
            ExceptionFunction::ThrowNullDereferenceException();
        }
        return _ptr.operator->();
    }

    ///
    /// @brief Convert to boolean value.
    ///
    explicit operator bool() const noexcept
    {
        return static_cast<bool>(_ptr);
    }

    ///
    /// @brief Reset.
    ///
    void Reset() noexcept
    {
        _ptr.reset();
    }

    ///
    /// @brief Swap.
    ///
    void Swap(Unique& other) noexcept
    {
        if (this != &other)
        {
            std::ranges::swap(_ptr, other._ptr);
        }
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
    auto As() && -> Unique<U>
    {
        return Unique<U>(DynamicCastFunction::As<U>(_ptr));
    }

    ///
    /// @brief Cast the pointer into another type, or return null on failure.
    ///
    /// @tparam U
    ///
    /// @return
    ///
    template <class U>
    auto Maybe() && noexcept -> Unique<U>
    {
        return Unique<U>(DynamicCastFunction::Maybe<U>(_ptr));
    }

    ///
    /// @brief Cast the pointer into another type without any runtime check.
    ///
    /// @tparam U
    ///
    /// @return
    ///
    template <class U>
    auto Assume() && noexcept -> Unique<U>
    {
        return Unique<U>(DynamicCastFunction::Assume<U>(_ptr));
    }

    template <class U, class V>
    friend constexpr auto operator<=>(Unique<U> const& lhs, Unique<V> const& rhs) -> std::strong_ordering;

    template <class U, class V>
    friend constexpr auto operator==(Unique<U> const& lhs, Unique<V> const& rhs) -> bool;

    template <class U>
    friend constexpr auto operator==(Unique<U> const& lhs, std::nullptr_t rhs) -> bool;
};

///
/// @brief Three-way comparison.
///
template <class U, class V>
constexpr auto operator<=>(Unique<U> const& lhs, Unique<V> const& rhs) -> std::strong_ordering
{
    return lhs._ptr <=> rhs._ptr;
}

///
/// @brief Equality comparison.
///
template <class U, class V>
constexpr auto operator==(Unique<U> const& lhs, Unique<V> const& rhs) -> bool
{
    return lhs._ptr == rhs._ptr;
}

///
/// @brief Equality comparison.
///
template <class U>
constexpr auto operator==(Unique<U> const& lhs, std::nullptr_t rhs) -> bool
{
    return lhs._ptr == rhs;
}
}
}