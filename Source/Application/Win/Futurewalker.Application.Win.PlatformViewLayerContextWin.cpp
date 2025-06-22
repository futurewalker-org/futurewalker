// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformViewLayerContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformDrawableViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWin.hpp"

namespace FW_DETAIL_NS
{
namespace
{
class ViewLayerWin : public PlatformViewLayerWin
{
    Shared<PlatformViewLayerContextWin> _context;

public:
    static auto Make(Shared<PlatformViewLayerContextWin> const& context, Shared<PlatformDCompositionDeviceWin> const& dcompDevice)
    {
        return PlatformViewLayer::MakeDerived<ViewLayerWin>(context, dcompDevice);
    }

    ViewLayerWin(PassKey<PlatformViewLayer> key, Shared<PlatformViewLayerContextWin> const& context, Shared<PlatformDCompositionDeviceWin> const& dcompDevice)
      : PlatformViewLayerWin(key, dcompDevice)
      , _context {context}
    {
    }

    auto GetControl() -> Shared<PlatformViewLayerControl> override
    {
        return nullptr;
    }

    auto CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3> override
    {
        if (const auto dcompDevice = GetDCompositionDevice())
        {
            return dcompDevice->CreateVisual();
        }
        return nullptr;
    }

    auto DestroyVisual(Microsoft::WRL::ComPtr<IDCompositionVisual3> visual) -> void override
    {
        (void)visual;
    }
};
}

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

auto PlatformViewLayerContextWin::MakeViewLayer() -> Shared<PlatformViewLayer>
{
    return ViewLayerWin::Make(GetSelf(), _dcompDevice);
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
