// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformViewLayerVisualContextMacType.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualContext.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief macOS implementation of the view layer visual context.
///
class PlatformViewLayerVisualContextMac : public PlatformViewLayerVisualContext
{
public:
    static auto Make() -> Shared<PlatformViewLayerVisualContextMac>;
    auto CreateVisual() -> Shared<PlatformViewLayerVisual> override;
};
}
}
