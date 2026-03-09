// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.PlatformSkiaFontManagerType.hpp"
#include "Futurewalker.Graphics.PlatformFontManager.hpp"

#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class PlatformSkiaFontManager : public PlatformFontManager
{
public:
    virtual auto GetSkFontMgr() -> sk_sp<SkFontMgr> = 0;
};
}
}
