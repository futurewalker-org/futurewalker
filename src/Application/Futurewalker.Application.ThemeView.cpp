// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ThemeView.hpp"
#include "Futurewalker.Application.Theme.hpp"

namespace FW_DETAIL_NS
{
auto ThemeView::Make() -> Shared<ThemeView>
{
    return View::MakeDerived<ThemeView>();
}

auto ThemeView::Make(Shared<Theme> const& theme) -> Shared<ThemeView>
{
    auto view = Make();
    view->SetTheme(theme);
    return view;
}

auto ThemeView::MakeWithContent(Shared<View> const& content) -> Shared<ThemeView>
{
    auto view = Make();
    view->SetContent(content);
    return view;
}

auto ThemeView::MakeWithContent(Shared<Theme> const& theme, Shared<View> const& content) -> Shared<ThemeView>
{
    auto view = Make();
    view->SetTheme(theme);
    view->SetContent(content);
    return view;
}

ThemeView::ThemeView(PassKey<View> key)
  : View(key)
{
}

auto ThemeView::GetContent() -> Shared<View>
{
    return GetChildAt(0);
}

auto ThemeView::SetContent(Shared<View> const& content) -> void
{
    auto const child = GetContent();
    if (child != content)
    {
        if (child)
        {
            child->RemoveFromParent();
        }
        AddChildBack(content);
    }
}

auto ThemeView::GetTheme() const -> Shared<Theme>
{
    return _theme;
}

auto ThemeView::SetTheme(Shared<Theme> const& theme) -> void
{
    if (_theme != theme)
    {
        _theme = theme;
        UpdateTheme();
    }
}

auto ThemeView::UpdateTheme() -> void
{
    if (_theme)
    {
        _theme->Apply(GetAttributeNode());
    }
}
}
