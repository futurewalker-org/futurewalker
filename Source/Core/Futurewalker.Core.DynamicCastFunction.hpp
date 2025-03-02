// SPDX-License-Identifier: MIT
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
    static auto Assume(T& r) -> U&;

    template <class U, class T>
    static auto Assume(T* p) -> U*;

    template <class U, class T>
    static auto Assume(std::shared_ptr<T>& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto Assume(const std::shared_ptr<T>& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto Assume(std::shared_ptr<T>&& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto Assume(const std::shared_ptr<T>&& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto Maybe(T* p) -> U*;

    template <class U, class T>
    static auto Maybe(std::shared_ptr<T>& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto Maybe(const std::shared_ptr<T>& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto Maybe(std::shared_ptr<T>&& p) -> std::shared_ptr<U>;

    template <class U, class T>
    static auto Maybe(const std::shared_ptr<T>&& p) -> std::shared_ptr<U>;
};

template <class U, class T>
auto DynamicCastFunction::Is(T& r) -> Bool
{
    return Maybe<U>(&r) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::Is(T* p) -> Bool
{
    return Maybe<U>(p) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::Is(const std::shared_ptr<T>& p) -> Bool
{
    return Maybe<U>(p) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::Is(std::shared_ptr<T>& p) -> Bool
{
    return Maybe<U>(p) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::Is(std::shared_ptr<T>&& p) -> Bool
{
    return Maybe<U>(std::move(p)) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::Is(const std::shared_ptr<T>&& p) -> Bool
{
    return Maybe<U>(std::move(p)) != nullptr;
}

template <class U, class T>
auto DynamicCastFunction::As(T& r) -> U&
{
    if (auto ptr = Maybe<U>(&r))
    {
        return *ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::As(T* p) -> U*
{
    if (auto ptr = Maybe<U>(p))
    {
        return ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::As(const std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    if (auto ptr = Maybe<U>(p))
    {
        return ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::As(std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    if (auto ptr = Maybe<U>(p))
    {
        return ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::As(const std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    if (auto ptr = Maybe<U>(std::move(p)))
    {
        return ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::As(std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    if (auto ptr = Maybe<U>(std::move(p)))
    {
        return ptr;
    }
    ExceptionFunction::ThrowDynamicCastException();
}

template <class U, class T>
auto DynamicCastFunction::Assume(T& r) -> U&
{
    return static_cast<U&>(r);
}

template <class U, class T>
auto DynamicCastFunction::Assume(T* p) -> U*
{
    return static_cast<U*>(p);
}

template <class U, class T>
auto DynamicCastFunction::Assume(std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    return std::static_pointer_cast<U>(p);
}

template <class U, class T>
auto DynamicCastFunction::Assume(const std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    return std::static_pointer_cast<U>(p);
}

template <class U, class T>
auto DynamicCastFunction::Assume(std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    return std::static_pointer_cast<U>(std::move(p));
}

template <class U, class T>
auto DynamicCastFunction::Assume(const std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    return std::static_pointer_cast<U>(std::move(p));
}


template <class U, class T>
auto DynamicCastFunction::Maybe(T* p) -> U*
{
    return dynamic_cast<U*>(p);
}

template <class U, class T>
auto DynamicCastFunction::Maybe(std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    return std::dynamic_pointer_cast<U>(p);
}

template <class U, class T>
auto DynamicCastFunction::Maybe(const std::shared_ptr<T>& p) -> std::shared_ptr<U>
{
    return std::dynamic_pointer_cast<U>(p);
}

template <class U, class T>
auto DynamicCastFunction::Maybe(std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    return std::dynamic_pointer_cast<U>(std::move(p));
}

template <class U, class T>
auto DynamicCastFunction::Maybe(const std::shared_ptr<T>&& p) -> std::shared_ptr<U>
{
    return std::dynamic_pointer_cast<U>(std::move(p));
}
}
}
