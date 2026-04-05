// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayerContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayerContextMac final : public PlatformViewLayerContext
{
public:
    auto GetNextId() -> PlatformViewLayerId override;
    auto MakeViewLayer() -> Shared<PlatformViewLayer> override;
    auto Commit() -> void override;

private:
    PlatformViewLayerId _nextLayerId = PlatformViewLayerId(0U);
};

template <>
struct Locator::Resolver<PlatformViewLayerContextMac>
{
    using Interface = PlatformViewLayerContext;
    static auto Resolve() -> Shared<PlatformViewLayerContextMac>;
};
}
}
