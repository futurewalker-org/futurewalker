// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.StaticAttributeBaseType.hpp"
#include "Futurewalker.Attribute.AttributeValue.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.StaticPointer.hpp"
#include "Futurewalker.Core.Optional.hpp"

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
      StaticAttributeComputeFunction const& computeFunction,
      std::span<StaticAttributeBaseRef const> const references);

    auto GetId() const noexcept -> AttributeId;
    auto GetReferences() const noexcept -> std::vector<StaticAttributeBaseRef> const&;
    auto GetComputeFunction() const noexcept -> StaticAttributeComputeFunction const&;

private:
#if FW_ENABLE_DEBUG
    Pointer<char const> _name = nullptr;
#endif
    UniqueAttributeId const _uniqueId;
    StaticAttributeComputeFunction const _computeFunction;
    std::vector<StaticAttributeBaseRef> const _references;
};
}
