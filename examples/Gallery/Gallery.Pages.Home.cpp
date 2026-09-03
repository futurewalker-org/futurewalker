// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#include "Gallery.Pages.Home.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>

#include <Futurewalker.Base.SystemInfo.hpp>
#include <Futurewalker.Base.Locale.hpp>

#include <Futurewalker.Resource.ResourceManager.hpp>
#include <Futurewalker.Resource.Resource.hpp>

#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>

#include <Futurewalker.Core.StringFunction.hpp>

#include "Resource/Gallery.hpp"
#include "Resource/Gallery.String.HomePage.hpp"

namespace Futurewalker::Gallery
{
auto HomePage::Make() -> Shared<HomePage>
{
    return View::MakeDerived<HomePage>();
}

HomePage::HomePage(PassKey<View> key)
  : View(key)
{
}

auto HomePage::Initialize() -> void
{
    auto column = FlexLayout::Make();
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);
    {
        auto resource = ResourceManager::GetResource(M::Gallery);
        {
            auto text = Lamp::TextView::Make();
            text->SetText(resource->LoadString(R::Gallery::String::HomePage::Version));
            text->SetEnabled(false);
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeTitleSmall);
            FlexLayout::SetChildMargin(text, EdgeInsets(0, 0, 0, 10));
            column->AddChild(text);
        }
        {
            auto text = Lamp::TextView::Make();
            text->SetText(resource->LoadString(R::Gallery::String::HomePage::Title));
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeDisplaySmall);
            FlexLayout::SetChildMargin(text, EdgeInsets(0, 0, 0, 10));
            column->AddChild(text);
        }
        // Platform
        {
            auto os = resource->LoadString(R::Gallery::String::HomePage::Platform);
            auto osName = resource->LoadString(R::Gallery::String::HomePage::Unknown);
            auto const platformKind = SystemInfo::GetPlatformKind();
            if (platformKind == PlatformKind::Windows)
            {
                osName = resource->LoadString(R::Gallery::String::HomePage::Windows);
            }
            else if (platformKind == PlatformKind::MacOS)
            {
                osName = resource->LoadString(R::Gallery::String::HomePage::MacOS);
            }
            auto text = Lamp::TextView::Make();
            text->SetText(StringFunction::Format(u8"{}: {}", os, osName));
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeBodyLarge);
            column->AddChild(text);
        }
        // Platform version
        {
            auto version = resource->LoadString(R::Gallery::String::HomePage::SystemVersion);
            auto systemVersion = SystemInfo::GetSystemVersion();
            auto text = Lamp::TextView::Make();
            text->SetText(StringFunction::Format(u8"{}: {}.{}.{}", version, systemVersion.major, systemVersion.minor, systemVersion.patch));
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeBodyLarge);
            column->AddChild(text);
        }
        // Preferred locales
        {
            auto const preferredLocales = Locale::GetPreferredUILocales();
            auto const text = Lamp::TextView::Make();
            auto locales = String();
            for (auto const& locale : preferredLocales)
            {
                if (!locales.IsEmpty())
                {
                    locales.Append(u8", ");
                }
                locales.Append(locale.GetLanguageTag());
            }
            auto const preferredLocalesLabel = resource->LoadString(R::Gallery::String::HomePage::PreferredLocales);
            text->SetText(StringFunction::Format(u8"{}: {}", preferredLocalesLabel, locales));
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeBodyLarge);
            column->AddChild(text);
        }
        // System locale
        {
            auto const systemLocale = Locale::GetSystemLocale();
            auto const text = Lamp::TextView::Make();
            auto const systemLocaleLabel = resource->LoadString(R::Gallery::String::HomePage::SystemLocale);
            text->SetText(StringFunction::Format(u8"{}: {}", systemLocaleLabel, systemLocale.GetLanguageTag()));
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeBodyLarge);
            column->AddChild(text);
        }
        // Representative locale
        {
            auto const representativeLocale = ResourceManager::GetRepresentativeLocale();
            auto const text = Lamp::TextView::Make();
            auto const representativeLocaleLabel = resource->LoadString(R::Gallery::String::HomePage::RepresentativeLocale);
            if (representativeLocale.HasValue())
            {
                text->SetText(StringFunction::Format(u8"{}: {}", representativeLocaleLabel, representativeLocale->GetLanguageTag()));
            }
            else
            {
                text->SetText(StringFunction::Format(u8"{}: {}", representativeLocaleLabel, resource->LoadString(R::Gallery::String::HomePage::None)));
            }
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeBodyLarge);
            column->AddChild(text);
        }
    }
    auto padding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingLarge, column);
    AddChildBack(padding);
}
}
