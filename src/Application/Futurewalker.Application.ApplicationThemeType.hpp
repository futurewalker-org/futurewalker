// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ApplicationTheme;

///
/// @brief Brightness type of application theme.
///
enum class ApplicationThemeBrightness
{
    System, ///< System theme brightness.
    Light,  ///< Light theme.
    Dark,   ///< Dark theme.
};
}
}
