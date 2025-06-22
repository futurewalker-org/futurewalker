// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.String.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class Screen;
using ScreenArray = std::vector<Shared<Screen>>;

///
/// @brief Information of screen.
///
struct ScreenInfo
{
    ///
    /// @brief Frame rectangle of the screen.
    ///
    /// @note Relative to origin of global screen space.
    ///
    Rect<Dp> frameRect;

    ///
    /// @brief Work are rectangle of the screen.
    ///
    /// @note Relative to top-left corner of the screen.
    ///
    Rect<Dp> workAreaRect;

    ///
    /// @brief Safe area rectangle of the screen.
    ///
    /// @note Relative to top-left corner of the screen.
    ///
    Rect<Dp> safeAreaRect;

    ///
    /// @brief Display scale of the screen.
    ///
    DisplayScale displayScale = 0;

    ///
    /// @brief Backing scale of the screen.
    ///
    BackingScale backingScale = 0;

    ///
    /// @brief Screen's display name.
    ///
    String displayName;
};
}
}
