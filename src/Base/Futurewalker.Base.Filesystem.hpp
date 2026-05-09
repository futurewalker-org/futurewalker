// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.Path.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
struct Filesystem : NonConstructible
{
    static auto GetCurrentExecutablePath() -> Path;
    static auto GetCurrentDirectoryPath() -> Path;
};
}
}
