// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Debug.hpp"
#include "Futurewalker.Base.PlatformDebug.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Debug print.
///
auto Debug::PrintPriv(StringView const message) -> void
{
    if (auto const debug = Locator::GetInstance<PlatformDebug>())
    {
        debug->Print(message);
    }
}

///
/// @brief Debug break.
///
auto Debug::BreakPriv() -> void
{
    if (auto const debug = Locator::GetInstance<PlatformDebug>())
    {
        debug->Break();
    }
}
}
