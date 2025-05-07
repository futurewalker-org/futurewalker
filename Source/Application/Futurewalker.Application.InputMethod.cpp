// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.InputMethod.hpp"
#include "Futurewalker.Application.InputMethodEditable.hpp"
#include "Futurewalker.Application.PlatformInputMethod.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make instance. 
///
auto InputMethod::Make(Shared<PlatformInputMethod> const& platform) -> Shared<InputMethod>
{
    return Shared<InputMethod>::Make(PassKey<InputMethod>(), platform);
}

///
/// @brief
///
InputMethod::InputMethod(PassKey<InputMethod>, Shared<PlatformInputMethod> const& platform)
  : _platform {platform}
{
}

///
/// @brief
///
auto InputMethod::SetEditable(Shared<InputMethodEditable> const& editable) -> void
{
    if (editable)
    {
        editable->Attach({}, _platform);
    }
}
}
