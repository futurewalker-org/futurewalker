// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.Mac.Prelude.hpp"
#include "Futurewalker.Base.PlatformLocaleContext.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Mac implementation of PlatformLocaleContext.
///
class PlatformLocaleContextMac : public PlatformLocaleContext
{
    auto GetPreferredUILocaleStrings() -> std::vector<String> override;
};

template <>
struct Locator::Resolver<PlatformLocaleContextMac>
{
    using Interface = PlatformLocaleContext;
    static auto Resolve() -> Shared<PlatformLocaleContextMac>;
};
}
}
