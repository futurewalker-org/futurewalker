// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.SkiaDisplayListType.hpp"
#include "Futurewalker.Graphics.DisplayList.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include <include/core/SkPicture.h>

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief
///
class SkiaDisplayList : public DisplayList
{
public:
    explicit SkiaDisplayList(const sk_sp<SkPicture>& picture);

    auto GetBounds() const -> Rect<Dp> override;

    auto GetPicture() const -> sk_sp<SkPicture>;

private:
    sk_sp<SkPicture> _picture;
};
}
