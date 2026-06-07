// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.PlatformLocaleContext.hpp"
#include "Futurewalker.Base.PlatformLocale.hpp" 

#include <unicode/locid.h>
#include <unicode/localematcher.h>

namespace FW_DETAIL_NS
{
namespace
{
class PlatformLocaleIcu final : public PlatformLocale
{
public:
    explicit PlatformLocaleIcu(icu::Locale locale)
      : _locale(std::move(locale))
    {
    }

    auto GetLocale() const -> icu::Locale const&
    {
        return _locale;
    }

private:
    auto GetLanguageTag() const -> String override
    {
        auto err = U_ZERO_ERROR;
        auto const bcp47Tag = _locale.toLanguageTag<std::string>(err);
        if (U_SUCCESS(err))
        {
            return String::MakeFromStdString(bcp47Tag);
        }
        return {};
    }

    auto GetLanguage() const -> String override
    {
        return String::MakeFromStdString(_locale.getLanguage());
    }

    auto GetScript() const -> String override
    {
        return String::MakeFromStdString(_locale.getScript());
    }

    auto GetCountry() const -> String override
    {
        return String::MakeFromStdString(_locale.getCountry());
    }

    auto Equals(PlatformLocale const& other) const -> Bool override
    {
        if (auto const otherIcu = dynamic_cast<PlatformLocaleIcu const*>(&other))
        {
            return _locale == otherIcu->_locale;
        }
        return false;
    }

    auto GetHash() const -> UInt32 override
    {
        return std::bit_cast<uint32_t>(_locale.hashCode());
    }

private:
    icu::Locale _locale;
};
}

PlatformLocaleContext::~PlatformLocaleContext() = default;

auto PlatformLocaleContext::MakeLocale(String const& language, String const& country, String const& variant) -> Shared<PlatformLocale>
{
    auto locale = icu::Locale(language.ToStdString().c_str(), country.ToStdString().c_str(), variant.ToStdString().c_str());
    if (!locale.isBogus())
    {
        return Shared<PlatformLocaleIcu>::Make(std::move(locale));
    }
    return {};
}

auto PlatformLocaleContext::MakeLocaleFromLanguageTag(String const& languageTag) -> Shared<PlatformLocale>
{
    auto err = U_ZERO_ERROR;
    auto locale = icu::Locale::forLanguageTag(languageTag.ToStdString(), err);
    if (U_SUCCESS(err))
    {
        return Shared<PlatformLocaleIcu>::Make(std::move(locale));
    }
    return {};
}

auto PlatformLocaleContext::GetSystemLocale() -> Shared<PlatformLocale>
{
    auto locale = icu::Locale::getDefault();
    if (!locale.isBogus())
    {
        return Shared<PlatformLocaleIcu>::Make(std::move(locale));
    }
    return {};
}

auto PlatformLocaleContext::GetPreferredUILocales() -> std::vector<Shared<PlatformLocale>>
{
    auto const localeStrings = GetPreferredUILocaleStrings();

    auto result = std::vector<Shared<PlatformLocale>>();
    result.reserve(localeStrings.size());

    for (auto const& localeString : localeStrings)
    {
        if (auto locale = MakeLocaleFromLanguageTag(localeString))
        {
            result.push_back(std::move(locale));
        }
    }
    return result;
}

auto PlatformLocaleContext::Match(std::vector<Shared<PlatformLocale>> const& availableLocales, std::vector<Shared<PlatformLocale>> const& preferredLocales) -> Shared<PlatformLocale>
{
    auto builder = icu::LocaleMatcher::Builder();

    for (auto const& availableLocale : availableLocales)
    {
        if (auto const platformLocale = availableLocale.TryAs<PlatformLocaleIcu>())
        {
            builder.addSupportedLocale(platformLocale->GetLocale());
        }
    }

    auto error = U_ZERO_ERROR;
    auto const matcher = builder.build(error);
    if (U_FAILURE(error))
    {
        return {};
    }

    // TODO: Maybe use icu::Locale::ConvertingIterator?
    auto icuLocales = std::vector<icu::Locale>();
    icuLocales.reserve(preferredLocales.size());

    for (auto const& preferredLocale : preferredLocales)
    {
        if (auto const platformLocale = preferredLocale.TryAs<PlatformLocaleIcu>())
        {
            icuLocales.push_back(platformLocale->GetLocale());
        }
    }

    auto iterator = icu::Locale::RangeIterator(icuLocales.begin(), icuLocales.end());
    auto const result = matcher.getBestMatch(iterator, error);
    if (U_SUCCESS(error) && result && !result->isBogus())
    {
        return Shared<PlatformLocaleIcu>::Make(*result);
    }
    return {};
}
}
