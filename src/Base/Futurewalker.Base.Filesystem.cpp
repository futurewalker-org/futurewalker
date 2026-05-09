// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Filesystem.hpp"
#include "Futurewalker.Base.PlatformFilesystem.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.Exception.hpp"

namespace FW_DETAIL_NS
{
auto Filesystem::GetCurrentExecutablePath() -> Path
{
    if (auto const platform = Locator::GetInstance<PlatformFilesystem>())
    {
        return platform->GetCurrentExecutablePath();
    }
    throw Exception(ErrorCode::Failure);
}

auto Filesystem::GetCurrentDirectoryPath() -> Path
{
    if (auto const platform = Locator::GetInstance<PlatformFilesystem>())
    {
        return platform->GetCurrentDirectoryPath();
    }
    throw Exception(ErrorCode::Failure);
}
}
