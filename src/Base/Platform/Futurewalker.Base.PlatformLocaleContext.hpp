// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.PlatformLocaleContextType.hpp"
#include "Futurewalker.Base.PlatformLocaleType.hpp" 

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformLocaleContext : NonCopyable
{
public:
    virtual ~PlatformLocaleContext() = 0;
    virtual auto MakeLocale(String const& language, String const& country, String const& variant) -> Shared<PlatformLocale>;
    virtual auto MakeLocaleFromLanguageTag(String const& languageTag) -> Shared<PlatformLocale>;
    virtual auto GetSystemLocale() -> Shared<PlatformLocale>;
    virtual auto GetPreferredUILocales() -> std::vector<Shared<PlatformLocale>>;
    virtual auto Match(std::vector<Shared<PlatformLocale>> const& availableLocales, std::vector<Shared<PlatformLocale>> const& preferredLocales) -> Shared<PlatformLocale>;

private:
    virtual auto GetPreferredUILocaleStrings() -> std::vector<String> = 0;
};
}
}
