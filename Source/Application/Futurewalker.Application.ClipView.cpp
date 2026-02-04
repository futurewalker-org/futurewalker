// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ClipView.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

namespace FW_DETAIL_NS
{
auto ClipView::Make() -> Shared<ClipView>
{
    return View::MakeDerived<ClipView>();
}

auto ClipView::MakeWithContent(Shared<View> const& content) -> Shared<ClipView>
{
    auto view = Make();
    view->SetContent(content);
    return view;
}

ClipView::ClipView(PassKey<View> key)
  : View(key)
{
}

auto ClipView::GetContent() -> Shared<View>
{
    return GetChildAt(0);
}

auto ClipView::GetContent() const -> Shared<View const>
{
    return GetChildAt(0);
}

auto ClipView::SetContent(Shared<View> const& content) -> void
{
    auto const child = GetChildAt(0);
    if (child != content)
    {
        if (child)
        {
            child->RemoveFromParent();
        }
        AddChildBack(content);
    }
}

auto ClipView::SetCornerRadius(AttributeArg<CornerRadius> radius) -> void
{
    _cornerRadius.SetAttributeArg(radius);
}

auto ClipView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(CornerRadius, AttributeCornerRadius, {});
    _cornerRadius.BindAndConnectAttributeWithDefaultValue(*this, &ClipView::ReceiveAttributeEvent, AttributeCornerRadius, {});
}

auto ClipView::Draw(DrawScope& scope) -> void
{
    auto const cornerRadius = _cornerRadius.GetValueOrDefault();

    if (cornerRadius != CornerRadius())
    {
        auto path = Graphics::Path();
        path.AddRoundRect(cornerRadius.GetRoundRect(GetContentRect(), GetLayoutDirection()));
        scope.SetClipPath(path);
    }
    else
    {
        scope.SetClipMode(ViewClipMode::Bounds);
    }
}

auto ClipView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent>())
    {
        InvalidateVisual();
        co_return true;
    }
    co_return false;
}
}
