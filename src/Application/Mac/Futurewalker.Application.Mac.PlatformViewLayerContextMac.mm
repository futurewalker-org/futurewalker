// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformViewLayerContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformViewLayerMac.hpp"

namespace FW_DETAIL_NS
{
auto PlatformViewLayerContextMac::GetNextId() -> PlatformViewLayerId
{
    _nextLayerId = PlatformViewLayerId(static_cast<UInt64>(_nextLayerId) + 1U);
    return _nextLayerId;
}

auto PlatformViewLayerContextMac::MakeViewLayer() -> Shared<PlatformViewLayer>
{
    return PlatformViewLayerMac::Make();
}

auto PlatformViewLayerContextMac::Commit() -> void
{
    // TODO.
}

auto Locator::Resolver<PlatformViewLayerContextMac>::Resolve() -> Shared<PlatformViewLayerContextMac>
{
    return Shared<PlatformViewLayerContextMac>::Make();
}
}
