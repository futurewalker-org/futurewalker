// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformDrawableViewLayerControlWin.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief 
///
/// @param owner 
///
PlatformDrawableViewLayerControlWin::PlatformDrawableViewLayerControlWin(Shared<PlatformDrawableViewLayerWin> owner)
  : _owner {owner}
{
}

///
/// @brief
///
/// @param displayList
///
auto PlatformDrawableViewLayerControlWin::Draw(Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& offset) -> Bool
{
    if (auto layer = GetLayer())
    {
        return layer->Draw(displayList, offset);
    }
    return false;
}

///
/// @brief
///
auto PlatformDrawableViewLayerControlWin::GetLayer() -> Shared<PlatformDrawableViewLayerWin>
{
    return _owner.Lock();
}
}
