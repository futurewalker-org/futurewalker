// SPDX-License-Identifier: MPL-2.0
#include "Futurewalker.Base.UniqueIdentifierManager.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Get next identifier number.
///
/// @note Do not assume that return values monotonically increase.
///
auto UniqueIdentifierManager::GetNext() -> UInt64
{
    // We support UniqueIdentifiers with static storage duration.
    // So we cannot reset current number when UniqueIdentifierManager was recreated.
    static std::atomic<uint64_t> _current = 0U;
    return _current.fetch_add(1) + 1;
}

auto Locator::Resolver<UniqueIdentifierManager>::Resolve() -> Shared<UniqueIdentifierManager>
{
    return Shared<UniqueIdentifierManager>::Make();
}
}
