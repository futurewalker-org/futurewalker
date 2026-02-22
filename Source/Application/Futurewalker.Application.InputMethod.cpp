// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.InputMethod.hpp"
#include "Futurewalker.Application.InputEditable.hpp"
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
auto InputMethod::SetEditable(Shared<InputEditable> const& editable) -> void
{
    if (_platform)
    {
        _platform->SetEditable(editable ? editable->GetPlatformObject({}) : nullptr);
    }
}
}
