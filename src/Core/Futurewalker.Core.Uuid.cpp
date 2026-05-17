// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Core.Uuid.hpp"

#include <boost/uuid.hpp>

#include <cstring>

namespace FW_DETAIL_NS
{
///
/// @brief Make random UUID.
///
auto Uuid::MakeRandom() -> Uuid
{
    auto gen = boost::uuids::random_generator();
    auto const uuid = gen();
    auto result = Uuid();
    std::memcpy(result.data.data(), uuid.data(), 16);
    return result;
}

///
/// @brief Make UUID from string.
///
/// @param string ASCII string of UUID.
///
auto Uuid::MakeFromString(std::string_view const string) -> Optional<Uuid>
{
    try
    {
        auto const gen = boost::uuids::string_generator();
        auto const uuid = gen(string.begin(), string.end());
        auto result = Uuid();
        std::memcpy(result.data.data(), uuid.data(), 16);
        return result;
    }
    catch (...)
    {
        return {};
    }
}
}
