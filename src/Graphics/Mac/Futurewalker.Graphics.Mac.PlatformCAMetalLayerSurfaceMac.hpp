// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Mac.PlatformCAMetalLayerSurfaceMacType.hpp"

#include "Futurewalker.Graphics.SceneType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Function.hpp"

#include <include/gpu/ganesh/GrDirectContext.h>
#include <include/gpu/ganesh/mtl/GrMtlDirectContext.h>
#include <include/gpu/ganesh/mtl/GrMtlBackendContext.h>
#include <include/gpu/ganesh/mtl/SkSurfaceMetal.h>
#include <include/core/SkSurface.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkColorSpace.h>

#import <MetalKit/MetalKit.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief MTKView surface.
///
class PlatformCAMetalLayerSurfaceMac : NonCopyable
{
public:
    PlatformCAMetalLayerSurfaceMac(id<MTLDevice> metalDevice, id<MTLCommandQueue> metalQueue);

    auto SetBackingScale(BackingScale const backingScale) -> void;
    auto SetDisplayScale(DisplayScale const displayScale) -> void;
    auto SetSize(Size<Dp> const& size) -> void;
    auto Draw(Function<void(Scene& canvas)> func) -> Bool;
    auto GetMetalLayer() -> CAMetalLayer*;

private:
    auto ResizeSurface() -> void;

private:
    __strong CAMetalLayer* _metalLayer = nil;
    __strong id<MTLDevice> _metalDevice = nil;
    __strong id<MTLCommandQueue> _metalCommandQueue = nil;
    sk_sp<GrDirectContext> _context;
    Size<Dp> _size;
    DisplayScale _displayScale = 1.0;
    BackingScale _backingScale = 1.0;
    Bool _needsUpdateSize = true;
};
}
}
