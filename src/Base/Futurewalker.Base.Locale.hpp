// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.LocaleType.hpp"
#include "Futurewalker.Base.PlatformLocaleType.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include <boost/functional/hash.hpp>

#include <memory>
#include <functional>
#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT 
{
class Locale
{
public:
    static auto GetPreferredUILocales() -> std::vector<Locale>;
    static auto GetSystemLocale() -> Locale;

public:
    static auto Make(String const& language, String const& country, String const& variant) -> Optional<Locale>;
    static auto MakeFromLanguageTag(String const& languageTag) -> Optional<Locale>;

    explicit Locale(Shared<PlatformLocale> platformLocale);
    Locale(Locale const& other);
    auto operator=(Locale const& other) -> Locale&;

    auto operator==(Locale const& other) const -> bool;
    auto operator!=(Locale const& other) const -> bool = default;

    auto GetLanguageTag() const -> String;
    auto GetLanguage() const -> String;
    auto GetScript() const -> String;
    auto GetCountry() const -> String;

    auto GetPlatformLocale() const -> Shared<PlatformLocale> const&;

private:
    Shared<PlatformLocale> _platform;
};
}
}

template <>
struct std::hash<FW_NS::Locale>
{
    auto operator()(FW_NS::Locale const& loc) const noexcept -> size_t;
};

template <>
struct boost::hash<FW_NS::Locale>
{
    auto operator()(FW_NS::Locale const& loc) const noexcept -> size_t;
};
