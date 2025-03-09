// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.ViewLayerType.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.HashMap.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ViewLayerManager;
using ViewLayerManagerMakeFunction = Function<Shared<ViewLayer>()>;
using ViewLayerManagerMakeFunctionMap = HashMap<ViewLayerKind, ViewLayerManagerMakeFunction>;

extern UniqueViewLayerKind ViewLayerKindNormal;   // A kind of plain ViewLayer instances.
extern UniqueViewLayerKind ViewLayerKindDrawable; // A kind of DrawableViewLayer instances.
}
}
