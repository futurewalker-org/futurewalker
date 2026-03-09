// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Mac.PlatformCAMetalLayerSurfaceMac.hpp"

#include "Futurewalker.Graphics.SkiaScene.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
PlatformCAMetalLayerSurfaceMac::PlatformCAMetalLayerSurfaceMac(id<MTLDevice> metalDevice, id<MTLCommandQueue> metalCommandQueue)
  : _metalDevice {metalDevice}
  , _metalCommandQueue {metalCommandQueue}
{
    @autoreleasepool
    {
        _metalLayer = [CAMetalLayer layer];
        [_metalLayer setDevice:_metalDevice];
        [_metalLayer setPixelFormat:MTLPixelFormatBGRA8Unorm];
        [_metalLayer setOpaque:NO];

        auto backendContext = GrMtlBackendContext();
        backendContext.fDevice.reset((__bridge void*)_metalDevice);
        backendContext.fQueue.reset((__bridge void*)_metalCommandQueue);
        auto grContextOptions = GrContextOptions();
        _context = GrDirectContexts::MakeMetal(backendContext, grContextOptions);
    }
}

auto PlatformCAMetalLayerSurfaceMac::Resize(IntPx const width, IntPx const height) -> void
{
    @autoreleasepool
    {
        auto const newSize = CGSizeMake(static_cast<CGFloat>(width), static_cast<CGFloat>(height));
        [_metalLayer setDrawableSize:newSize];
    }
}

auto PlatformCAMetalLayerSurfaceMac::Draw(Function<void(Scene& canvas)> func) -> Bool
{
    @autoreleasepool
    {
        auto currentDrawable = GrMTLHandle(nullptr);
        auto const colorType = kBGRA_8888_SkColorType;
        auto const colorSpace = sk_sp<SkColorSpace>();
        auto const origin = kTopLeft_GrSurfaceOrigin;
        auto const surfaceProps = SkSurfaceProps();
        auto const sampleCount = static_cast<int>(1);
        if (auto const surface = SkSurfaces::WrapCAMetalLayer(_context.get(), (__bridge GrMTLHandle)_metalLayer, origin, sampleCount, colorType, colorSpace, &surfaceProps, &currentDrawable))
        {
            if (auto const canvas = surface->getCanvas())
            {
                canvas->clear(SkColor4f(0, 0, 0, 0));

                auto const scene = Shared<SkiaScene>::Make();
                scene->SetCanvas(canvas);

                try
                {
                    if (func)
                    {
                        func(*scene);
                    }
                }
                catch (...)
                {
                }
            }
            _context->flushAndSubmit(surface.get(), GrSyncCpu::kYes);
        }
        auto const commandBuffer = [_metalCommandQueue commandBuffer];
        [commandBuffer presentDrawable:(__bridge id<CAMetalDrawable>)currentDrawable];
        [commandBuffer commit];
        return true;
    }
}

auto PlatformCAMetalLayerSurfaceMac::GetMetalLayer() -> CAMetalLayer*
{
    @autoreleasepool
    {
        return _metalLayer;
    }
}
}
