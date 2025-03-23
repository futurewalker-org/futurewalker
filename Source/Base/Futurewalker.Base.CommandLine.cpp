// SPDX-License-Identifier: MIT

#include "Futurewalker.Base.CommandLine.hpp"
#include "Futurewalker.Base.PlatformCommandLine.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Get parsed arguments.
///
auto CommandLine::GetArguments() -> std::vector<String>
{
    if (auto const platform = Locator::GetInstance<PlatformCommandLine>())
    {
        return platform->GetArguments();
    }
    FW_DEBUG_ASSERT(false);
    return {};
}
}
