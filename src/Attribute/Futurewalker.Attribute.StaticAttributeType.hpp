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

// Macro tricks for converting Att1, Att2, ... to &Att1, &Att2, ...
#define FW_IMPL_STATIC_ATTRIBUTE_EXPAND(x) x
#define FW_IMPL_STATIC_ATTRIBUTE_ADDR(x) &x
#define FW_IMPL_STATIC_ATTRIBUTE_FE_1(F, x) F(x)
#define FW_IMPL_STATIC_ATTRIBUTE_FE_2(F, x, ...) F(x), FW_IMPL_STATIC_ATTRIBUTE_EXPAND(FW_IMPL_STATIC_ATTRIBUTE_FE_1(F, __VA_ARGS__))
#define FW_IMPL_STATIC_ATTRIBUTE_FE_3(F, x, ...) F(x), FW_IMPL_STATIC_ATTRIBUTE_EXPAND(FW_IMPL_STATIC_ATTRIBUTE_FE_2(F, __VA_ARGS__))
#define FW_IMPL_STATIC_ATTRIBUTE_FE_4(F, x, ...) F(x), FW_IMPL_STATIC_ATTRIBUTE_EXPAND(FW_IMPL_STATIC_ATTRIBUTE_FE_3(F, __VA_ARGS__))
#define FW_IMPL_STATIC_ATTRIBUTE_FE_5(F, x, ...) F(x), FW_IMPL_STATIC_ATTRIBUTE_EXPAND(FW_IMPL_STATIC_ATTRIBUTE_FE_4(F, __VA_ARGS__))
#define FW_IMPL_STATIC_ATTRIBUTE_FE_6(F, x, ...) F(x), FW_IMPL_STATIC_ATTRIBUTE_EXPAND(FW_IMPL_STATIC_ATTRIBUTE_FE_5(F, __VA_ARGS__))
#define FW_IMPL_STATIC_ATTRIBUTE_FE_7(F, x, ...) F(x), FW_IMPL_STATIC_ATTRIBUTE_EXPAND(FW_IMPL_STATIC_ATTRIBUTE_FE_6(F, __VA_ARGS__))
#define FW_IMPL_STATIC_ATTRIBUTE_FE_8(F, x, ...) F(x), FW_IMPL_STATIC_ATTRIBUTE_EXPAND(FW_IMPL_STATIC_ATTRIBUTE_FE_7(F, __VA_ARGS__))
#define FW_IMPL_STATIC_ATTRIBUTE_GET_FE(_1,_2,_3,_4,_5,_6,_7,_8,NAME,...) NAME
#define FW_IMPL_STATIC_ATTRIBUTE_FOR_EACH(F, ...) FW_IMPL_STATIC_ATTRIBUTE_EXPAND(FW_IMPL_STATIC_ATTRIBUTE_GET_FE(__VA_ARGS__, FW_IMPL_STATIC_ATTRIBUTE_FE_8, FW_IMPL_STATIC_ATTRIBUTE_FE_7, FW_IMPL_STATIC_ATTRIBUTE_FE_6, FW_IMPL_STATIC_ATTRIBUTE_FE_5, FW_IMPL_STATIC_ATTRIBUTE_FE_4, FW_IMPL_STATIC_ATTRIBUTE_FE_3, FW_IMPL_STATIC_ATTRIBUTE_FE_2, FW_IMPL_STATIC_ATTRIBUTE_FE_1)(F, __VA_ARGS__))

#if FW_ENABLE_DEBUG
// For public attribute declarations and definitions.
#define FW_STATIC_ATTRIBUTE(type, attribute) static StaticAttribute<type> const attribute;
#define FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(attribute, reference) std::add_const_t<decltype(attribute)> attribute = decltype(attribute)::MakeWithDefaultReference<&reference>(#attribute)
#define FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(attribute, value) std::add_const_t<decltype(attribute)> attribute = decltype(attribute)::MakeWithDefaultValue(#attribute, value)
#define FW_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(attribute, func, ...) std::add_const_t<decltype(attribute)> attribute = decltype(attribute)::MakeWithDefaultFunction<FW_IMPL_STATIC_ATTRIBUTE_FOR_EACH(FW_IMPL_STATIC_ATTRIBUTE_ADDR, __VA_ARGS__)>(#attribute, func)
// For local attribute definitions.
#define FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(type, attribute, reference) static StaticAttribute<type> const attribute = StaticAttribute<type>::MakeWithDefaultReference<&reference>(#attribute)
#define FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(type, attribute, value) static StaticAttribute<type> const attribute = StaticAttribute<type>::MakeWithDefaultValue(#attribute, value)
#define FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(type, attribute, func, ...) static StaticAttribute<type> const attribute = StaticAttribute<type>::MakeWithDefaultFunction<FW_IMPL_STATIC_ATTRIBUTE_FOR_EACH(FW_IMPL_STATIC_ATTRIBUTE_ADDR, __VA_ARGS__)>(#attribute, func)
#else
// For public attribute declarations and definitions.
#define FW_STATIC_ATTRIBUTE(type, attribute) static StaticAttribute<type> const attribute;
#define FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(attribute, reference) std::add_const_t<decltype(attribute)> attribute = decltype(attribute)::MakeWithDefaultReference<&reference>()
#define FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(attribute, value) std::add_const_t<decltype(attribute)> attribute = decltype(attribute)::MakeWithDefaultValue(value)
#define FW_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(attribute, func, ...) std::add_const_t<decltype(attribute)> attribute = decltype(attribute)::MakeWithDefaultFunction<FW_IMPL_STATIC_ATTRIBUTE_FOR_EACH(FW_IMPL_STATIC_ATTRIBUTE_ADDR, __VA_ARGS__)>(func)
// For local attribute definitions.
#define FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(type, attribute, reference) static StaticAttribute<type> const attribute = StaticAttribute<type>::MakeWithDefaultReference<&reference>()
#define FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(type, attribute, value) static StaticAttribute<type> const attribute = StaticAttribute<type>::MakeWithDefaultValue(value)
#define FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(type, attribute, func, ...) static StaticAttribute<type> const attribute = StaticAttribute<type>::MakeWithDefaultFunction<FW_IMPL_STATIC_ATTRIBUTE_FOR_EACH(FW_IMPL_STATIC_ATTRIBUTE_ADDR, __VA_ARGS__)>(func)
#endif
}
}
