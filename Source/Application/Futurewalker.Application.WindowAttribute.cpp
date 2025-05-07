// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.WindowAttribute.hpp"

namespace FW_DETAIL_NS
{
StaticAttribute<Shared<WindowAreaManager>> const WindowAttribute::AreaManager = StaticAttribute<Shared<WindowAreaManager>>::MakeWithDefaultValue(nullptr);
}
