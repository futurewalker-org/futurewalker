// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A convenience class to disallow instantiation of a class.
///
/// Inherit this class to delete constructors of the derived class.
///
struct NonConstructible
{
    NonConstructible() = delete;
    NonConstructible(NonConstructible const&) = delete;
};
}
}