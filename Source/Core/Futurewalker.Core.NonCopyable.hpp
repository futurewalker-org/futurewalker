// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A convenience class to disallow copy operations on a class.
///
/// Inherit this class to delete copy operation of the derived class.
///
/// @note Non-copyable implies non-movable by default.
/// Derived classes must implement move constructor or assignment operator to enable move semantics.
///
struct NonCopyable
{
    NonCopyable() = default;
    NonCopyable(NonCopyable const&) = delete;
    NonCopyable& operator=(NonCopyable const&) = delete;
};
}
}
