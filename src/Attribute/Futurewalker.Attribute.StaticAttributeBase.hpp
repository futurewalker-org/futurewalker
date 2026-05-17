// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.StaticAttributeBaseType.hpp"
#include "Futurewalker.Attribute.AttributeComputeFunction.hpp"
#include "Futurewalker.Attribute.AttributeValue.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.StaticPointer.hpp"
#include "Futurewalker.Core.Optional.hpp"

#include <boost/container/small_vector.hpp>

namespace FW_DETAIL_NS
{
///
/// @brief Base class of attribute description.
///
class StaticAttributeBase : NonCopyable
{
public:
    StaticAttributeBase() = delete;

    StaticAttributeBase(
#if FW_ENABLE_DEBUG
      Pointer<char const> str,
#endif
      AttributeComputeFunction const& computeFunction,
      std::span<StaticAttributeBaseRef const> const references);

    auto GetId() const noexcept -> AttributeId;
    auto GetReferences() const noexcept -> boost::container::small_vector<StaticAttributeBaseRef, 4> const&;
    auto GetComputeFunction() const noexcept -> AttributeComputeFunction const&;

#if FW_ENABLE_DEBUG
    auto GetName() const noexcept -> Pointer<char const>
    {
        return _name;
    }
#endif

private:
#if FW_ENABLE_DEBUG
    Pointer<char const> _name = nullptr;
#endif
    UniqueAttributeId const _uniqueId;
    AttributeComputeFunction const _computeFunction;
    boost::container::small_vector<StaticAttributeBaseRef, 4> const _references;
};
}
