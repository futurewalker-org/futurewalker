// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ScrollView;

enum class ScrollViewDirection
{
    None,
    Horizontal = 1 << 0,
    Vertical = 1 << 1,
};
}
}
