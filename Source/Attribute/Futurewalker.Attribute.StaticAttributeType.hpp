// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.Prelude.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.StaticReference.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class T>
class StaticAttribute;

template <class T>
using StaticAttributeRef = StaticReference<StaticAttribute<T> const>;

#if FW_ENABLE_DEBUG
// For public attribute declarations and definitions.
#define FW_STATIC_ATTRIBUTE(type, attribute) static StaticAttribute<type> const attribute;
#define FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(attribute, reference) std::add_const_t<decltype(attribute)> attribute = decltype(attribute)::MakeWithDefaultReference<&reference>(#attribute)
#define FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(attribute, value) std::add_const_t<decltype(attribute)> attribute = decltype(attribute)::MakeWithDefaultValue(#attribute, value)
// For local attribute definitions.
#define FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(type, attribute, reference) static StaticAttribute<type> const attribute = StaticAttribute<type>::MakeWithDefaultReference<&reference>(#attribute)
#define FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(type, attribute, value) static StaticAttribute<type> const attribute = StaticAttribute<type>::MakeWithDefaultValue(#attribute, value)
#else
// For public attribute declarations and definitions.
#define FW_STATIC_ATTRIBUTE(type, attribute) static StaticAttribute<type> const attribute;
#define FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(attribute, reference) std::add_const_t<decltype(attribute)> attribute = decltype(attribute)::MakeWithDefaultReference<&reference>()
#define FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(attribute, value) std::add_const_t<decltype(attribute)> attribute = decltype(attribute)::MakeWithDefaultValue(value)
// For local attribute definitions.
#define FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(type, attribute, reference) static StaticAttribute<type> const attribute = StaticAttribute<type>::MakeWithDefaultReference<&reference>()
#define FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(type, attribute, value) static StaticAttribute<type> const attribute = StaticAttribute<type>::MakeWithDefaultValue(value)
#endif
}
}
