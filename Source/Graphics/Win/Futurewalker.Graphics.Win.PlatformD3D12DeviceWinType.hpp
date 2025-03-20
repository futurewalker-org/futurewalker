// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.Win.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformD3D12DeviceWin;

///
/// @brief Specifies which adapter (GPU) will be used by D3D12 device.
///
/// @note This option might be ignored on old systems.
///
enum class PlatformD3D12DevicePreference
{
    Unspecified,     // No preference.
    HighPerformance, // Select high performance GPU.
    MinimumPower,    // Select low power GPU.
};
}
}
