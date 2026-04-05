// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformViewLayerMacType.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief macOS Implementation of PlatformViewLayer.
///
class PlatformViewLayerMac : public PlatformViewLayer
{
public:
    static auto Make() -> Shared<PlatformViewLayerMac>;

    PlatformViewLayerMac(PassKey<PlatformViewLayer>);

protected:
    auto Initialize() -> void override;
    auto OnAttach() -> void override;
    auto OnDetach() -> void override;
};
}
}
