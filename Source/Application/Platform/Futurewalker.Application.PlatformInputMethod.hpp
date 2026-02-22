// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformInputMethodType.hpp"
#include "Futurewalker.Application.PlatformInputEditable.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Range.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform input context.
///
class PlatformInputMethod : NonCopyable
{
public:
    virtual ~PlatformInputMethod() = 0;
    virtual auto SetEditable(Shared<PlatformInputEditable> const& editable) -> void = 0;
};
}
}
