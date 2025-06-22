// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TapGestureViewType.hpp"
#include "Futurewalker.Application.TapGestureViewEvent.hpp"
#include "Futurewalker.Application.TapGestureRecognizerType.hpp"
#include "Futurewalker.Application.GestureRecognizerViewType.hpp"
#include "Futurewalker.Application.View.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TapGestureView : public View
{
public:
    static auto Make() -> Shared<TapGestureView>;
    static auto MakeWithContent(const Shared<TapGestureView>& contentView) -> Shared<TapGestureView>;

    TapGestureView(PassKey<View> key);

    auto GetContent() -> Shared<View>;
    auto SetContent(const Shared<View>& view) -> void;

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event& event) -> Async<Bool>;

private:
    Shared<TapGestureRecognizer> _gestureRecognizer;
    Shared<GestureRecognizerView> _gestureRecognizerView;
};
}
}
