// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.Mac.Prelude.hpp"
#include "Futurewalker.Base.PlatformFilesystem.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief macOS implementation of PlatformFilesystem.
///
class PlatformFilesystemMac : public PlatformFilesystem
{
public:
    auto GetCurrentDirectoryPath() -> Path override;
    auto GetCurrentExecutablePath() -> Path override;
};

template <>
struct Locator::Resolver<PlatformFilesystemMac>
{
    using Interface = PlatformFilesystem;
    static auto Resolve() -> Shared<PlatformFilesystemMac>;
};
}
}
