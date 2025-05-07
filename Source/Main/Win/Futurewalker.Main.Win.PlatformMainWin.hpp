// SPDX-License-Identifier: MIT

#include "Futurewalker.Main.Win.Prelude.hpp"
#include "Futurewalker.Main.ExitCode.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformMainWin : NonConstructible
{
public:
    static auto Register(HINSTANCE instanceHandle) -> void;
    static auto Main() -> ExitCode;
};
}
}
