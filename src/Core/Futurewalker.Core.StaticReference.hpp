// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.StaticReferenceType.hpp"
#include "Futurewalker.Core.StaticPointer.hpp"
#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.TypeTraits.hpp"
#include "Futurewalker.Core.StaticPointer.hpp"

#include <memory>

namespace FW_DETAIL_NS
{
// See LWG Issue 2993 for details.
// https://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#2993
template <class T>
void StaticReferenceCtorTest(T&) noexcept; // definition is omitted.
template <class T>
void StaticReferenceCtorTest(T&&) = delete;
template <class T, class U>
concept ValidStaticReferenceCtorArg = requires(U u) { StaticReferenceCtorTest<T>(u); };

namespace FW_EXPORT
{
///
/// @brief Reference wrapper class which can only refer to objects with static storage duration.
///
template <class T>
class StaticReference
{
    template <class U>
    friend class StaticReference;

public:
    StaticReference() = delete;

    template <class U>
    requires(!Concepts::SameAs<std::remove_cvref_t<U>, StaticReference> && ValidStaticReferenceCtorArg<T, U>)
    consteval StaticReference(U&& ref) noexcept(noexcept(StaticReferenceCtorTest<T>(std::declval<U>())))
      : _ref {std::addressof(std::forward<U>(ref))}
    {
    }

    template <class U>
    constexpr StaticReference(StaticReference<U> const& other) noexcept
      : _ref {other._ref}
    {
    }

    template <class U>
    constexpr auto operator=(StaticReference<U> const& other) noexcept -> StaticReference&
    {
        _ref = other._ref;
        return *this;
    }

    ///
    /// @brief Get underlying reference.
    ///
    constexpr auto Get() const noexcept -> T&
    {
        return *_ref;
    }

    ///
    /// @brief Get address of underlying reference.
    ///
    constexpr auto GetPointer() const noexcept -> StaticPointer<T>
    {
        return StaticPointer<T>(PassKey<StaticReference>(), _ref);
    }

private:
    Pointer<T> _ref = nullptr;
};

template <class T>
StaticReference(T&) -> StaticReference<T>;
}
}
