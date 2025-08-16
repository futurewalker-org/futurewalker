// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.PlatformSystemInfo.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief macOS implementation of PlatformSystemInfo.
///
class PlatformSystemInfoMac final : public PlatformSystemInfo
{
public:
    auto IsWindows() const -> Bool override;
    auto IsUnix() const -> Bool override;
    auto IsMacOS() const -> Bool override;
    auto IsIOS() const -> Bool override;
    auto IsAndroid() const -> Bool override;
};

template <>
struct Locator::Resolver<PlatformSystemInfoMac>
{
    using Interface = PlatformSystemInfo;
    static auto Resolve() -> Shared<PlatformSystemInfoMac>;
};
}
}
