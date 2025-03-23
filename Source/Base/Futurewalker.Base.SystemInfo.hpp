// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.SystemInfoType.hpp"
#include "Futurewalker.Base.PlatformSystemInfoType.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief System information.
///
class SystemInfo : NonConstructible
{
public:
    static auto IsWindows() -> Bool;
    static auto IsUnix() -> Bool;
    static auto IsMacOS() -> Bool;
    static auto IsIOS() -> Bool;
    static auto IsAndroid() -> Bool;
};
}
}
