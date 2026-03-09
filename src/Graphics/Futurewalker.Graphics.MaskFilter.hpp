// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.MaskFilterType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Mask filter.
///
class MaskFilter : NonCopyable
{
public:
    static auto MakeBlur(BlurStyle const style, Dp const sigma) -> Shared<MaskFilter>;

    virtual ~MaskFilter() = 0;
};
}
}
