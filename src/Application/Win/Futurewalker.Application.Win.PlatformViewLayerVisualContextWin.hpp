// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformViewLayerVisualContextWinType.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualContext.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayerVisualContextWin : public PlatformViewLayerVisualContext
{
public:
    explicit PlatformViewLayerVisualContextWin(Shared<PlatformDCompositionDeviceWin> const& device);
    auto CreateVisual() -> Shared<PlatformViewLayerVisual> override;

private:
    Shared<PlatformDCompositionDeviceWin> _device;
};
}
}
