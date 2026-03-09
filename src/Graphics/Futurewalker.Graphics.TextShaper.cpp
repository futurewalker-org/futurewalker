// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.TextShaper.hpp"
#include "Futurewalker.Graphics.SkiaTextShaper.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief Make TextShaper.
///
auto TextShaper::Make() -> Shared<TextShaper>
{
    return Shared<SkiaTextShaper>::Make();
}

///
/// @brief Destructor.
///
TextShaper::~TextShaper() = default;
}
