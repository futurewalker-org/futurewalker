// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Mac.PlatformDebugMac.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#import <AppKit/AppKit.h>

namespace FW_DETAIL_NS
{
///
/// @brief Print message to debug console.
///
/// @param message Message to print.
///
auto PlatformDebugMac::Print(StringView const message) -> void
{
    auto const utf8Str = StringFunction::ConvertStringToStdString(message);
    auto const nsStr = [NSString stringWithUTF8String: utf8Str.c_str()];
    ::NSLog(@"%@", nsStr);
}

///
/// @brief Break program execution.
///
auto PlatformDebugMac::Break() -> void
{
    __builtin_trap();
}

auto Locator::Resolver<PlatformDebugMac>::Resolve() -> Shared<PlatformDebugMac>
{
    return Shared<PlatformDebugMac>::Make();
}
}
