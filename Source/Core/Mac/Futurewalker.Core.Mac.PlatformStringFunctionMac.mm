// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Core.Mac.PlatformStringFunctionMac.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Convert NSString to String.
///
auto PlatformStringFunctionMac::ConvertNSStringToString(NSString* str) -> String
{
    if (str)
    {
        return StringFunction::ConvertUtf8ToStringUnchecked([str UTF8String]);
    }
    return {};
}

///
/// @brief Convert UTF-8 string to NSString.
///
__attribute__((ns_returns_retained))
auto PlatformStringFunctionMac::ConvertStringToNSString(StringView sv) -> NSString*
{
    @autoreleasepool
    {
        auto const utf8Str = StringFunction::ConvertStringToStdString(sv);
        return [NSString stringWithUTF8String:utf8Str.c_str()];
    }
}
}
