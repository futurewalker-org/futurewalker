// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.UniqueIdentifierManagerType.hpp"
#include "Futurewalker.Base.UniqueIdentifierType.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.Identifier.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

#include <atomic>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Unique identifier manager.
///
/// Manages unique IDs used in current process.
///
/// @note Do not directly use this interface in your application, use UniqueIdentifier instead.
///
class UniqueIdentifierManager : NonCopyable
{
public:
    template <class IdentifierType>
    static auto Generate() -> IdentifierType;

private:
    auto GetNext() -> UInt64;
};

///
/// @brief 
///
template <>
struct Locator::Resolver<UniqueIdentifierManager>
{
    using Interface = UniqueIdentifierManager;
    static auto Resolve() -> Shared<UniqueIdentifierManager>;
};

///
/// @brief Generate new identifier value.
///
/// @tparam Identifier Identifier type to generate.
///
template <class IdentifierType>
auto UniqueIdentifierManager::Generate() -> IdentifierType
{
    if (auto const manager = Locator::GetInstance<UniqueIdentifierManager>())
    {
        return IdentifierType(manager->GetNext());
    }
    throw Exception(ErrorCode::Failure);
}
}
}
