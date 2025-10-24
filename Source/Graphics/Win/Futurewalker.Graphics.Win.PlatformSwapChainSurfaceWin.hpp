// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Win.PlatformSwapChainSurfaceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceObjectWin.hpp"
#include "Futurewalker.Graphics.SceneType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Function.hpp"

#include <dxgi1_4.h>
#include <wrl/client.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Base class for swap chain surface.
///
class PlatformSwapChainSurfaceWin : public PlatformGraphicsDeviceObjectWin
{
public:
    struct Delegate
    {
        Function<void(PlatformSwapChainSurfaceWin& surface)> deviceLost;
        Function<void(PlatformSwapChainSurfaceWin& surface)> deviceRestored;
    };
    virtual ~PlatformSwapChainSurfaceWin() = 0;

    ///
    /// @brief Resize size of buffer.
    ///
    /// @param width New width of buffer. Should be >= 1.
    /// @param height New height of buffer. Should be >= 1.
    ///
    virtual auto Resize(IntPx const width, IntPx const height) -> Bool = 0;

    ///
    /// @brief Draw onto current buffer then swap.
    ///
    /// @param func Draw function.
    ///
    virtual auto Draw(Function<void(Scene& canvas)> func) -> Bool = 0;

    ///
    /// @brief Get content of swap chain.
    ///
    virtual auto GetSwapChain() -> Microsoft::WRL::ComPtr<IUnknown> = 0;

    ///
    /// @brief Set delegate object.
    ///
    virtual auto SetDelegate(Delegate delegate) -> void = 0;
};
}
}
