// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Mac.Prelude.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.String.hpp"

#import <Foundation/Foundation.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform specific string utilities. 
///
class PlatformStringFunctionMac : NonConstructible
{
public:
    static auto ConvertNSStringToString(NSString* str) -> String;

    __attribute__((ns_returns_retained)) 
    static auto ConvertStringToNSString(StringView sv) -> NSString*;
};
}
}
