// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Mac.PlatformLocaleContextMac.hpp"

#include "Futurewalker.Core.Mac.PlatformStringFunctionMac.hpp"

namespace FW_DETAIL_NS
{
auto PlatformLocaleContextMac::GetPreferredUILocaleStrings() -> std::vector<String>
{
    @autoreleasepool
    {
        auto preferredLanguages = [NSLocale preferredLanguages];
        auto result = std::vector<String>();
        result.reserve(preferredLanguages.count);
        for (NSString* language in preferredLanguages)
        {
            result.push_back(PlatformStringFunctionMac::ConvertNSStringToString(language));
        }
        return result;
    }
}

auto Locator::Resolver<PlatformLocaleContextMac>::Resolve() -> Shared<PlatformLocaleContextMac>
{
    return Shared<PlatformLocaleContextMac>::Make();
}
}
