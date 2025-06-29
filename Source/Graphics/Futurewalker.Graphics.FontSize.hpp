// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Prelude.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Font size value.
///
class FontSize final : public Integer<int64_t, FontSize>
{
public:
    using Integer<int64_t, FontSize>::Integer;
};
}
}
