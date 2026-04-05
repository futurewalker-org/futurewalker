// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformViewLayerVisualMac.hpp"

#include "Futurewalker.Graphics.DisplayList.hpp"
#include "Futurewalker.Graphics.SkiaScene.hpp"
#include "Futurewalker.Graphics.Mac.PlatformCAMetalLayerSurfaceMac.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include <include/gpu/ganesh/GrDirectContext.h>
#include <include/gpu/ganesh/mtl/GrMtlDirectContext.h>
#include <include/gpu/ganesh/mtl/GrMtlBackendContext.h>
#include <include/gpu/ganesh/mtl/SkSurfaceMetal.h>
#include <include/core/SkSurface.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkColorSpace.h>

#import <MetalKit/MetalKit.h>

using namespace Futurewalker;

// NSView subclass that hosts a CAMetalLayer for rendering.
// CAMetalLayer is managed as sublayer of this view's layer.
@interface PlatformCAMetalLayerView : NSView
@property(assign, nonatomic) Shared<Graphics::PlatformCAMetalLayerSurfaceMac> surface;
@property(strong, nonatomic) id<MTLDevice> metalDevice;
@property(strong, nonatomic) id<MTLCommandQueue> metalQueue;
@property(strong, nonatomic) CALayer* maskLayer;
- (instancetype)initWithFrame:(CGRect)frameRect;
@end

@implementation PlatformCAMetalLayerView
- (instancetype)initWithFrame:(CGRect)frameRect
{
    if (self = [super initWithFrame:frameRect])
    {
        auto const metalDevice = MTLCreateSystemDefaultDevice(); // TODO: Share MTLDevice instance across the app.
        auto const metalCommandQueue = [metalDevice newCommandQueue];

        FW_DEBUG_ASSERT(metalDevice);
        FW_DEBUG_ASSERT(metalCommandQueue);

        // Become layer-backed view to host sublayers.
        self.wantsLayer = YES;
        self.metalDevice = metalDevice;
        self.metalQueue = metalCommandQueue;
        self.surface = Shared<Graphics::PlatformCAMetalLayerSurfaceMac>::Make(metalDevice, metalCommandQueue);

        auto const metalLayer = self.surface->GetMetalLayer();
        metalLayer.anchorPoint = CGPointMake(0, 0);
        metalLayer.contentsGravity = kCAGravityTopLeft;
        [self.layer addSublayer:metalLayer];

        // Setup mask layer for clipping.
        self.maskLayer = [CALayer new];
        self.maskLayer.masksToBounds = YES;
        self.maskLayer.contentsFormat = kCAContentsFormatGray8Uint;
        self.maskLayer.backgroundColor = [NSColor blackColor].CGColor;
        self.maskLayer.anchorPoint = CGPointMake(0, 0);
        metalLayer.mask = self.maskLayer;
    }
    return self;
}

- (NSView*)hitTest:(NSPoint)point
{
    auto hitView = [super hitTest:point];
    return hitView == self ? nil : hitView;
}

- (void)setLayerBounds:(CGRect)bounds
{
    if (auto const surface = self.surface)
    {
        auto const metalLayer = surface->GetMetalLayer();
        metalLayer.position = bounds.origin;
        metalLayer.bounds = CGRectMake(0, 0, bounds.size.width, bounds.size.height);
    }
}

- (void)setMaskBounds:(CGRect)bounds
{
    if (self.maskLayer)
    {
        self.maskLayer.position = bounds.origin;
        self.maskLayer.bounds = CGRectMake(0, 0, bounds.size.width, bounds.size.height);
    }
}

- (void)drawVisual:(Shared<PlatformViewLayerVisual>)visual withDrawableRect:(Futurewalker::Rect<Dp>)drawableRect
{
    auto const surface = [self surface];
    if (!surface)
    {
        return;
    }

    if (!visual)
    {
        return;
    }
    auto const& clipPaths = visual->GetClipPaths();
    auto const displayScale = visual->GetDisplayScale();
    auto const backingScale = visual->GetBackingScale();
    auto const scale = static_cast<FW_NS::Float64>(displayScale) * static_cast<FW_NS::Float64>(backingScale);

    surface->SetDisplayScale(displayScale);
    surface->SetBackingScale(backingScale);
    surface->SetSize(drawableRect.GetSize());
    surface->Draw([&](Graphics::Scene& scene) {
        scene.PushScale({.x = scale, .y = scale});
        for (auto const& clipPath : clipPaths)
        {
            scene.PushClipPath({.path = clipPath, .antiAlias = true});
        }
        scene.PushTranslate({.x = -drawableRect.GetLeft(), .y = -drawableRect.GetTop()});
        visual->ForEachFragment([&](auto const& fragmentInfo) {
            if (fragmentInfo.type == PlatformViewLayerVisual::FragmentType::PushNode)
            {
                if (auto const fragment = visual->GetPushNodeFragment(fragmentInfo.index))
                {
                    scene.PushTranslate({.x = fragment->offset.GetDeltaX(), .y = fragment->offset.GetDeltaY()});
                    scene.PushClipRect({.rect = fragment->clipRect});

                    if (fragment->clipPath)
                    {
                        scene.PushClipPath({.path = *fragment->clipPath, .antiAlias = true});
                    }
                }
            }
            else if (fragmentInfo.type == PlatformViewLayerVisual::FragmentType::PopNode)
            {
                if (auto const fragment = visual->GetPopNodeFragment(fragmentInfo.index))
                {
                    scene.Pop({.count = 2});

                    if (auto const pushFragment = visual->GetPushNodeFragment(fragment->pushNodeIndex))
                    {
                        if (pushFragment->clipPath)
                        {
                            scene.Pop({.count = 1});
                        }
                    }
                }
            }
            else if (fragmentInfo.type == PlatformViewLayerVisual::FragmentType::DisplayList)
            {
                if (auto const fragment = visual->GetDisplayListFragment(fragmentInfo.index))
                {
                    scene.PushTranslate({.x = fragment->displayListOffset.GetDeltaX(), .y = fragment->displayListOffset.GetDeltaY()});
                    scene.AddDisplayList({.displayList = fragment->displayList});
                    scene.Pop({.count = 1});
                }
            }
        });
        scene.Pop({.count = std::ssize(clipPaths) + 2});
    });
}
@end

