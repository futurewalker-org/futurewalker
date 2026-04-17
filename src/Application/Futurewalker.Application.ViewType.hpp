// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Identifier.hpp"
#include "Futurewalker.Core.TypeTag.hpp"

#include <vector>
#include <list>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class View;
using ViewId = Identifier<UInt64, TypeTag<View, 0>>;
using ViewArray = std::vector<Shared<View>>;
using ConstViewArray = std::vector<Shared<View const>>;
using ViewList = std::list<Shared<View>>;
using ConstViewList = std::list<Shared<View const>>;

///
/// @brief View's layout direction.
///
enum class ViewLayoutDirection
{
    Inherited,   ///< Inherits direction of surrounding context.
    LeftToRight, ///< LTR direction.
    RightToLeft, ///< RTL direction.
};

///
/// @brief View's clipping style.
///
enum class ViewClipMode
{
    None,   ///< No clipping.
    Bounds, ///< Clips to bounds.
};

///
/// @brief View's pointer tracking flags.
///
enum class ViewPointerTrackingFlag
{
    None = 0,     ///< No pointer tracking.
    All = 1 << 0, ///< Track all pointer intputs.
};

///
/// @brief View's focus tracking flags.
///
enum class ViewFocusTrackingFlag
{
    None = 0,     ///< No focus tracking.
    All = 1 << 0, ///< Track all focus events.
};
}
}
