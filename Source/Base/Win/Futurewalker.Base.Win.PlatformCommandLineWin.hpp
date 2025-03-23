// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.PlatformCommandLine.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Win32 implementation of PlatformCommandLine.
///
class PlatformCommandLineWin final : public PlatformCommandLine
{
public:
    PlatformCommandLineWin();
    
    auto GetArguments() const -> std::vector<String> override;

private:
    std::vector<String> _args;
};

///
/// @brief Locator support.
///
template <>
struct Locator::Resolver<PlatformCommandLineWin>
{
    using Interface = PlatformCommandLine; 
    static auto Resolve() -> Shared<PlatformCommandLineWin>;
};
}
}
