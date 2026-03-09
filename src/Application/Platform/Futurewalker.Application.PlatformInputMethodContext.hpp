// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformInputMethodContextType.hpp"
#include "Futurewalker.Application.PlatformInputEditable.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformInputMethodContext : NonCopyable
{
public:
    virtual ~PlatformInputMethodContext() = 0;
    virtual auto MakeEditable(PlatformInputEditable::Delegate const& delegate) -> Shared<PlatformInputEditable> = 0;
};
}
}
