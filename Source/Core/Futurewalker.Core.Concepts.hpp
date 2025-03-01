// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Concepts.Prelude.hpp"

#include <concepts>
#include <type_traits>
#include <iterator>

namespace FW_CONCEPTS_DETAIL_NS
{
template <class T, template <class...> class P>
struct IsSpecializationOf : std::false_type
{
};

template <template <class...> class P, class... Args>
struct IsSpecializationOf<P<Args...>, P> : std::true_type
{
};

namespace FW_EXPORT
{
///
/// @brief Integral type concept.
///
template <class T>
concept Integral = std::integral<T>;

///
/// @brief Signed integral type concept.
///
template <class T>
concept SignedIntegral = std::signed_integral<T>;

///
/// @brief Unsigned integral type concept.
///
template <class T>
concept UnsignedIntegral = std::unsigned_integral<T>;

///
/// @brief Floating point type concept.
///
template <class T>
concept FloatingPoint = std::floating_point<T>;

///
/// @brief Arithmetic type concept.
///
template <class T>
concept Arithmetic = std::is_arithmetic_v<T>;

///
/// @brief Input iterator concept.
///
template <class Iter>
concept InputIterator = std::input_iterator<Iter>;

///
/// @brief Output iterator concept.
///
template <class Iter, class Type>
concept OutputIterator = std::output_iterator<Iter, Type>;

///
/// @brief Output iterator concept.
///
template <class Iter>
concept ContiguousIterator = std::contiguous_iterator<Iter>;

///
/// @brief same_as concept.
///
template <class T, class U>
concept SameAs = std::is_same_v<T, U>;

///
/// @brief pointer concept.
///
template <class T>
concept Pointer = std::is_pointer_v<T>;

///
/// @brief array concept.
///
template <class T>
concept Array = std::is_array_v<T>;

///
/// @brief Non-void type concept.
///
template <class T>
concept NonVoid = (!std::is_void_v<T>);

///
/// @brief derived_from concept.
///
template <class Derived, class Base>
concept DerivedFrom = std::derived_from<Derived, Base>;

///
/// @brief Check if a type can be implicitly convertible to another type.
///
template <class From, class To>
concept ImplicitlyConvertibleTo = std::convertible_to<From, To>;

///
/// @brief Check if a type can be explicitly convertible to another type.
///
template <class From, class To>
concept ExplicitlyConvertibleTo = requires(From&& v) { static_cast<To>(std::forward<From>(v)); };

///
/// @brief Check if a type can be convertible into another type without losing information.
///
/// @note This concept treats some conversions differently from `brace-init-list`.
/// For example, narrowing conversion from non-`constexpr` `int` to `double` is not allowed even when `double` can represent all possible values of `int`.
/// This concept treats such conversions as valid cases for narrowing conversions.
///
/// See "[P0870R4] A proposal for a type trait to detect narrowing conversions" for more discussion.
///
template <class From, class To>
concept NarrowConvertibleTo =
// clang-format off
    ImplicitlyConvertibleTo<From, To> && (
      (requires(From&& v) { { std::type_identity_t<To[]>{std::forward<From>(v)} } -> SameAs<To[1]>; }) || // see P0870R4
      (Integral<From> && FloatingPoint<To> && (std::numeric_limits<To>::digits >= std::numeric_limits<From>::digits))
    );
// clang-format on

///
/// @brief constructible_from concept.
///
template <class To, class From>
concept ConstructibleFrom = std::constructible_from<To, From>;

///
/// @brief Check if a type can be default constructible.
///
template <class T>
concept DefaultConstructible = std::is_default_constructible_v<T>;

///
/// @brief assignable_from concept.
///
template <class To, class From>
concept AssignableFrom = std::assignable_from<To, From>;

///
/// @brief Constructible from at least one of reference types.
///
template <class To, class From>
concept ConstructibleFromAnyReferenceOf =      //
  std::destructible<To> &&                     //
  (std::is_constructible_v<To, From&> ||       //
   std::is_constructible_v<To, const From&> || //
   std::is_constructible_v<To, From&&> ||      //
   std::is_constructible_v<To, const From&&>); //

///
/// @brief Convertible from at least one of reference types.
///
template <class To, class From>
concept ConvertibleFromAnyReferenceOf =     //
  std::is_convertible_v<From&, To> ||       //
  std::is_convertible_v<const From&, To> || //
  std::is_convertible_v<From&&, To> ||      //
  std::is_convertible_v<const From&&, To>;  //

///
/// @brief Assignable from at least one of reference types.
///
template <class To, class From>
concept AssignableFromAnyReferenceOf =     //
  std::is_assignable_v<To, From&> ||       //
  std::is_assignable_v<To, const From&> || //
  std::is_assignable_v<To, From&&> ||      //
  std::is_assignable_v<To, const From&&>;  //

///
/// @brief Invocable concept.
///
template <class T, class... Args>
concept Invocable = std::invocable<T, Args...>;

///
/// @brief Check if given T is specialization of template P.
///
template <class T, template <class...> class P>
concept SpecializationOf = IsSpecializationOf<T, P>::value;
}
}
