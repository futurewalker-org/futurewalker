// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.PlatformFilesystemType.hpp"
#include "Futurewalker.Base.Path.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformFilesystem : NonCopyable
{
public:
    virtual ~PlatformFilesystem() = 0;
    virtual auto GetCurrentDirectoryPath() -> Path = 0;
    virtual auto GetCurrentExecutablePath() -> Path = 0;
};
}
}
