// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Prelude.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.DynamicCastFunction.hpp"

#include <boost/operators.hpp>
#include <boost/functional/hash.hpp>

#include <functional>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Safe pointer wrapper.
///
/// Behaves like T* raw pointers, but cannot be default constructed.
///
template <class T>
class Pointer 
{
    template <class U>
    friend class Pointer;

public:
    Pointer() = delete;
    Pointer(Pointer const&) noexcept = default;
    Pointer& operator=(Pointer const&) noexcept = default;

    ///
    /// @brief Construct from other Pointer.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U*, T*>
    constexpr Pointer(Pointer<U> other) noexcept
      : _ptr {other._ptr}
    {
    }

    ///
    /// @brief Assign from other Pointer.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U*, T*>
    constexpr auto operator=(Pointer<U> other) noexcept -> Pointer&
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
    constexpr Pointer(U) noexcept
      : _ptr {nullptr}
    {
    }

    ///
    /// @brief Assign null pointer.
    ///
    template <class U>
    requires Concepts::SameAs<U, std::nullptr_t>
    constexpr auto operator=(U) noexcept -> Pointer&
    {
        _ptr = nullptr;
        return *this;
    }

    ///
    /// @brief Construct from raw pointer.
    ///
    template <Concepts::Pointer U>
    requires Concepts::ImplicitlyConvertibleTo<U, T*>
    constexpr Pointer(U ptr) noexcept
      : _ptr {ptr}
    {
    }

    ///
    /// @brief Assign null pointer.
    ///
    template <Concepts::Pointer U>
    requires Concepts::ImplicitlyConvertibleTo<U, T*>
    constexpr auto operator=(U ptr) noexcept -> Pointer&
    {
        _ptr = ptr;
        return *this;
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
    constexpr auto operator+=(SInt64 const x) noexcept -> Pointer&
    {
        _ptr += static_cast<ptrdiff_t>(x);
        return *this;
    }

    ///
    /// @brief operator-=
    ///
    constexpr auto operator-=(SInt64 const x) noexcept -> Pointer&
    {
        _ptr -= static_cast<ptrdiff_t>(x);
        return *this;
    }

    ///
    /// @brief operator++
    ///
    constexpr auto operator++() noexcept -> Pointer&
    {
        ++_ptr;
        return *this;
    }

    ///
    /// @brief operator--
    ///
    constexpr auto operator--() noexcept -> Pointer&
    {
        --_ptr;
        return *this;
    }

    ///
    /// @brief operator++
    ///
    constexpr auto operator++(int) noexcept -> Pointer
    {
        auto const tmp = *this;
        ++_ptr;
        return tmp;
    }

    ///
    /// @brief operator--
    ///
    constexpr auto operator--(int) noexcept -> Pointer
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
    /// @brief operator()
    ///
    template <class... Args>
    constexpr auto operator()(Args&&...args) const
    {
        return _ptr(std::forward<Args>(args)...);
    }

    ///
    /// @brief operator<=>
    ///
    friend constexpr auto operator<=>(Pointer const& l, Pointer const& r) noexcept -> std::strong_ordering
    {
        return l._ptr <=> r._ptr;
    }

    ///
    /// @brief operator==
    ///
    friend constexpr bool operator==(Pointer const& l, Pointer const& r) noexcept
    {
        return l._ptr == r._ptr;
    }

    ///
    /// @brief operator+
    ///
    friend constexpr auto operator+(Pointer const& ptr, SInt64 const x) noexcept -> Pointer
    {
        auto tmp = ptr;
        tmp += x;
        return tmp;
    }

    ///
    /// @brief operator-
    ///
    friend constexpr auto operator-(Pointer const& ptr, SInt64 const x) noexcept -> Pointer
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
    auto As() const -> Pointer<U>
    {
        return Pointer<U>(DynamicCastFunction::As<U>(_ptr));
    }

    ///
    /// @brief Cast the pointer into another type, or return null on failure.
    ///
    /// @tparam U
    ///
    /// @return
    ///
    template <class U>
    auto Maybe() const noexcept -> Pointer<U>
    {
        return Pointer<U>(DynamicCastFunction::Maybe<U>(_ptr));
    }

    ///
    /// @brief Cast the pointer into another type without any runtime check.
    ///
    /// @tparam U
    ///
    /// @return
    ///
    template <class U>
    auto Assume() const noexcept -> Pointer<U>
    {
        return Pointer<U>(DynamicCastFunction::Assume<U>(_ptr));
    }

private:
    T* _ptr;
};

template <class T>
Pointer(T*) -> Pointer<T>;
}
}

template <class T>
struct std::hash<FW_NS::Pointer<T>>
{
    constexpr auto operator()(FW_NS::Pointer<T> const& p) const noexcept -> size_t
    {
        return std::hash<T*>()(static_cast<T*>(p));
    }
};

template <class T>
struct boost::hash<FW_NS::Pointer<T>>
{
    constexpr auto operator()(FW_NS::Pointer<T> const& p) const noexcept -> size_t
    {
        return std::hash<T*>()(static_cast<T*>(p));
    }
};
