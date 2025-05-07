// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Tagged float type for DPI independent pixels.
///
/// Represents DPI independent pixel coordinate values.
///
/// Relation with Vp: Dp = Sp / DisplayScale.
///
/// Mainly used for view layout and rendering coordinates.
///
using Dp = Float<float64_t, class UnitTagDp>;

///
/// @brief Tagged float type for virtual pixels.
///
/// Represents DPI scaled pixel coordinate values.
///
/// Relation with Dp: Sp = Dp * DisplayScale.
/// Relation with Px: Sp = Px / BackingScale.
///
/// Mainly used for platform window coordinates.
///
using Sp = Float<float64_t, class UnitTagSp>;

///
/// @brief Tagged float type for raw pixels.
///
/// Represents pixel coordinate of rendering buffer.
///
/// Relation with Vp: Px = Sp * BackingScale.
///
/// Mainly used for platform window rendering and bitmap cache.
///
using Px = Float<float64_t, class UnitTagPx>;

///
/// @brief Integer variant of Dp.
///
using IntDp = Integer<int64_t, class UnitTagDp>;

///
/// @brief Integer variant of Sp.
///
using IntSp = Integer<int64_t, class UnitTagSp>;

///
/// @brief Integer variant of Px.
///
using IntPx = Integer<int64_t, class UnitTagPx>;

///
/// @brief DPI scaling value.
///
using DisplayScale = Float<float64_t, class UnitTagDisplayScale>;

///
/// @brief Backing scaling value.
///
using BackingScale = Float<float64_t, class UnitTagBackingScale>;
}
}
