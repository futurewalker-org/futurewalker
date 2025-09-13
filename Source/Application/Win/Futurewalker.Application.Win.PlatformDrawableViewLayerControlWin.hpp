// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformDrawableViewLayerControlWinType.hpp"
#include "Futurewalker.Application.Win.PlatformDrawableViewLayerWin.hpp"
#include "Futurewalker.Application.PlatformDrawableViewLayerControl.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class PlatformDrawableViewLayerControlWin : public PlatformDrawableViewLayerControl
{
public:
    PlatformDrawableViewLayerControlWin(Shared<PlatformDrawableViewLayerWin> owner);

    auto Draw(Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& offset) -> Bool override;

private:
    auto GetLayer() -> Shared<PlatformDrawableViewLayerWin>;

private:
    Weak<PlatformDrawableViewLayerWin> _owner;
};
}
}
