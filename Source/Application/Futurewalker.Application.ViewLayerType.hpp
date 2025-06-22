// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Identifier.hpp"
#include "Futurewalker.Core.TypeTag.hpp"

#include <list>
#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ViewLayer;
using ViewLayerList = std::list<Shared<ViewLayer>>;
using ViewLayerArray = std::vector<Shared<ViewLayer>>;
using ViewLayerKind = Identifier<UInt64, TypeTag<ViewLayer, 0>>;
using ViewLayerId = Identifier<UInt64, TypeTag<ViewLayer, 1>>;
using UniqueViewLayerKind = UniqueIdentifier<ViewLayerKind::TagType>;
using UniqueViewLayerId = UniqueIdentifier<ViewLayerId::TagType>;
}
}
