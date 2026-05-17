// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.UuidType.hpp"

#include "Futurewalker.Core.Optional.hpp"

#include <array>

#include <string_view>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief UUID data type.
///
struct Uuid
{
    static auto MakeRandom() -> Uuid;
    static auto MakeFromString(std::string_view const string) -> Optional<Uuid>;

    std::array<uint8_t, 16> data = {};
};
}
}
