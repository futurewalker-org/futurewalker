// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Platform.Prelude.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Base class of platform view layer controls.
///
/// @note Used for decoupling functional aspect of layers from platform implementation.
/// @note Should weakly reference owning layer.
///
class PlatformViewLayerControl : NonCopyable
{
public:
    virtual ~PlatformViewLayerControl() = 0;
};
}
}
