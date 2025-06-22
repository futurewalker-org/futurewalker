// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Win.Prelude.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.String.hpp"

#include <string>

#include <windows.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform specific string utilities. 
///
class PlatformStringFunctionWin : NonConstructible
{
public:
    static auto WideToUtf8(std::wstring_view sv) -> String;
    static auto Utf8ToWide(StringView sv) -> std::wstring;
};
}
}
