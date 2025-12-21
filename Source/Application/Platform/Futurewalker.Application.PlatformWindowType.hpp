// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Platform.Prelude.hpp"

#include "Futurewalker.Application.WindowType.hpp"
#include "Futurewalker.Application.BoxConstraints.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.String.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformWindow;

///
/// @brief Options for creating PlatformWindow.
///
struct PlatformWindowOptions
{
    WindowBehavior behavior = WindowBehavior::Normal;
    WindowBackgroundStyle backgroundStyle = WindowBackgroundStyle::System;
    Shared<PlatformWindow> owner = nullptr;
    String title = String();
    Bool closable = true;
    Bool resizable = true;
    Bool minimizable = true;
    Bool hasFrame = true;
    Bool hasTitleBar = true;
    Bool visible = true;
    Bool maximized = false;
    Bool minimized = false;
    Bool allowActiveOwnerPopup = false;
    BoxConstraints sizeConstraints = BoxConstraints::MakeUnconstrained();
};
}
}
