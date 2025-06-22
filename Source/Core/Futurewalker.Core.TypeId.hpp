// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Identifier.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Type identifier.
///
class TypeId : public Identifier<UIntPtr, TypeId>
{
public:
    using Identifier<UIntPtr, TypeId>::Identifier;

    ///
    /// @brief Get type identifier for given type.
    ///
    /// @tparam T Type.
    ///
    template <class T>
    static auto Get() noexcept -> TypeId
    {
        // HACK: Relying on uniqueness of address of template functions.
        return TypeId(reinterpret_cast<std::uintptr_t>(Get<std::remove_cvref_t<T>>));
    }
};
}
}

template <>
struct std::hash<FW_NS::TypeId> : std::hash<FW_NS::Identifier<FW_NS::UIntPtr, FW_NS::TypeId>>
{
};

template <>
struct boost::hash<FW_NS::TypeId> : boost::hash<FW_NS::Identifier<FW_NS::UIntPtr, FW_NS::TypeId>>
{
};
