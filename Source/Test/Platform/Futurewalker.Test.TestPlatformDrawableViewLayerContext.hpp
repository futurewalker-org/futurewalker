// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformDrawableViewLayerContext.hpp"

#include "Futurewalker.Test.TestPlatformDrawableViewLayer.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TestPlatformDrawableViewLayerContext : public PlatformDrawableViewLayerContext
{
public:
    auto MakeDrawableLayer() -> Shared<PlatformViewLayer> override
    {
        return TestPlatformDrawableViewLayer::Make();
    }
};

template <>
struct Locator::Resolver<TestPlatformDrawableViewLayerContext>
{
    using Interface = PlatformDrawableViewLayerContext;

    static auto Resolve()
    {
        return Shared<TestPlatformDrawableViewLayerContext>::Make();
    }
};
}
}
