// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief An empty class which can only be created from T.
///
/// Passkey idiom helper.
///
/// You can create function which can only be called from T by specifying PassKey<T> as an argument.
///
/// @param T A class which is allowed to create instance of this class.
///
/// @note This class is copyable just for convenience.
///
/// @todo Make this class to variadic template when P2893 lands.
///
template <class T>
class [[maybe_unused]] PassKey final
{
    friend T;
    PassKey() = default;
};
}
}
