// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.Win.Prelude.hpp"
#include "Futurewalker.Base.PlatformLocaleContext.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Windows implementation of PlatformLocaleContext.
///
class PlatformLocaleContextWin : public PlatformLocaleContext
{
    auto GetPreferredUILocaleStrings() -> std::vector<String> override;
};

template <>
struct Locator::Resolver<PlatformLocaleContextWin>
{
    using Interface = PlatformLocaleContext;
    static auto Resolve() -> Shared<PlatformLocaleContextWin>;
};
}
}
