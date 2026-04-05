// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformRootViewLayerMac.hpp"
#include "Futurewalker.Application.Mac.PlatformViewLayerVisualMac.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualRenderer.hpp"

#import <QuartzCore/QuartzCore.h>

static NSComparisonResult CompareSubviewOrder(__kindof NSView* _Nonnull view1, __kindof NSView* _Nonnull view2, void* _Nullable context)
{
    @autoreleasepool
    {
        auto const order = (__bridge NSArray*)context;
        auto const index1 = [order indexOfObjectIdenticalTo:view1];
        auto const index2 = [order indexOfObjectIdenticalTo:view2];

        if (index1 < index2)
        {
            return NSOrderedAscending;
        }
        if (index1 > index2)
        {
            return NSOrderedDescending;
        }
        return NSOrderedSame;
    }
}

@interface PlatformRootViewLayerView : NSView
@end

@implementation PlatformRootViewLayerView
- (NSView*)hitTest:(NSPoint)point
{
    auto hitView = [super hitTest:point];
    return hitView == self ? nil : hitView;
}
@end

namespace FW_DETAIL_NS
{
auto PlatformRootViewLayerMac::Make(Shared<PlatformViewLayerVisualContextMac> const& visualContext, NSWindow* window) -> Shared<PlatformRootViewLayerMac>
{
    @autoreleasepool
    {
        return PlatformViewLayer::MakeDerived<PlatformRootViewLayerMac>(visualContext, window);
    }
}

PlatformRootViewLayerMac::PlatformRootViewLayerMac(PassKey<PlatformViewLayer> key, Shared<PlatformViewLayerVisualContextMac> const& visualContext, NSWindow* window)
  : PlatformViewLayerMac(key)
  , _window {window}
  , _visualContext {visualContext}
{
    @autoreleasepool
    {
        _view = [PlatformRootViewLayerView new];
        [_view setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
        [_view setFrame:_window.contentView.bounds];
        [_window.contentView addSubview:_view];
    }
}

auto PlatformRootViewLayerMac::GetControl() -> Shared<PlatformViewLayerControl>
{
    return {};
}

auto PlatformRootViewLayerMac::Render() -> void
{
    if (_renderer)
    {
        _renderer->Render();
    }
}

auto PlatformRootViewLayerMac::Initialize() -> void
{
    PlatformViewLayerMac::Initialize();

    SetClipMode(ViewClipMode::Bounds);

    auto const layer = GetSelf();
    auto const context = _visualContext;
    _renderer = Unique<PlatformViewLayerVisualRenderer>::Make(
      PlatformViewLayerVisualRenderer::Delegate {
        .renderBegin = [this]() -> void {
            @autoreleasepool
            {
                [CATransaction begin];
                [CATransaction setDisableActions:YES];
                _expectedViews = [NSMutableArray new];
            }
        },
        .renderEnd = [this]() -> void {
            @autoreleasepool
            {
                // Remove subviews that are no longer needed.
                for (NSView* subview in [[_view subviews] copy])
                {
                    if (![_expectedViews containsObject:subview])
                    {
                        [subview removeFromSuperview];
                    }
                }
                // Reorder subviews to match the expected order.
                [_view sortSubviewsUsingFunction:CompareSubviewOrder context:(__bridge void*)_expectedViews];
                _expectedViews = nil;
                [CATransaction commit];
            }
        },
        .renderVisual = [this](Shared<PlatformViewLayerVisual> const& visual) -> void {
            @autoreleasepool
            {
                if (auto const visualMac = visual.TryAs<PlatformViewLayerVisualMac>())
                {
                    auto const visualView = visualMac->GetView();
                    [visualView setFrame:_view.bounds];
                    [visualView setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
                    if ([visualView superview] != _view)
                    {
                        [_view addSubview:visualView];
                    }
                    [_expectedViews addObject:visualView];
                    visualMac->Render(_window);
                }
            }
        },
      },
      layer,
      context);
}

auto PlatformRootViewLayerMac::RootGetDisplayScale() const -> DisplayScale
{
    return 1.0;
}

auto PlatformRootViewLayerMac::RootGetBackingScale() const -> BackingScale
{
    return [_window backingScaleFactor];
}

auto PlatformRootViewLayerMac::RootOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

auto PlatformRootViewLayerMac::RootSizeChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

auto PlatformRootViewLayerMac::RootClipPathChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

auto PlatformRootViewLayerMac::RootClipModeChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

auto PlatformRootViewLayerMac::RootOpacityChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

auto PlatformRootViewLayerMac::RootRenderFlagsChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestRebuildLayer();
    }
}

auto PlatformRootViewLayerMac::RootDisplayListChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    // TODO: Optimize to only update changed parts.
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

auto PlatformRootViewLayerMac::RootDisplayListOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    // TODO: Optimize to only update changed parts.
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

auto PlatformRootViewLayerMac::RootChildAdded(Shared<PlatformViewLayer> const& child) -> void
{
    (void)child;
    if (_renderer)
    {
        _renderer->RequestRebuildLayer();
    }
}

auto PlatformRootViewLayerMac::RootChildRemoved(Shared<PlatformViewLayer> const& parent) -> void
{
    (void)parent;
    if (_renderer)
    {
        _renderer->RequestRebuildLayer();
    }
}
}
