// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Win.PlatformFilesystemWin.hpp"

#include <boost/dll/runtime_symbol_info.hpp>

namespace FW_DETAIL_NS
{
auto PlatformFilesystemWin::GetCurrentDirectoryPath() -> Path
{
    try
    {
        auto const path = std::filesystem::current_path();
        return Path::MakeFromNativeString(path.native());
    }
    catch (...)
    {
        throw Exception(ErrorCode::Failure);
    }
}

auto PlatformFilesystemWin::GetCurrentExecutablePath() -> Path
{
    try
    {
        auto const path = boost::dll::program_location();
        return Path::MakeFromNativeString(path.native());
    }
    catch (...)
    {
        throw Exception(ErrorCode::Failure);
    }
}

auto Locator::Resolver<PlatformFilesystemWin>::Resolve() -> Shared<PlatformFilesystemWin>
{
    return Shared<PlatformFilesystemWin>::Make();
}
}
