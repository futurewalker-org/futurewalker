// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.Prelude.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class SystemInfo;

///
/// @brief Kind of the current platform.
///
enum class PlatformKind
{
    Unknown = 0,
    Windows,
    MacOS,
};

///
/// @brief System version information.
///
struct SystemVersion
{
    SInt32 major;
    SInt32 minor;
    SInt32 patch;
};
}
}
