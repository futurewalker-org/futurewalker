// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.String.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class Application;

///
/// @brief Options for creating Application object.
///
struct ApplicationOptions
{
    ///
    /// @brief An application identifier in reverse-DNS format.
    ///
    String identifier = u8"com.example.Futurewalker.Application";
};
}
}
