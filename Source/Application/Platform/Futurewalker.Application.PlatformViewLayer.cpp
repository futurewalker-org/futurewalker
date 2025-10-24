// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformViewLayer.hpp"
#include "Futurewalker.Application.PlatformViewLayerContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
PlatformViewLayer::PlatformViewLayer(PassKey<PlatformViewLayer>)
{
}

///
/// @brief
///
PlatformViewLayer::~PlatformViewLayer() = default;

///
/// @brief Get unique identifier of this layer.
///
auto PlatformViewLayer::GetId() const -> PlatformViewLayerId
{
    return _id;
}

///
/// @brief
///
auto PlatformViewLayer::Initialize() -> void
{
}

///
/// @brief
///
auto PlatformViewLayer::InitializeSelf() -> void
{
    _context = Locator::Resolve<PlatformViewLayerContext>();
    _id = _context->GetNextId();
}
}
