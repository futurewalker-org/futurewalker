// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.PlatformDebug.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include <iostream>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TestPlatformDebug : public PlatformDebug
{
public:
    auto Print(StringView const message) -> void override
    {
        auto const str = StringFunction::ConvertStringToStdString(message);
        std::cerr << str << std::endl;
    }

    auto Break() -> void override
    {
        Print(u8"TestPlatformDebugTest::Break() called");
    }
};

template <>
struct Locator::Resolver<TestPlatformDebug>
{
    using Interface = PlatformDebug;
    static auto Resolve() -> Shared<PlatformDebug>
    {
        return Shared<TestPlatformDebug>::Make();
    }
};
}
}
