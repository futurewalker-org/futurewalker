// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.WindowFrameViewType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.ViewAreaManagerType.hpp"
#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.EdgeInsets.hpp"

#include "Futurewalker.Attribute.AttributeObserverType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class WindowFrameView : public View
{
public:
    static auto Make() -> Shared<WindowFrameView>;

    WindowFrameView(PassKey<View> key);

    auto GetBackground() -> Shared<View>;
    auto SetBackground(Shared<View> background) -> void;

    auto GetContentBackground() -> Shared<View>;
    auto SetContentBackground(Shared<View> background) -> void;

    auto GetTitleBackground() -> Shared<View>;
    auto SetTitleBackground(Shared<View> background) -> void;

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> content) -> void;

    auto GetTitleContent() -> Shared<View>;
    auto SetTitleContent(Shared<View> content) -> void;

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Arrange(ArrangeScope& scope) -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;

private:
    auto GetAreaManager() -> Shared<ViewAreaManager>;
    auto GetAreaManager() const -> Shared<const ViewAreaManager>;
    auto GetTitleBarInsets() const -> EdgeInsets;
    auto GetTitleBarBoundingRects() const -> std::vector<Rect<Dp>>;
    auto GetTitleBarContentRect(Size<Dp> const& size) const -> Rect<Dp>;
    auto UpdateAreaManager() -> void;

private:
    Unique<AttributeObserver<Shared<ViewAreaManager>>> _areaManager;
    Shared<ContainerView> _background;
    Shared<ContainerView> _titleBackground;
    Shared<ContainerView> _contentBackground;
    Shared<ContainerView> _titleContent;
    Shared<ContainerView> _content;
    ScopedSignalConnection _connection;
};
}
}
