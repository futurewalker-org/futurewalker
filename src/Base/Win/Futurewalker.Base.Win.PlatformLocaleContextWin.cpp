// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Win.PlatformLocaleContextWin.hpp"

#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

namespace FW_DETAIL_NS
{
auto PlatformLocaleContextWin::GetPreferredUILocaleStrings() -> std::vector<String>
{
    auto numLanguages = ULONG(0);
    auto bufferSize = ULONG(0);

    if (!::GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &numLanguages, nullptr, &bufferSize))
    {
        return {};
    }

    auto languageBuffer = std::vector<wchar_t>(bufferSize);

    if (!::GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &numLanguages, languageBuffer.data(), &bufferSize))
    {
        return {};
    }

    auto locales = std::vector<std::wstring>();
    locales.reserve(numLanguages);

    // Parse the multi-string buffer (ends with a double null terminator).
    wchar_t* currentLanguage = languageBuffer.data();
    while (*currentLanguage != L'\0')
    {
        locales.push_back(currentLanguage);
        currentLanguage += ::wcslen(currentLanguage) + 1;
    }

    auto result = std::vector<String>();
    for (auto const& locale : locales)
    {
        result.push_back(PlatformStringFunctionWin::WideToUtf8(locale));
    }
    return result;
}

auto Locator::Resolver<PlatformLocaleContextWin>::Resolve() -> Shared<PlatformLocaleContextWin>
{
    return Shared<PlatformLocaleContextWin>::Make();
}
}
