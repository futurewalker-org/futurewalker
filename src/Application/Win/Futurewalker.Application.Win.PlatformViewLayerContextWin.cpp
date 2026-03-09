// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformViewLayerContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp"

namespace FW_DETAIL_NS
{
auto PlatformViewLayerContextWin::Make(Shared<PlatformDCompositionDeviceWin> const& dcompDevice) -> Shared<PlatformViewLayerContextWin>
{
    auto context = Shared<PlatformViewLayerContextWin>::Make(PassKey<PlatformViewLayerContextWin>(), dcompDevice);
    context->_self = context;
    return context;
}

PlatformViewLayerContextWin::PlatformViewLayerContextWin(PassKey<PlatformViewLayerContextWin>, Shared<PlatformDCompositionDeviceWin> const& dcompDevice)
  : PlatformViewLayerContext()
  , _dcompDevice {dcompDevice}
{
}

auto PlatformViewLayerContextWin::GetNextId() -> PlatformViewLayerId
{
    _nextLayerId = PlatformViewLayerId(static_cast<UInt64>(_nextLayerId) + 1U);
    return _nextLayerId;
}

auto PlatformViewLayerContextWin::MakeViewLayer() -> Shared<PlatformViewLayer>
{
    return PlatformViewLayerWin::Make(_dcompDevice);
}

auto PlatformViewLayerContextWin::Commit() -> void
{
    if (_dcompDevice)
    {
        _dcompDevice->Commit();
    }
}

auto PlatformViewLayerContextWin::GetSelf() -> Shared<PlatformViewLayerContextWin>
{
    return _self.Lock();
}

auto Locator::Resolver<PlatformViewLayerContextWin>::Resolve() -> Shared<PlatformViewLayerContextWin>
{
    auto dcompDevice = Locator::Resolve<PlatformDCompositionDeviceWin>();
    return PlatformViewLayerContextWin::Make(dcompDevice);
}
}
