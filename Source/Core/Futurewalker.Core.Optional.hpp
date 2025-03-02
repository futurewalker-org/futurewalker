// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.TypeTraits.hpp"

#include <optional>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT 
{
///
/// @brief Optional container class.
///
/// Contains either value of T or nothing.
///
template <class T>
class Optional final : std::optional<T>
{
    using Base = std::optional<T>;

public:
    using std::optional<T>::optional;

    ///
    /// @brief Disable nullopt_t constructor.
    ///
    Optional(std::nullopt_t) = delete;

    ///
    /// @brief Returns true if the optional has a value.
    ///
    auto HasValue() const noexcept -> Bool
    {
        return Base::has_value();
    }

    ///
    /// @brief Access value.
    ///
    /// @throw Exception if empty.
    ///
    template <class Self>
    auto operator*(this Self&& self) -> decltype(auto)
    {
        if (self.HasValue())
        {
            return static_cast<TypeTraits::PropagateCVRef<decltype(self), Base>>(self).operator*();
        }
        throw Exception(ErrorCode::InvalidOperation);
    }

    ///
    /// @brief operator->
    ///
    /// @throw Exception if empty.
    ///
    template <class Self>
    auto operator->(this Self&& self) -> auto*
    {
        if (self.HasValue())
        {
            return static_cast<TypeTraits::PropagateCVRef<decltype(self), Base>>(self).operator->();
        }
        throw Exception(ErrorCode::InvalidOperation);
    }

    ///
    /// @brief Emplace value.
    ///
    /// @tparam Args Argument types.
    ///
    /// @param args Arguments.
    ///
    template <class... Args>
    auto Emplace(Args&&... args) -> void
    {
        Base::emplace(std::forward<Args>(args)...);
    }

    ///
    /// @brief Reset.
    ///
    auto Reset() -> void
    {
        Base::reset();
    }

    ///
    /// @brief Conversion operator to bool.
    ///
    explicit operator bool() const noexcept
    {
        return Base::operator bool();
    }
};

template <class T>
Optional(T) -> Optional<T>;
}
}
