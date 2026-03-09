// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.StaticPointerType.hpp"
#include "Futurewalker.Core.StaticReferenceType.hpp"
#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Safe pointer wrapper which can only point to objects with static storage duration.
///
/// Behaves like T* raw pointers, but cannot be default constructed.
///
/// @tparam T Type of pointee.
///
template <class T>
class StaticPointer 
{
    template <class U>
    friend class StaticPointer;

public:
    StaticPointer() = delete;
    StaticPointer(StaticPointer const&) noexcept = default;
    StaticPointer& operator=(StaticPointer const&) noexcept = default;

    ///
    /// @brief Construct from other StaticPointer.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U*, T*>
    constexpr StaticPointer(StaticPointer<U> other) noexcept
      : _ptr {other._ptr}
    {
    }

    ///
    /// @brief Assign from other StaticPointer.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U*, T*>
    constexpr auto operator=(StaticPointer<U> other) noexcept -> StaticPointer&
    {
        _ptr = other._ptr;
        return *this;
    }

    ///
    /// @brief Construct from nullptr_t.
    ///
    /// @note Uses constrained template parameter to avoid implicit conversion to nullptr_t.
    ///
    template <class U>
    requires Concepts::SameAs<U, std::nullptr_t>
    constexpr StaticPointer(U) noexcept
      : _ptr {nullptr}
    {
    }

    ///
    /// @brief Assign null pointer.
    ///
    template <class U>
    requires Concepts::SameAs<U, std::nullptr_t>
    constexpr auto operator=(U) noexcept -> StaticPointer&
    {
        _ptr = nullptr;
        return *this;
    }

    ///
    /// @brief Construct from raw pointer.
    ///
    template <Concepts::Pointer U>
    requires Concepts::ImplicitlyConvertibleTo<U, T*>
    consteval StaticPointer(U ptr) noexcept
      : _ptr {ptr}
    {
    }

    ///
    /// @brief Assign raw pointer.
    ///
    template <Concepts::Pointer U>
    requires Concepts::ImplicitlyConvertibleTo<U, T*>
    consteval auto operator=(U ptr) noexcept -> StaticPointer&
    {
        _ptr = ptr;
        return *this;
    }

    ///
    /// @brief Construct from raw pointer.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U*, T*>
    consteval StaticPointer(Pointer<U> ptr)
      : _ptr {static_cast<U*>(ptr)}
    {
    }

    ///
    /// @brief Assign raw pointer.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U*, T*>
    consteval auto operator=(Pointer<U> ptr) noexcept -> StaticPointer&
    {
        _ptr = static_cast<U*>(ptr);
        return *this;
    }

    ///
    /// @brief Construct from raw pointer (via StaticReference).
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U*, T*>
    constexpr StaticPointer(PassKey<StaticReference<U>>, Pointer<U> ptr)
      : _ptr {static_cast<U*>(ptr)}
    {
    }

    ///
    /// @brief Convert to pointer type.
    ///
    template <Concepts::Pointer U>
    requires Concepts::ImplicitlyConvertibleTo<T*, U>
    constexpr explicit operator U() const noexcept
    {
        return static_cast<U>(_ptr);
    }

    ///
    /// @brief Convert to other StaticPointer type.
    ///
    template <class U>
    requires (!Concepts::ImplicitlyConvertibleTo<T*, U*>) && Concepts::ExplicitlyConvertibleTo<T*, U*>
    constexpr explicit operator StaticPointer<U>() const noexcept
    {
        return StaticPointer<U>(static_cast<U*>(_ptr));
    }

    ///
    /// @brief Convert to other Pointer type.
    ///
    template <class U>
    requires (!Concepts::ImplicitlyConvertibleTo<T*, U*>) && Concepts::ExplicitlyConvertibleTo<T*, U*>
    constexpr explicit operator Pointer<U>() const noexcept
    {
        return Pointer<U>(static_cast<U*>(_ptr));
    }

    ///
    /// @brief operator bool 
    ///
    constexpr explicit operator bool() const noexcept
    {
        return static_cast<bool>(_ptr);
    }

