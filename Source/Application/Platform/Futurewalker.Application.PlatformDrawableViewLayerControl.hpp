// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformDrawableViewLayerControlType.hpp"
#include "Futurewalker.Application.PlatformViewLayerControl.hpp"

#include "Futurewalker.Graphics.DisplayList.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

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
    ///
    /// @note Renders content of DisplayList into underlying buffer, then presents new content to screen.
    ///
    virtual auto Draw(Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& offset) -> Bool = 0;
};
}
}
