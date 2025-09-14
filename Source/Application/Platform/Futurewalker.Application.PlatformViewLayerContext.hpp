// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayerContextType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayerContext : NonCopyable
{
public:
    virtual ~PlatformViewLayerContext() = 0;
    virtual auto MakeViewLayer() -> Shared<PlatformViewLayer> = 0;
    virtual auto CommitChanges() -> void = 0;
};
}
}
