// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Test.TestPlatformViewLayer.hpp"

#include "Futurewalker.Application.PlatformDrawableViewLayerControl.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TestPlatformDrawableViewLayerControl : public PlatformDrawableViewLayerControl
{
    auto Draw(Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& offset, Float64 const scale) -> Bool override
    {
        (void)displayList;
        (void)offset;
        (void)scale;
        FW_DEBUG_LOG_INFO("TestPlatformDrawableViewLayerControl::Draw() called");
        return true;
    }
};

class TestPlatformDrawableViewLayer : public TestPlatformViewLayer
{
public:
    Shared<TestPlatformDrawableViewLayerControl> _control;

    static auto Make()
    {
        return PlatformViewLayer::MakeDerived<TestPlatformDrawableViewLayer>();
    }

    TestPlatformDrawableViewLayer(PassKey<PlatformViewLayer> key)
      : TestPlatformViewLayer(key)
    {
    }

    auto GetControl() -> Shared<PlatformViewLayerControl> override
    {
        return _control;
    }
};
}
}
