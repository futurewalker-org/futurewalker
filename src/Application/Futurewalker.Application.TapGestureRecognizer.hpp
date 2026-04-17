// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TapGestureRecognizerType.hpp"
#include "Futurewalker.Application.GestureRecognizer.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp"
#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.Flags.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

#include <set>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Recognizer for tap gestures.
///
class TapGestureRecognizer final : public GestureRecognizer
{
public:
    auto Start(Delegate const& delegate) -> void override;
    auto Stop() -> void override;
    auto Recognize(const Event<PointerEvent>& event, const Rect<Dp>& area) -> Bool override;

    auto SetAllowedButtons(Flags<PointerButtonFlag> const buttons) -> void;
    auto SetAllowedActions(Flags<TapGestureActionFlag> const actions) -> void;

private:
    auto InternalRecognize() -> Async<void>;
    auto InternalRecognizeFirstDown() -> Async<Bool>;
    auto InternalRecognizeFirstUp() -> Async<Bool>;

private:
    Flags<PointerButtonFlag> _allowedButtons = PointerButtonFlag::Button1;
    Flags<TapGestureActionFlag> _allowedActions = TapGestureActionFlag::SingleTap;
    Delegate _delegate;
    Optional<TaskHandle<void>> _taskHandle;
};
}
}
