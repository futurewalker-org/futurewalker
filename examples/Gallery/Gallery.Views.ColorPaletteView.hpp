// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.View.hpp>

#include <Futurewalker.Core.TimeInterval.hpp>

namespace Futurewalker::Gallery
{
class ColorPaletteView : public View
{
public:
    static auto Make() -> Shared<ColorPaletteView>;

    ColorPaletteView(PassKey<View> key);

    auto GetInitializeTime() const -> TimeInterval;

private:
    auto Initialize() -> void override;

private:
    TimeInterval _initializeTime;
};
}
