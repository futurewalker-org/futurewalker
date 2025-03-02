// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Detects build configurations.
///
class BuildConfig : NonConstructible
{
public:
    static constexpr Bool IsDebug() noexcept;
};

///
/// @brief Returns true if the library is being built in debug mode.
///
/// @note Do not assume this function depends on _NDEBUG macro.
///
constexpr Bool BuildConfig::IsDebug() noexcept
{
#if defined(_NDEBUG)
    return false;
#else
    return true;
#endif
}
}
}
