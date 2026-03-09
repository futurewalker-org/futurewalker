// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayerVisualContextType.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayerVisualContext : NonCopyable
{
public:
    virtual ~PlatformViewLayerVisualContext() = 0;
    virtual auto CreateVisual() -> Shared<PlatformViewLayerVisual> = 0;
};
}
}
