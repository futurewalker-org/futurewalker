// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Win.PlatformDebugWin.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

#include <iostream>

namespace FW_DETAIL_NS
{
///
/// @brief Print message to debug console.
///
/// @param message Message to print.
///
auto PlatformDebugWin::Print(StringView const message) -> void
{
    std::wstring wideMessage = PlatformStringFunctionWin::Utf8ToWide(message);
    wideMessage += L'\n';

    if (::GetConsoleWindow())
    {
        std::wcerr << wideMessage;
    }
    else
    {
        ::OutputDebugStringW(wideMessage.c_str());
    }
}

///
/// @brief Break program execution.
///
auto PlatformDebugWin::Break() -> void
{
    ::DebugBreak();
}

auto Locator::Resolver<PlatformDebugWin>::Resolve() -> Shared<PlatformDebugWin>
{
    return Shared<PlatformDebugWin>::Make();
}
}
