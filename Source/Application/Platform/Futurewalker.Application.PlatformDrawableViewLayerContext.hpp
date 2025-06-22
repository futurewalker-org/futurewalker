// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformDrawableViewLayerContextType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Context object of platform view layers.
///
class PlatformDrawableViewLayerContext : NonCopyable
{
public:
    virtual ~PlatformDrawableViewLayerContext() = 0;

    ///
    /// @brief Make drawable view layer.
    ///
    virtual auto MakeDrawableLayer() -> Shared<PlatformViewLayer> = 0;
};
}
}
