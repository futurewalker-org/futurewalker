// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.HoverGestureViewType.hpp"
#include "Futurewalker.Application.HoverGestureViewEvent.hpp"
#include "Futurewalker.Application.HoverGestureRecognizerType.hpp"
#include "Futurewalker.Application.GestureRecognizerViewType.hpp"
#include "Futurewalker.Application.View.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class HoverGestureView : public View
{
public:
    static auto Make() -> Shared<HoverGestureView>;
    static auto MakeWithContent(const Shared<View>& contentView) -> Shared<HoverGestureView>;

    HoverGestureView(PassKey<View> key);

    auto GetContent() -> Shared<View>;
    auto SetContent(const Shared<View>& view) -> void;

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;

private:
    Shared<HoverGestureRecognizer> _gestureRecognizer;
    Shared<GestureRecognizerView> _gestureRecognizerView;
};
}
}
