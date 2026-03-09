// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"
#include "Futurewalker.Core.Concepts.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class T, class Tag = void>
class Boolean;

using Bool = Boolean<bool>;
}
}
