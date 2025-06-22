// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.InputMethodType.hpp"
#include "Futurewalker.Application.InputMethodEditableType.hpp"
#include "Futurewalker.Application.PlatformInputMethodType.hpp"

#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Range.hpp"
#include "Futurewalker.Core.String.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Input method.
///
/// Interface for communicating with text input system.
///
class InputMethod : NonCopyable
{
public:
    static auto Make(Shared<PlatformInputMethod> const& platform) -> Shared<InputMethod>;

    InputMethod(PassKey<InputMethod>, Shared<PlatformInputMethod> const& platform);

    auto SetEditable(Shared<InputMethodEditable> const& editable) -> void;

private:
    Shared<PlatformInputMethod> _platform;
};
}
}
