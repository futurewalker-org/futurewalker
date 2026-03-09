// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ViewLayerManagerType.hpp"
#include "Futurewalker.Application.PlatformViewLayerContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief ViewLayer manager.
///
class ViewLayerManager : public NonCopyable
{
public:
    ViewLayerManager(Shared<PlatformViewLayerContext> const& viewLayerContext);

    auto MakeLayer(ViewLayerKind const kind) -> Shared<ViewLayer>;
    auto Register(ViewLayerKind const kind, ViewLayerManagerMakeFunction const& function) -> void;

private:
    ViewLayerManagerMakeFunctionMap _map;
    Shared<PlatformViewLayerContext> _viewLayerContext;
};

template <>
struct Locator::Resolver<ViewLayerManager>
{
    using Interface = ViewLayerManager;
    static auto Resolve() -> Shared<ViewLayerManager>;
};
}
}
