// SPDX-License-Identifier: MIT

#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Convert wide string to UTF-8 string.
///
auto PlatformStringFunctionWin::WideToUtf8(std::wstring_view sv) -> String
{
    auto const len = ::WideCharToMultiByte(CP_UTF8, 0, sv.data(), static_cast<int>(sv.length()), NULL, 0, NULL, NULL);
    if (len == 0)
    {
        return {};
    }

    auto ret = std::string();
    ret.resize(len);

    if (::WideCharToMultiByte(CP_UTF8, 0, sv.data(), static_cast<int>(sv.length()), reinterpret_cast<char*>(ret.data()), static_cast<int>(ret.length()), NULL, NULL) != 0)
    {
        return StringFunction::ConvertUtf8ToStringUnchecked(ret);
    }
    return {};
}

///
/// @brief Convert UTF-8 string to wide string.
///
auto PlatformStringFunctionWin::Utf8ToWide(StringView sv) -> std::wstring
{
    if (sv.IsEmpty())
    {
        return {};
    }

    auto const len = ::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(static_cast<char8_t const*>(sv.GetData())), static_cast<int>(sv.GetSize()), NULL, 0);
    if (len == 0)
    {
        return {};
    }

    auto ret = std::wstring();
    ret.resize(len);

    if (::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(static_cast<char8_t const*>(sv.GetData())), static_cast<int>(sv.GetSize()), ret.data(), static_cast<int>(ret.length())) != 0)
    {
        return ret;
    }
    return {};
}
}
