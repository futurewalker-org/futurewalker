// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformInputMethodMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformInputEditableMacType.hpp"
#include "Futurewalker.Application.PlatformInputMethod.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include <AppKit/AppKit.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform input context.
///
class PlatformInputMethodMac : public PlatformInputMethod
{
public:
    static auto Make(NSView* view) -> Shared<PlatformInputMethodMac>;

    explicit PlatformInputMethodMac(PassKey<PlatformInputMethodMac>, NSView* view);

    auto GetEditable() -> Shared<PlatformInputEditableMac>;
    auto SetEditable(Shared<PlatformInputEditable> const& editable) -> void override;

    auto GetTextInputClient() -> id<NSTextInputClient>;

private:
    __weak NSView* _view = nil;
    __weak NSTextInputContext* _inputContext = nil;
    __strong id<NSTextInputClient> _textInputClient = nil;
    Shared<PlatformInputEditableMac> _editable;
};
}
}
