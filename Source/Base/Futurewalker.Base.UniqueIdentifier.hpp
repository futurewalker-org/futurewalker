// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.UniqueIdentifierType.hpp"
#include "Futurewalker.Base.UniqueIdentifierManager.hpp"

#include "Futurewalker.Core.Identifier.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Unique identifier class.
///
/// @param[in] Tag Tag type.
///
template <class Tag>
class UniqueIdentifier final : NonCopyable
{
    using ValueType = UInt64;
    using TagType = Tag;
    using IdentifierType = Identifier<ValueType, TagType>;

public:
    UniqueIdentifier() = default;

    ///
    /// @brief Implicit conversion to Identifier.
    ///
    operator IdentifierType() const noexcept
    {
        if (_id == 0U)
        {
            _id = UniqueIdentifierManager::template Generate<IdentifierType>();
        }
        return _id;
    }

private:
    mutable IdentifierType _id = 0U;
};

///
/// @brief Generic unique ID type.
///
using UniqueId = UniqueIdentifier<void>;
}
}
