// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TapGestureRecognizerType.hpp"
#include "Futurewalker.Application.GestureRecognizer.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief 
///
class HoverGestureRecognizer final : public GestureRecognizer
{
public:
    auto Recognize(const Delegate& delegate, const Event<PointerEvent>& event, const Rect<Dp>& area) -> Bool override;
};
}
}
