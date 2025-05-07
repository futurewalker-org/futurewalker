// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Prelude.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT 
{
///
/// @brief RAII class which resets value at the end of scope.
///
template <class T>
class [[nodiscard]] ScopedResetter : NonCopyable
{
public:
    ///
    /// @brief Constructor.
    ///
    /// @param ref Reference to value to reset
    ///
    ScopedResetter(T& ref)
      : _ref {ref}
      , _original {ref}
    {
    }

    ///
    /// @brief Destructor which resets original value.
    ///
    ~ScopedResetter()
    {
        Reset();
    }

    ///
    /// @brief Set original value to the reference. 
    ///
    auto Reset() -> void
    {
        _ref = _original;
    }

private:
    T& _ref;
    T _original;
};

template <class T>
ScopedResetter(T&)->ScopedResetter<T>;
}
}
