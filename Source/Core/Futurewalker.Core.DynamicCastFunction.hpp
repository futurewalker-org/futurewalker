// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.ExceptionFunction.hpp"

#include <memory>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Low-level helper functions for dynamic casting.
///
class DynamicCastFunction : NonConstructible
{
public:
    template <class U, class T>
    static auto Is(T& r) -> Bool;

    template <class U, class T>
    static auto Is(T* p) -> Bool;

    template <class U, class T>
    static auto Is(const std::shared_ptr<T>& p) -> Bool;
    
    template <class U, class T>
    static auto Is(std::shared_ptr<T>& p) -> Bool;

    template <class U, class T>
    static auto Is(std::shared_ptr<T>&& p) -> Bool;

    template <class U, class T>
    static auto Is(const std::shared_ptr<T>&& p) -> Bool;

    template <class U, class T>
    static auto As(T& r) -> U&;

    template <class U, class T>
    static auto As(T* p) -> U*;

    template <class U, class T>
    static auto As(const std::shared_ptr<T>& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto As(std::shared_ptr<T>& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto As(const std::shared_ptr<T>&& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto As(std::shared_ptr<T>&& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto UnsafeAs(T& r) -> U&;

    template <class U, class T>
    static auto UnsafeAs(T* p) -> U*;

    template <class U, class T>
    static auto UnsafeAs(std::shared_ptr<T>& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto UnsafeAs(const std::shared_ptr<T>& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto UnsafeAs(std::shared_ptr<T>&& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto UnsafeAs(const std::shared_ptr<T>&& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto TryAs(T* p) -> U*;

    template <class U, class T>
    static auto TryAs(std::shared_ptr<T>& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto TryAs(const std::shared_ptr<T>& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto TryAs(std::shared_ptr<T>&& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto TryAs(const std::shared_ptr<T>&& p) -> std::shared_ptr<U>;
};

template <class U, class T>
auto DynamicCastFunction::Is(T& r) -> Bool
{
    return TryAs<U>(&r) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::Is(T* p) -> Bool
{
    return TryAS<U>(p) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::Is(const std::shared_ptr<T>& p) -> Bool
{
    return TryAs<U>(p) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::Is(std::shared_ptr<T>& p) -> Bool
{
    return TryAs<U>(p) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::Is(std::shared_ptr<T>&& p) -> Bool
{
    return TryAs<U>(std::move(p)) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::Is(const std::shared_ptr<T>&& p) -> Bool
{
    return TryAs<U>(std::move(p)) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::As(T& r) -> U&
{
    if (auto ptr = TryAs<U>(&r))
    {
        return *ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::As(T* p) -> U*
{
    if (auto ptr = TryAs<U>(p))
    {
        return ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::As(const std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    if (auto ptr = TryAs<U>(p))
    {
        return ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::As(std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    if (auto ptr = TryAs<U>(p))
    {
        return ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::As(const std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    if (auto ptr = TryAs<U>(std::move(p)))
    {
        return ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::As(std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    if (auto ptr = TryAs<U>(std::move(p)))
    {
        return ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::UnsafeAs(T& r) -> U&
{
    return static_cast<U&>(r);
}

template <class U, class T>
auto DynamicCastFunction::UnsafeAs(T* p) -> U*
{
    return static_cast<U*>(p);
}

template <class U, class T>
auto DynamicCastFunction::UnsafeAs(std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    return std::static_pointer_cast<U>(p);
}

template <class U, class T>
auto DynamicCastFunction::UnsafeAs(const std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    return std::static_pointer_cast<U>(p);
}

template <class U, class T>
auto DynamicCastFunction::UnsafeAs(std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    return std::static_pointer_cast<U>(std::move(p));
}

template <class U, class T>
auto DynamicCastFunction::UnsafeAs(const std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    return std::static_pointer_cast<U>(std::move(p));
}

template <class U, class T>
auto DynamicCastFunction::TryAs(T* p) -> U*
{
    return dynamic_cast<U*>(p);
}

template <class U, class T>
auto DynamicCastFunction::TryAs(std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    return std::dynamic_pointer_cast<U>(p);
}

template <class U, class T>
auto DynamicCastFunction::TryAs(const std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    return std::dynamic_pointer_cast<U>(p);
}

template <class U, class T>
auto DynamicCastFunction::TryAs(std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    return std::dynamic_pointer_cast<U>(std::move(p));
}

template <class U, class T>
auto DynamicCastFunction::TryAs(const std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    return std::dynamic_pointer_cast<U>(std::move(p));
}
}
}
