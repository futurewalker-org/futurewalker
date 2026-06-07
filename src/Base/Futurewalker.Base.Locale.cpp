// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Locale.hpp"
#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.PlatformLocaleContext.hpp"
#include "Futurewalker.Base.PlatformLocale.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.StringFunction.hpp"
#include "Futurewalker.Core.Exception.hpp"

namespace FW_DETAIL_NS
{
auto Locale::GetPreferredUILocales() -> std::vector<Locale>
{
    if (auto const context = Locator::GetInstance<PlatformLocaleContext>())
    {
        auto const platformLocales = context->GetPreferredUILocales();

        auto locales = std::vector<Locale>();
        locales.reserve(platformLocales.size());

        for (auto const& platformLocale : platformLocales)
        {
            locales.emplace_back(platformLocale);
        }
        return locales;
    }
    return {};
}

auto Locale::GetSystemLocale() -> Locale
{
    if (auto const context = Locator::GetInstance<PlatformLocaleContext>())
    {
        if (auto platformLocale = context->GetSystemLocale())
        {
            return Locale(std::move(platformLocale));
        }
    }
    FW_DEBUG_ASSERT(false);
    return Locale(nullptr);
}

auto Locale::Make(String const& language, String const& country, String const& variant) -> Optional<Locale>
{
    if (auto const context = Locator::GetInstance<PlatformLocaleContext>())
    {
        if (auto platformLocale = context->MakeLocale(language, country, variant))
        {
            return Locale(std::move(platformLocale));
        }
    }
    return {};
}

auto Locale::MakeFromLanguageTag(String const& languageTag) -> Optional<Locale>
{
    if (auto const context = Locator::GetInstance<PlatformLocaleContext>())
    {
        if (auto platformLocale = context->MakeLocaleFromLanguageTag(languageTag))
        {
            return Locale(std::move(platformLocale));
        }
    }
    return {};
}

Locale::Locale(Shared<PlatformLocale> platformLocale)
  : _platform(std::move(platformLocale))
{
}

Locale::Locale(Locale const& other)
  : _platform(other._platform)
{
}

auto Locale::operator=(Locale const& other) -> Locale&
{
    if (this != &other)
    {
        _platform = other._platform;
    }
    return *this;
}

auto Locale::operator==(Locale const& other) const -> bool
{
    if (_platform == other._platform)
    {
        return true;
    }

    if (_platform && other._platform)
    {
        return static_cast<bool>(_platform->Equals(*other._platform));
    }
    return false;
}

auto Locale::GetLanguageTag() const -> String
{
    if (_platform)
    {
        return _platform->GetLanguageTag();
    }
    return {};
}

auto Locale::GetLanguage() const -> String
{
    if (_platform)
    {
        return _platform->GetLanguage();
    }
    return {};
}

auto Locale::GetScript() const -> String
{
    if (_platform)
    {
        return _platform->GetScript();
    }
    return {};
}

auto Locale::GetCountry() const -> String
{
    if (_platform)
    {
        return _platform->GetCountry();
    }
    return {};
}

auto Locale::GetPlatformLocale() const -> Shared<PlatformLocale> const&
{
    return _platform;
}
}

auto std::hash<FW_NS::Locale>::operator()(FW_NS::Locale const& loc) const noexcept -> size_t
{
    if (auto const& platform = loc.GetPlatformLocale())
    {
        return static_cast<size_t>(platform->GetHash());
    }
    return 0u;
}

auto boost::hash<FW_NS::Locale>::operator()(FW_NS::Locale const& loc) const noexcept -> size_t
{
    if (auto const& platform = loc.GetPlatformLocale())
    {
        return static_cast<size_t>(platform->GetHash());
    }
    return 0u;
}
