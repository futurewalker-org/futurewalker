// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformViewLayerContextWinType.hpp"
#include "Futurewalker.Application.PlatformViewLayerContext.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayerContextWin : public PlatformViewLayerContext
{
public:
    static auto Make(Shared<PlatformDCompositionDeviceWin> const& dcompDevice) -> Shared<PlatformViewLayerContextWin>;

    PlatformViewLayerContextWin(PassKey<PlatformViewLayerContextWin>, Shared<PlatformDCompositionDeviceWin> const& dcompDevice);

    auto GetNextId() -> PlatformViewLayerId override;
    auto MakeViewLayer() -> Shared<PlatformViewLayer> override;
    auto Commit() -> void override;

private:
    auto GetSelf() -> Shared<PlatformViewLayerContextWin>;

private:
    PlatformViewLayerId _nextLayerId = PlatformViewLayerId(0U);
    Weak<PlatformViewLayerContextWin> _self;
    Shared<PlatformDCompositionDeviceWin> _dcompDevice;
};

template <>
struct Locator::Resolver<PlatformViewLayerContextWin>
{
    using Interface = PlatformViewLayerContext;
    static auto Resolve() -> Shared<PlatformViewLayerContextWin>;
};
}
}
