// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Mac.PlatformMTKViewSurfaceMacType.hpp"

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
class PlatformMTKViewSurfaceMac : NonCopyable
{
public:
    PlatformMTKViewSurfaceMac(id<MTLDevice> metalDevice, id<MTLCommandQueue>);

    auto SetupConfiguration(MTKView* mtkView) -> void;
    auto Draw(MTKView* mtkView, Function<void(Scene& canvas)> func) -> Bool;

private:
    __strong id<MTLDevice> _metalDevice = nil;
    __strong id<MTLCommandQueue> _metalCommandQueue = nil;
    sk_sp<GrDirectContext> _context;
};
}
}
