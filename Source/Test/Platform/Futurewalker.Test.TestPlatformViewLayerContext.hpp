// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformViewLayerContext.hpp"
#include "Futurewalker.Test.TestPlatformViewLayer.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TestPlatformViewLayerContext : public PlatformViewLayerContext
{
public:
    auto MakeViewLayer() -> Shared<PlatformViewLayer> override
    {
        return TestPlatformViewLayer::Make();
    }
};

template <>
struct Locator::Resolver<TestPlatformViewLayerContext>
{
    using Interface = PlatformViewLayerContext;

    static auto Resolve()
    {
        return Shared<TestPlatformViewLayerContext>::Make();
    }
};
}
}
