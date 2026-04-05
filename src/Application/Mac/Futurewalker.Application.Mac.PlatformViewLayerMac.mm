// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformViewLayerMac.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto PlatformViewLayerMac::Make() -> Shared<PlatformViewLayerMac>
{
    return PlatformViewLayer::MakeDerived<PlatformViewLayerMac>();
}

PlatformViewLayerMac::PlatformViewLayerMac(PassKey<PlatformViewLayer> key)
  : PlatformViewLayer(key)
{
}

auto PlatformViewLayerMac::Initialize() -> void
{
}

auto PlatformViewLayerMac::OnAttach() -> void
{
}

auto PlatformViewLayerMac::OnDetach() -> void
{
}
}
