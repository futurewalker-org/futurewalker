// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaDisplayList.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief
///
SkiaDisplayList::SkiaDisplayList(const sk_sp<SkPicture>& picture)
  : _picture {picture}
{
}

///
/// @brief
///
auto SkiaDisplayList::GetBounds() const -> Rect<Dp>
{
    auto const rect = _picture->cullRect();
    return Rect<Dp>(rect.fLeft, rect.fTop, rect.fRight, rect.fBottom);
}

///
/// @brief
///
auto SkiaDisplayList::GetPicture() const -> sk_sp<SkPicture>
{
    return _picture;
}
}
