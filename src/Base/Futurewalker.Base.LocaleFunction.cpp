// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.LocaleFunction.hpp"
#include "Futurewalker.Base.Locale.hpp"
#include "Futurewalker.Base.PlatformLocaleContext.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Find best matching locale.
///
/// @param[in] availableLocales List of available locales.
/// @param[in] preferredLocales List of preferred locales.
///
/// @return Best matching locale in available locales.
///
auto LocaleFunction::Match(std::span<Locale const> availableLocales, std::span<Locale const> preferredLocales) -> Optional<Locale>
{
    if (auto const context = Locator::GetInstance<PlatformLocaleContext>())
    {
        auto platformAvailableLocales = std::vector<Shared<PlatformLocale>>();
        platformAvailableLocales.reserve(availableLocales.size());

        for (auto& locale : availableLocales)
        {
            platformAvailableLocales.push_back(locale.GetPlatformLocale());
        }

        auto platformPreferredLocales = std::vector<Shared<PlatformLocale>>();
        platformPreferredLocales.reserve(preferredLocales.size());

        for (auto& locale : preferredLocales)
        {
            platformPreferredLocales.push_back(locale.GetPlatformLocale());
        }

        if (auto matchedPlatformLocale = context->Match(platformAvailableLocales, platformPreferredLocales))
        {
            return Locale(std::move(matchedPlatformLocale));
        }
    }
    return {};
}
}
