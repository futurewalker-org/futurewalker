// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.PlatformScreenWinType.hpp"
#include "Futurewalker.Application.PlatformScreen.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief 
///
class PlatformScreenWin : public PlatformScreen
{
public:
    explicit PlatformScreenWin(HMONITOR monitor);

    auto GetInfo() const -> Optional<ScreenInfo> override;
    auto Refresh() -> void;
    auto Invalidate() -> void;

    auto GetNativeHandle() const -> HMONITOR;

private:
    HMONITOR _monitor = NULL;
    Optional<ScreenInfo> _info;
};
}
}