    ///
    /// @brief operator*
    ///
    constexpr auto operator*() const noexcept -> auto&
    requires Concepts::NonVoid<T>
    {
        return *_ptr;
    }

    ///
    /// @brief operator->
    ///
    constexpr auto operator->() const noexcept -> T*
    {
        return _ptr;
    }

    ///
    /// @brief operator+=
    ///
    constexpr auto operator+=(SInt64 const x) noexcept -> StaticPointer&
    {
        _ptr += static_cast<ptrdiff_t>(x);
        return *this;
    }

    ///
    /// @brief operator-=
    ///
    constexpr auto operator-=(SInt64 const x) noexcept -> StaticPointer&
    {
        _ptr -= static_cast<ptrdiff_t>(x);
        return *this;
    }

    ///
    /// @brief operator++
    ///
    constexpr auto operator++() noexcept -> StaticPointer&
    {
        ++_ptr;
        return *this;
    }

    ///
    /// @brief operator--
    ///
    constexpr auto operator--() noexcept -> StaticPointer&
    {
        --_ptr;
        return *this;
    }

    ///
    /// @brief operator++
    ///
    constexpr auto operator++(int) noexcept -> StaticPointer
    {
        auto const tmp = *this;
        ++_ptr;
        return tmp;
    }

    ///
    /// @brief operator--
    ///
    constexpr auto operator--(int) noexcept -> StaticPointer
    {
        auto const tmp = *this;
        --_ptr;
        return tmp;
    }

    ///
    /// @brief operator[]
    ///
    constexpr auto operator[](SInt64 const d) const noexcept -> auto&
    requires Concepts::NonVoid<T>
    {
        return _ptr[static_cast<ptrdiff_t>(d)];
    }

    ///
    /// @brief operator<=>
    ///
    friend constexpr auto operator<=>(StaticPointer const& l, StaticPointer const& r) noexcept -> std::strong_ordering
    {
        return l._ptr <=> r._ptr;
    }

    ///
    /// @brief operator==
    ///
    friend constexpr bool operator==(StaticPointer const& l, StaticPointer const& r) noexcept
    {
        return l._ptr == r._ptr;
    }

    ///
    /// @brief operator+
    ///
    friend constexpr auto operator+(StaticPointer const& ptr, SInt64 const x) noexcept -> StaticPointer
    {
        auto tmp = ptr;
        tmp += x;
        return tmp;
    }

    ///
    /// @brief operator-
    ///
    friend constexpr auto operator-(StaticPointer const& ptr, SInt64 const x) noexcept -> StaticPointer
    {
        auto tmp = ptr;
        tmp -= x;
        return tmp;
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
    auto As() const -> StaticPointer<U>
    {
        return StaticPointer<U>(DynamicCastFunction::As<U>(_ptr));
    }

    ///
    /// @brief Cast the pointer into another type, or return null on failure.
    ///
    /// @tparam U
    ///
    /// @return
    ///
    template <class U>
    auto TryAs() const noexcept -> StaticPointer<U>
    {
        return StaticPointer<U>(DynamicCastFunction::TryAs<U>(_ptr));
    }

    ///
    /// @brief Cast the pointer into another type without any runtime check.
    ///
    /// @tparam U
    ///
    /// @return
    ///
    template <class U>
    auto UnsafeAs() const noexcept -> StaticPointer<U>
    {
        return StaticPointer<U>(DynamicCastFunction::UnsafeAs<U>(_ptr));
    }

private:
    T* _ptr;
};

template <class T>
StaticPointer(T*) -> StaticPointer<T>;
}
}

template <class T>
struct std::hash<FW_NS::StaticPointer<T>>
{
    constexpr auto operator()(FW_NS::StaticPointer<T> const& p) const noexcept -> size_t
    {
        return std::hash<T*>()(static_cast<T*>(p));
    }
};

template <class T>
struct boost::hash<FW_NS::StaticPointer<T>>
{
    constexpr auto operator()(FW_NS::StaticPointer<T> const& p) const noexcept -> size_t
    {
        return std::hash<T*>()(static_cast<T*>(p));
    }
};
