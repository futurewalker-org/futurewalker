// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Win.PlatformFilesystemWin.hpp"


namespace FW_DETAIL_NS
{
auto PlatformFilesystemWin::GetCurrentDirectoryPath() -> Path
{
    return {}; // FIXME
}

auto PlatformFilesystemWin::GetCurrentExecutablePath() -> Path
{
    return {}; // FIXME
}

auto Locator::Resolver<PlatformFilesystemWin>::Resolve() -> Shared<PlatformFilesystemWin>
{
    return Shared<PlatformFilesystemWin>::Make();
}
}
