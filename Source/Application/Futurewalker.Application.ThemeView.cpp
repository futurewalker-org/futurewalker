// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ThemeView.hpp"
#include "Futurewalker.Application.Theme.hpp"
#include "Futurewalker.Application.ContainerView.hpp"

namespace FW_DETAIL_NS
{
auto ThemeView::Make() -> Shared<ThemeView>
{
    return View::MakeDerived<ThemeView>();
}

auto ThemeView::MakeWithContent(Shared<View> const& content) -> Shared<ThemeView>
{
    auto view = Make();
    view->SetContent(content);
    return view;
}

ThemeView::ThemeView(PassKey<View> key)
  : View(key)
{
}

auto ThemeView::GetContent() const -> Shared<View>
{
    if (_container)
    {
        return _container->GetContent();
    }
    return {};
}

auto ThemeView::SetContent(Shared<View> const& content) -> void
{
    if (_container)
    {
        _container->SetContent(content);
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

auto ThemeView::Initialize() -> void
{
    _container = ContainerView::Make();
    AddChildBack(_container);
}

auto ThemeView::UpdateTheme() -> void
{
    if (_theme)
    {
        _theme->Apply(GetAttributeNode());
    }
}
}
