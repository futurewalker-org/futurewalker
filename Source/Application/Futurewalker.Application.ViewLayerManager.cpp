// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ViewLayerManager.hpp"
#include "Futurewalker.Application.ViewLayer.hpp"
#include "Futurewalker.Application.DrawableViewLayer.hpp"
#include "Futurewalker.Application.PlatformDrawableViewLayerContext.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
UniqueViewLayerKind ViewLayerKindNormal;   // A kind of plain ViewLayer instances.
UniqueViewLayerKind ViewLayerKindDrawable; // A kind of DrawableViewLayer instances.
}

///
/// @brief
///
/// @param viewLayerContext
/// @param drawableViewLayerContext
///
ViewLayerManager::ViewLayerManager(Shared<PlatformViewLayerContext> const& viewLayerContext, Shared<PlatformDrawableViewLayerContext> const& drawableViewLayerContext)
  : _viewLayerContext {viewLayerContext}
  , _drawableViewLayerContext {drawableViewLayerContext}
{
    Register(ViewLayerKindNormal, [&] { return ViewLayer::Make(_viewLayerContext->MakeViewLayer()); });
    Register(ViewLayerKindDrawable, [&] { return DrawableViewLayer::Make(_drawableViewLayerContext->MakeDrawableLayer()); });
}

///
/// @brief
///
/// @param kind
///
auto ViewLayerManager::MakeLayer(ViewLayerKind const kind) -> Shared<ViewLayer>
{
    const auto it = _map.find(kind);
    if (it != _map.end())
    {
        return it->second();
    }
    return {};
}

///
/// @brief
///
/// @param kind
/// @param function
///
auto ViewLayerManager::Register(ViewLayerKind const kind, ViewLayerManagerMakeFunction const& function) -> void
{
    _map.insert_or_assign(kind, function);
}

///
/// @brief
///
auto Locator::Resolver<ViewLayerManager>::Resolve() -> Shared<ViewLayerManager>
{
    auto viewLayerContext = Locator::Resolve<PlatformViewLayerContext>();
    auto drawbleViewLayerContext = Locator::Resolve<PlatformDrawableViewLayerContext>();
    return Shared<ViewLayerManager>::Make(viewLayerContext, drawbleViewLayerContext);
}
}
