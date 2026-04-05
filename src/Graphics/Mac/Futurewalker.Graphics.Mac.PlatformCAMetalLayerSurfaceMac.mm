// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Mac.PlatformCAMetalLayerSurfaceMac.hpp"

#include "Futurewalker.Graphics.SkiaScene.hpp"

#include "Futurewalker.Unit.UnitFunction.hpp"

#include "Futurewalker.Base.Debug.hpp"

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

        // Synchronize presentation with current CATransaction to avoid desynch between frame buffer and CALayer offset/size changes.
        [_metalLayer setPresentsWithTransaction:YES];

        auto backendContext = GrMtlBackendContext();
        backendContext.fDevice.retain((__bridge void*)_metalDevice);
        backendContext.fQueue.retain((__bridge void*)_metalCommandQueue);
        auto grContextOptions = GrContextOptions();
        _context = GrDirectContexts::MakeMetal(backendContext, grContextOptions);
    }
}

auto PlatformCAMetalLayerSurfaceMac::SetBackingScale(BackingScale const backingScale) -> void
{
    if (_backingScale != backingScale)
    {
        _backingScale = backingScale;
        _needsUpdateSize = true;
    }
}

auto PlatformCAMetalLayerSurfaceMac::SetDisplayScale(DisplayScale const displayScale) -> void
{
    if (_displayScale != displayScale)
    {
        _displayScale = displayScale;
        _needsUpdateSize = true;
    }
}

auto PlatformCAMetalLayerSurfaceMac::SetSize(Size<Dp> const& size) -> void
{
    if (_size != size)
    {
        _size = size;
        _needsUpdateSize = true;
    }
}

auto PlatformCAMetalLayerSurfaceMac::Draw(Function<void(Scene& canvas)> func) -> Bool
{
    @autoreleasepool
    {
        if (!_context)
        {
            return false;
        }

        ResizeSurface();

        auto currentDrawable = GrMTLHandle(nullptr);
        auto const colorType = kBGRA_8888_SkColorType;
        auto const colorSpace = sk_sp<SkColorSpace>();
        auto const origin = kTopLeft_GrSurfaceOrigin;
        auto const surfaceProps = SkSurfaceProps();
        auto const sampleCount = static_cast<int>(1);
        if (auto surface = SkSurfaces::WrapCAMetalLayer(_context.get(), (__bridge GrMTLHandle)_metalLayer, origin, sampleCount, colorType, colorSpace, &surfaceProps, &currentDrawable))
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
                    FW_DEBUG_ASSERT(false);
                }
            }

            // SkSurfaces::WrapCAMetalLayer() defers drawable acquisition until command submission.
            _context->flushAndSubmit(surface.get(), GrSyncCpu::kNo);

            if (currentDrawable)
            {
                // Skia uses different command buffer from _metalCommandQueue for rendering.
                // We assume command queue guarantees that command buffer commited early will be scheduled before command buffer commited later.
                auto const commandBuffer = [_metalCommandQueue commandBuffer];
                [commandBuffer commit];
                [commandBuffer waitUntilScheduled];

                // SkSurfaces::WrapCAMetalLayer() returns a drawable as CF pointer via __bridge_retained.
                // Use __bridge_transfer to transfer ownership back to ARC.
                auto drawable = (__bridge_transfer id<CAMetalDrawable>)currentDrawable;
                [drawable present];
            }
            else
            {
                FW_DEBUG_ASSERT(false);
                return false;
            }
            currentDrawable = nil;
        }
        else
        {
            FW_DEBUG_ASSERT(false);
            return false;
        }
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

auto PlatformCAMetalLayerSurfaceMac::ResizeSurface() -> void
{
    @autoreleasepool
    {
        if (_needsUpdateSize)
        {
            auto const sizePx = UnitFunction::ConvertDpToPxRound(_size, _displayScale, _backingScale);
            [_metalLayer setDrawableSize:CGSizeMake(static_cast<CGFloat>(sizePx.GetWidth()), static_cast<CGFloat>(sizePx.GetHeight()))];
            [_metalLayer setContentsScale:static_cast<CGFloat>(_backingScale)];
            _needsUpdateSize = false;
        }
    }
}
}
