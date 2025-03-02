// SPDX-License-Identifier: MIT

#include "Futurewalker.Base.Debug.hpp"
#include "Futurewalker.Base.PlatformDebug.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Debug print.
///
auto Debug::PrintPriv(const StringView message) -> void
{
    if (const auto debug = Locator::GetInstance<PlatformDebug>())
    {
        debug->Print(message);
    }
}

///
/// @brief Debug break.
///
auto Debug::BreakPriv() -> void
{
    if (const auto debug = Locator::GetInstance<PlatformDebug>())
    {
        debug->Break();
    }
}
}
