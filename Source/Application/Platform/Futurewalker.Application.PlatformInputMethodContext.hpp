// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformInputMethodContextType.hpp"
#include "Futurewalker.Application.PlatformInputMethodEditable.hpp"

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
    virtual auto MakeEditable(PlatformInputMethodEditable::Delegate const& delegate) -> Shared<PlatformInputMethodEditable> = 0;
};
}
}
