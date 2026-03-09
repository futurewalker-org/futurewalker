// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.PathData.hpp"
#include "Futurewalker.Graphics.SkiaPathData.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
auto PathData::Make() -> Shared<PathData>
{
    return Shared<SkiaPathData>::Make();
}

PathData::~PathData() = default;
}
