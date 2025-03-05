// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Pointer.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Argument reference wrapper.
///
/// Helper class for function arguments that can accept both T& and Shared<T>.
///
/// This class does not manage ownership of referenced objects; it just holds a pointer.
/// Since Shared<T> can be null, this class also requires a null check to access holding reference.
///
template <class T>
class ReferenceArg : NonCopyable
{
public:
    ///
    /// @brief This class is not default constructible.
    ///
    ReferenceArg() = delete;

    ///
    /// @brief Constructor for T&.
    ///
    /// @param ref Plain reference.
    ///
    template <class U>
    ReferenceArg(U& ref) noexcept
      : _ptr {&ref}
    {
    }

    ///
    /// @brief Constructor for Shared<T>.
    ///
    /// @param ref Shared reference.
    ///
    template <class U>
    ReferenceArg(Shared<U> shared) noexcept
      : _ptr {shared.GetPointer()}
    {
    }

    ///
    /// @brief bool conversion.
    ///
    explicit operator bool() const noexcept
    {
        return static_cast<bool>(_ptr);
    }

    ///
    /// @brief Get pointer.
    ///
    auto GetPointer() const noexcept -> Pointer<T>
    {
        return _ptr;
    }

    ///
    /// @brief Dereference.
    ///
    /// @return Valid pointer to referenced object.
    ///
    /// @throw Exception when null.
    ///
    auto operator->() const -> Pointer<T>
    {
        if (_ptr)
        {
            return _ptr;
        }
        throw Exception(ErrorCode::InvalidOperation, "Invalid pointer");
    }

    ///
    /// @brief Dereference.
    ///
    /// @return Reference.
    ///
    /// @throw Exception when null.
    ///
    auto operator*() const -> T&
    {
        if (_ptr)
        {
            return *_ptr;
        }
        throw Exception(ErrorCode::InvalidOperation, "Invalid pointer");
    }

private:
    T* _ptr = nullptr;
};
}
}
