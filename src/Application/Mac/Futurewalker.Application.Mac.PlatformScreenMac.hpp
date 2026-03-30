// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformScreenMacType.hpp"
#include "Futurewalker.Application.PlatformScreen.hpp"

#include <AppKit/AppKit.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Screen.
///
class PlatformScreenMac : public PlatformScreen
{
public:
    PlatformScreenMac(NSScreen* screen);

    auto GetInfo() const -> Optional<ScreenInfo> override;

    auto Refresh(CGFloat const mainScreenOrigin) -> void;
    auto Invalidate() -> void;
    auto GetNativeHandle() const -> NSScreen*;

private:
    __strong NSScreen* _screen = nil;
    ScreenInfo _info;
};
}
}
