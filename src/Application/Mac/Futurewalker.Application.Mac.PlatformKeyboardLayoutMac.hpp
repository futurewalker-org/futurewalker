// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.Prelude.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.String.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
struct PlatformKeyboardLayoutMac : NonConstructible
{
    static auto MapControlCharsToNamedKey(NSString* chars) -> String;
};
}
}
