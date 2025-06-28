// SPDX-License-Identifier: MPL-2.0
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
/// Relation with Vp: Dp = Vp / DisplayScale.
///
/// Mainly used for view layout and rendering coordinates.
///
using Dp = Float<float64_t, class UnitTagDp>;

///
/// @brief Tagged float type for virtual pixels.
///
/// Represents virtual screen pixel coordinate values.
///
/// Relation with Dp: Vp = Dp * DisplayScale.
/// Relation with Px: Vp = Px / BackingScale.
///
/// Mainly used for platform screen coordinates.
///
using Vp = Float<float64_t, class UnitTagVp>;

///
/// @brief Tagged float type for raw pixels.
///
/// Represents pixel coordinate of rendering buffer.
///
/// Relation with Vp: Px = Vp * BackingScale.
///
/// Mainly used for platform window rendering and bitmap cache.
///
using Px = Float<float64_t, class UnitTagPx>;

///
/// @brief Integer variant of Dp.
///
using IntDp = Integer<int64_t, class UnitTagDp>;

///
/// @brief Integer variant of Vp.
///
using IntVp = Integer<int64_t, class UnitTagVp>;

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
