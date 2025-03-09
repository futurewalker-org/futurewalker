// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformDrawableViewLayerControlType.hpp"
#include "Futurewalker.Application.PlatformViewLayerControl.hpp"

#include "Futurewalker.Graphics.DisplayList.hpp"
#include "Futurewalker.Graphics.Rect.hpp"
#include "Futurewalker.Graphics.Offset.hpp"
#include "Futurewalker.Graphics.GraphicsUnits.hpp"

#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Drawable view layer's control object.
///
class PlatformDrawableViewLayerControl : public PlatformViewLayerControl
{
public:
    ///
    /// @brief Draw content of composition view.
    ///
    /// @param[in] displayList Display list containing new content of composition view
    /// @param[in] offset Offset of content
    /// @param[in] scale Scale of content
    ///
    /// @note Renders content of DisplayList into underlying buffer, then presents new content to screen.
    ///
    virtual auto Draw(Shared<DisplayList> const& displayList, Offset<Dp> const& offset, Float64 const scale) -> Bool = 0;
};
}
}
