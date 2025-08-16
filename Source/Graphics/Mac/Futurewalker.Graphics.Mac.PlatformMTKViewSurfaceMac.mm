// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Mac.PlatformMTKViewSurfaceMac.hpp"

#include "Futurewalker.Graphics.SkiaScene.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
PlatformMTKViewSurfaceMac::PlatformMTKViewSurfaceMac(id<MTLDevice> metalDevice, id<MTLCommandQueue> metalCommandQueue)
  : _metalDevice {metalDevice}
  , _metalCommandQueue {metalCommandQueue}
{
    auto backendContext = GrMtlBackendContext();
    backendContext.fDevice.reset((__bridge void*)_metalDevice);
    backendContext.fQueue.reset((__bridge void*)_metalCommandQueue);
    GrContextOptions grContextOptions;
    _context = GrDirectContexts::MakeMetal(backendContext, grContextOptions);
}

auto PlatformMTKViewSurfaceMac::SetupConfiguration(MTKView* mtkView) -> void
{
    @autoreleasepool
    {
        [mtkView setDepthStencilPixelFormat:MTLPixelFormatDepth32Float_Stencil8];
        [mtkView setColorPixelFormat:MTLPixelFormatBGRA8Unorm];
        [mtkView setSampleCount:1];
    }
}

auto PlatformMTKViewSurfaceMac::Draw(MTKView* mtkView, Function<void(Scene& canvas)> func) -> Bool
{
    @autoreleasepool
    {
        auto const colorType = kBGRA_8888_SkColorType;
        auto const colorSpace = sk_sp<SkColorSpace>();
        auto const origin = kTopLeft_GrSurfaceOrigin;
        auto const surfaceProps = SkSurfaceProps();
        auto const sampleCount = static_cast<int>([mtkView sampleCount]);
        if (auto const surface = SkSurfaces::WrapMTKView(_context.get(), (__bridge GrMTLHandle)mtkView, origin, sampleCount, colorType, colorSpace, &surfaceProps))
        {
            if (auto const canvas = surface->getCanvas())
            {
                canvas->save();

                auto r1 = rand()%255;
                auto r2 = rand()%255;
                auto r3 = rand()%255;
                canvas->clear(SkColor4f(r1/255.0,r2/255.0,r3/255.0,1));

                try
                {
                    if (func)
                    {
                        auto const scene = Shared<SkiaScene>::Make();
                        scene->SetCanvas(canvas);
                        func(*scene);
                    }
                }
                catch (...)
                {
                }
                canvas->restore();
                _context->flushAndSubmit(surface.get());
            }
        }
        auto const commandBuffer = [_metalCommandQueue commandBuffer];
        [commandBuffer presentDrawable:[mtkView currentDrawable]];
        [commandBuffer commit];
        return true;
    }
}
}
