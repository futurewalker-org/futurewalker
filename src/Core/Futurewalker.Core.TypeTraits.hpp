// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.TypeTraits.Prelude.hpp"

#include <type_traits>

namespace FW_TYPE_TRAITS_DETAIL_NS
{
///
/// @brief Apply type function if condition is satisfied.
///
/// @tparam Cond Condition.
/// @tparam Function Type function to apply.
/// @tparam T Argument for type funtion.
///
template <auto Cond, template <class> class Function, class T>
using ApplyIf = std::conditional_t<Cond, Function<T>, T>;

///
/// @brief CVRef type traits.
///
template <class T>
struct CVRefTrait
{
    using NoRef = std::remove_reference_t<T>;
    using NoCVRef = std::remove_cvref_t<T>;

    static constexpr bool IsConst = std::is_const_v<NoRef>;
    static constexpr bool IsVolatile = std::is_volatile_v<NoRef>;
    static constexpr bool IsLValue = std::is_lvalue_reference_v<T>;
    static constexpr bool IsRValue = std::is_rvalue_reference_v<T>;

    using NoCV = ApplyIf<IsLValue, std::add_lvalue_reference_t, ApplyIf<IsRValue, std::add_rvalue_reference_t, NoCVRef>>;
};

///
/// @brief CVRef type functions.
///
template <class From, class To>
struct CVRefFunction
{
    using FromNoRef = typename CVRefTrait<From>::NoRef;
    using ToNoRef = typename CVRefTrait<To>::NoRef;
    using ToNoCVRef = typename CVRefTrait<To>::NoCVRef;

    static constexpr bool IsFromConst = CVRefTrait<From>::IsConst;
    static constexpr bool IsFromVolatile = CVRefTrait<From>::IsVolatile;
    static constexpr bool IsFromLValue = CVRefTrait<From>::IsLValue;
    static constexpr bool IsFromRValue = CVRefTrait<From>::IsRValue;
    static constexpr bool IsToLValue = CVRefTrait<To>::IsLValue;
    static constexpr bool IsToRValue = CVRefTrait<To>::IsRValue;

    using PropagateRef = ApplyIf<IsFromLValue, std::add_lvalue_reference_t, ApplyIf<IsFromRValue, std::add_rvalue_reference_t, ToNoRef>>;

    using PropagateCVRef = ApplyIf<
      IsFromLValue,
      std::add_lvalue_reference_t,
      ApplyIf<IsFromRValue, std::add_rvalue_reference_t, ApplyIf<IsFromConst, std::add_const_t, ApplyIf<IsFromVolatile, std::add_volatile_t, ToNoCVRef>>>>;

    using PropagateCV = ApplyIf<
      IsToLValue,
      std::add_lvalue_reference_t,
      ApplyIf<IsToRValue, std::add_rvalue_reference_t, ApplyIf<IsFromConst, std::add_const_t, ApplyIf<IsFromVolatile, std::add_volatile_t, ToNoCVRef>>>>;
};

namespace FW_EXPORT
{
///
/// @brief Detects complete/incomplete type at instantiation point.
///
/// @tparam T Argument type.
///
template <class T, auto L = []() -> void {}>
constexpr bool IsComplete = requires
{
    sizeof(T);
    L;
};

///
/// @brief Checks const qualifier of given type.
///
/// @tparam T Argument type.
///
/// @note Unlike std::is_const_v, returns true for const references.
///
template <class T>
constexpr bool IsConst = CVRefTrait<T>::IsConst;

///
/// @brief Checks volatile qualifier of given type.
///
/// @tparam T Argument type.
///
/// @note Unlike std::is_const_v, returns true for volatile references.
///
template <class T>
constexpr bool IsVolatile = CVRefTrait<T>::IsVolatile;

///
/// @brief Remove CV qualifier from given type.
///
/// @tparam T Argument type.
///
/// @note Unlike standard type functions, this function can remove cv-qualifier from references.
///
template <class T>
using RemoveCV = typename CVRefTrait<T>::NoCV;

///
/// @brief Propagates reference From to To. 
///
template <class From, class To>
using PropagateRef = typename CVRefFunction<From, To>::PropagateRef;

///
/// @brief Propagates cv-qualifier From to To.
///
template <class From, class To>
using PropagateCV = typename CVRefFunction<From, To>::PropagateCV;

///
/// @brief Propagates cv-qualifier and reference From to To.
///
template <class From, class To>
using PropagateCVRef = typename CVRefFunction<From, To>::PropagateCVRef;
}
}
