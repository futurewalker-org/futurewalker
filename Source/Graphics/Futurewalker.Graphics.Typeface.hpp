// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.TypefaceType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
class Typeface : NonCopyable
{
public:
    virtual ~Typeface() = 0;
};
}
}
