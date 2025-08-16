// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.PlatformCommandLine.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief macOS implementation of PlatformCommandLine.
///
class PlatformCommandLineMac final : public PlatformCommandLine
{
public:
    PlatformCommandLineMac();
    
    auto GetArguments() const -> std::vector<String> override;

private:
    std::vector<String> _args;
};

template <>
struct Locator::Resolver<PlatformCommandLineMac>
{
    using Interface = PlatformCommandLine; 
    static auto Resolve() -> Shared<PlatformCommandLineMac>;
};
}
}
