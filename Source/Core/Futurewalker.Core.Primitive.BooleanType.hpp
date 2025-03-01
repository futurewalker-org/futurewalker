// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Prelude.hpp"
#include "Futurewalker.Core.Concepts.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class T, class Tag = void>
class Boolean;

///
/// @brief Default boolean type.
///
using Bool = Boolean<bool>;
}
}
