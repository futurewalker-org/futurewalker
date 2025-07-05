// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.GestureRecognizerViewType.hpp"
#include "Futurewalker.Application.GestureRecognizerType.hpp" 
#include "Futurewalker.Application.ContainerView.hpp"

#include "Futurewalker.Application.View.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief View that directs pointer events to GestureRecognizer.
///
class GestureRecognizerView : public View
{
public:
    static auto Make() -> Shared<GestureRecognizerView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<GestureRecognizerView>;

    GestureRecognizerView(PassKey<View> key);

    auto SetGestureRecognizer(Shared<GestureRecognizer> const& gestureRecognizer) -> void;
    auto GetGestureRecognizer() -> Shared<GestureRecognizer>;
    auto GetGestureRecognizer() const -> Shared<GestureRecognizer const>;

    auto SetContent(Shared<View> const& view) -> void;
    auto GetContent() -> Shared<View>;
    auto GetContent() const -> Shared<View const>;

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;

private:
    Shared<ContainerView> _containerView;
    Shared<GestureRecognizer> _gestureRecognizer;
};
}
}
