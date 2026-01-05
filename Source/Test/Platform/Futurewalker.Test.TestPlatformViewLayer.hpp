// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayer.hpp"

#include <ranges>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TestPlatformViewLayer : public PlatformViewLayer
{
public:
    static auto Make() -> Shared<TestPlatformViewLayer>
    {
        return PlatformViewLayer::MakeDerived<TestPlatformViewLayer>();
    }

    TestPlatformViewLayer(PassKey<PlatformViewLayer> key)
      : PlatformViewLayer(key)
    {
    }
};
}
}