namespace FW_DETAIL_NS
{
auto PlatformViewLayerVisualMac::Make() -> Shared<PlatformViewLayerVisualMac>
{
    return PlatformViewLayerVisual::MakeDerived<PlatformViewLayerVisualMac>();
}

PlatformViewLayerVisualMac::PlatformViewLayerVisualMac(PassKey<PlatformViewLayerVisual> key)
  : PlatformViewLayerVisual(key)
{
    @autoreleasepool
    {
        _view = [[PlatformCAMetalLayerView alloc] initWithFrame:CGRectMake(0, 0, 1, 1)];
    }
}

auto PlatformViewLayerVisualMac::Render(NSWindow* window) -> void
{
    @autoreleasepool
    {
        if (_view && (_needsUpdateContents || _needsUpdateGeometry))
        {
            auto const view = static_cast<PlatformCAMetalLayerView*>(_view);
            auto const drawableRect = CalcFragmentBounds();

            if (_needsUpdateGeometry)
            {
                auto const offset = UnitFunction::ConvertDpToVp(GetOffset().As<Point>(), GetDisplayScale());
                auto const offsetX = static_cast<CGFloat>(offset.GetX());
                auto const offsetY = static_cast<CGFloat>(offset.GetY());

                auto const clip = UnitFunction::ConvertDpToVp(GetClipRect(), GetDisplayScale());
                auto const clipX = static_cast<CGFloat>(clip.GetLeft());
                auto const clipY = static_cast<CGFloat>(clip.GetTop());
                auto const clipWidth = static_cast<CGFloat>(clip.GetWidth());
                auto const clipHeight = static_cast<CGFloat>(clip.GetHeight());

                auto const bounds = UnitFunction::ConvertDpToVp(drawableRect, GetDisplayScale());
                auto const boundsX = static_cast<CGFloat>(bounds.GetLeft());
                auto const boundsY = static_cast<CGFloat>(bounds.GetTop());
                auto const boundsWidth = static_cast<CGFloat>(bounds.GetWidth());
                auto const boundsHeight = static_cast<CGFloat>(bounds.GetHeight());

                auto const layerX = offsetX + boundsX;
                auto const layerY = window.frame.size.height - (offsetY + boundsY + boundsHeight); // FIXME: Use content view's height instead.
                [view setLayerBounds:CGRectMake(layerX, layerY, boundsWidth, boundsHeight)];

                auto const maskX = clipX - boundsX;
                auto const maskY = (boundsY - clipY) + (boundsHeight - clipHeight);
                [view setMaskBounds:CGRectMake(maskX, maskY, clipWidth, clipHeight)];

                _needsUpdateGeometry = false;
            }

            if (_needsUpdateContents)
            {
                auto const metalLayerView = static_cast<PlatformCAMetalLayerView*>(_view);
                [metalLayerView drawVisual:GetSelf() withDrawableRect:drawableRect];
                _needsUpdateContents = false;
            }
        }
    }
}

auto PlatformViewLayerVisualMac::GetView() -> NSView*
{
    return _view;
}

auto PlatformViewLayerVisualMac::Initialize() -> void
{
}

auto PlatformViewLayerVisualMac::OnFragmentChanged() -> void
{
    _needsUpdateGeometry = true;
    _needsUpdateContents = true;
}

auto PlatformViewLayerVisualMac::OnOffsetChanged() -> void
{
    _needsUpdateGeometry = true;
}

auto PlatformViewLayerVisualMac::OnClipRectChanged() -> void
{
    _needsUpdateGeometry = true;
    _needsUpdateContents = true;
}

auto PlatformViewLayerVisualMac::OnClipPathChanged() -> void
{
    // TODO: Is there any way to set clip path in CALayer?
    _needsUpdateGeometry = true;
    _needsUpdateContents = true;
}

auto PlatformViewLayerVisualMac::OnOpacityChanged() -> void
{
    @autoreleasepool
    {
        auto const alpha = static_cast<CGFloat>(GetOpacity());
        [_view setAlphaValue:alpha];
    }
}

auto PlatformViewLayerVisualMac::OnDisplayScaleChanged() -> void
{
    _needsUpdateGeometry = true;
    _needsUpdateContents = true;
}

auto PlatformViewLayerVisualMac::OnBackingScaleChanged() -> void
{
    _needsUpdateGeometry = true;
    _needsUpdateContents = true;
}
}
