// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaColorFilter.hpp"

#include <include/effects/SkColorMatrix.h>

namespace FW_GRAPHICS_DETAIL_NS
{
auto SkiaColorFilter::MakeMatrixFilter(Matrix5x5<Float64> const& matrix, Bool const clamp) -> Shared<SkiaColorFilter>
{
    SkScalar colorMatrix[20] {};
    colorMatrix[0] = static_cast<SkScalar>(matrix.m00);
    colorMatrix[1] = static_cast<SkScalar>(matrix.m01);
    colorMatrix[2] = static_cast<SkScalar>(matrix.m02);
    colorMatrix[3] = static_cast<SkScalar>(matrix.m03);
    colorMatrix[4] = static_cast<SkScalar>(matrix.m04);
    colorMatrix[5] = static_cast<SkScalar>(matrix.m10);
    colorMatrix[6] = static_cast<SkScalar>(matrix.m11);
    colorMatrix[7] = static_cast<SkScalar>(matrix.m12);
    colorMatrix[8] = static_cast<SkScalar>(matrix.m13);
    colorMatrix[9] = static_cast<SkScalar>(matrix.m14);
    colorMatrix[10] = static_cast<SkScalar>(matrix.m20);
    colorMatrix[11] = static_cast<SkScalar>(matrix.m21);
    colorMatrix[12] = static_cast<SkScalar>(matrix.m22);
    colorMatrix[13] = static_cast<SkScalar>(matrix.m23);
    colorMatrix[14] = static_cast<SkScalar>(matrix.m24);
    colorMatrix[15] = static_cast<SkScalar>(matrix.m30);
    colorMatrix[16] = static_cast<SkScalar>(matrix.m31);
    colorMatrix[17] = static_cast<SkScalar>(matrix.m32);
    colorMatrix[18] = static_cast<SkScalar>(matrix.m33);
    colorMatrix[19] = static_cast<SkScalar>(matrix.m34);
    auto skColorFilter = SkColorFilters::Matrix(colorMatrix, clamp ? SkColorFilters::Clamp::kYes : SkColorFilters::Clamp::kNo);
    return Shared<SkiaColorFilter>::Make(skColorFilter);
}

SkiaColorFilter::SkiaColorFilter(sk_sp<SkColorFilter> filter)
  : _filter(std::move(filter))
{
}

auto SkiaColorFilter::GetSkiaColorFilter() const -> sk_sp<SkColorFilter>
{
    return _filter;
}
}
