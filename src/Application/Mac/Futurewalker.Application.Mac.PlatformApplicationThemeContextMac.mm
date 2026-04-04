// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformApplicationThemeContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformApplicationThemeMac.hpp"

using namespace FW_NS;

@interface PlatformApplicationThemeContextObserver : NSObject
@property(assign, nonatomic) Weak<PlatformApplicationThemeContextMac> context;
@end

@implementation PlatformApplicationThemeContextObserver

- (instancetype)init
{
    self = [super init];
    if (self)
    {
        [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(interfaceThemeChanged:) name:@"AppleInterfaceThemeChangedNotification" object:nil];
    }
    return self;
}

- (void)interfaceThemeChanged:(NSNotification*)notification
{
    if (auto context = _context.Lock())
    {
        context->UpdateSystemBrightness();
    }
}
@end

namespace FW_DETAIL_NS
{
auto PlatformApplicationThemeContextMac::Make() -> Shared<PlatformApplicationThemeContextMac>
{
    return PlatformApplicationThemeContext::MakeDerived<PlatformApplicationThemeContextMac>();
}

PlatformApplicationThemeContextMac::PlatformApplicationThemeContextMac(PassKey<PlatformApplicationThemeContext> key)
  : PlatformApplicationThemeContext(key)
{
}

auto PlatformApplicationThemeContextMac::MakeApplicationTheme(PlatformApplicationTheme::Delegate const& delegate) -> Shared<PlatformApplicationTheme>
{
    auto theme = PlatformApplicationThemeMac::Make(delegate, GetSelf());
    _applicationThemes.push_back(theme);
    return theme;
}

auto PlatformApplicationThemeContextMac::GetSystemBrightness() const -> ThemeBrightness
{
    return _systemBrightness;
}

auto PlatformApplicationThemeContextMac::GetCurrentSystemBrightness() const -> ThemeBrightness
{
    @autoreleasepool
    {
        auto const appearance = [NSApp effectiveAppearance];
        auto const matched = [appearance bestMatchFromAppearancesWithNames:@[NSAppearanceNameAqua, NSAppearanceNameDarkAqua]];
        if ([matched isEqualToString:NSAppearanceNameAqua])
        {
            return ThemeBrightness::Light;
        }
        return ThemeBrightness::Dark;
    }
}

auto PlatformApplicationThemeContextMac::Initialize() -> void
{
    _systemBrightness = GetCurrentSystemBrightness();

    @autoreleasepool
    {
        auto const observer = [[PlatformApplicationThemeContextObserver alloc] init];
        observer.context = GetSelf();
        _observer = observer;
    }
}

auto PlatformApplicationThemeContextMac::UpdateSystemBrightness() -> void
{
    auto const newBrightness = GetCurrentSystemBrightness();
    if (_systemBrightness != newBrightness)
    {
        _systemBrightness = newBrightness;

        for (auto const& weakTheme : _applicationThemes)
        {
            if (auto const theme = weakTheme.Lock())
            {
                theme->NotifyBrightnessChanged();
            }
        }
    }
}

auto Locator::Resolver<PlatformApplicationThemeContextMac>::Resolve() -> Shared<PlatformApplicationThemeContextMac>
{
    return PlatformApplicationThemeContextMac::Make();
}
}
