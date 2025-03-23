// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.PlatformSystemInfoType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief System information.
///
class PlatformSystemInfo : NonCopyable
{
public:
    virtual ~PlatformSystemInfo() = 0;
    virtual auto IsWindows() const -> Bool = 0;
    virtual auto IsUnix() const -> Bool = 0;
    virtual auto IsMacOS() const -> Bool = 0;
    virtual auto IsIOS() const -> Bool = 0;
    virtual auto IsAndroid() const -> Bool = 0;
};
}
}
