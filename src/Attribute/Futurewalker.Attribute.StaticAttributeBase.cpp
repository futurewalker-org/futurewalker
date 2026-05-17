// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.StaticAttributeBase.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
#if FW_ENABLE_DEBUG
///
/// @brief
///
/// @param value
///
StaticAttributeBase::StaticAttributeBase(Pointer<char const> str, AttributeComputeFunction const& computeFunction, std::span<StaticAttributeBaseRef const> const references)
  : _name {str}
  , _computeFunction {computeFunction}
  , _references {references.begin(), references.end()}
{
    FW_DEBUG_ASSERT(_computeFunction);
}
#else
StaticAttributeBase::StaticAttributeBase(AttributeComputeFunction const& computeFunction, std::span<StaticAttributeBaseRef const> const references)
  : _computeFunction {computeFunction}
  , _references {references.begin(), references.end()}
{
    FW_DEBUG_ASSERT(_computeFunction);
}
#endif

///
/// @brief
///
/// @return
///
auto StaticAttributeBase::GetId() const noexcept -> AttributeId
{
    return static_cast<AttributeId>(_uniqueId);
}

auto StaticAttributeBase::GetReferences() const noexcept -> boost::container::small_vector<StaticAttributeBaseRef, 4> const&
{
    return _references;
}

auto StaticAttributeBase::GetComputeFunction() const noexcept -> AttributeComputeFunction const&
{
    return _computeFunction;
}
}
