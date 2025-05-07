// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Platform.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformScreen;
using PlatformScreenArray = std::vector<Shared<PlatformScreen>>;
}
}
