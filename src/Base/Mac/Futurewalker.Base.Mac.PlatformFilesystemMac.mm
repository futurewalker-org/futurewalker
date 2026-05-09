// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Mac.PlatformFilesystemMac.hpp"

#include <boost/dll/runtime_symbol_info.hpp>

namespace FW_DETAIL_NS
{
///
/// @brief Get current working directory path.
///
auto PlatformFilesystemMac::GetCurrentDirectoryPath() -> Path
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

///
/// @brief Get current executable path.
///
auto PlatformFilesystemMac::GetCurrentExecutablePath() -> Path
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

auto Locator::Resolver<PlatformFilesystemMac>::Resolve() -> Shared<PlatformFilesystemMac>
{
    return Shared<PlatformFilesystemMac>::Make();
}
}
