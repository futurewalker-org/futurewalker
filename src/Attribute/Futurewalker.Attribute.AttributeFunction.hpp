// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.StaticAttributeType.hpp"
#include "Futurewalker.Attribute.StaticAttributeBaseType.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

#include <utility>

namespace FW_DETAIL_NS
{
struct AttributeFunction : NonConstructible
{
public:
    ///
    /// @brief Make compute function for callable object.
    ///
    /// @tparam R Return type of function.
    /// @tparam Ts Parameter types of function.
    ///
    /// @param f Function object.
    ///
    template <class R, class... Ts, class F>
    static auto MakeComputeFunction(F&& f) -> StaticAttributeComputeFunction
    {
        using types = std::tuple<Ts...>;
        static auto constexpr indices = std::make_index_sequence<sizeof...(Ts)>();
        static auto constexpr mapper = []<size_t... Seq>(std::index_sequence<Seq...>, auto& args, auto& f) { return f(*args[Seq].template GetValue<std::tuple_element_t<Seq, types>>()...); };
        return [f = std::forward<F>(f)](auto const args) { return AttributeValue(R(mapper(indices, args, f))); };
    }
};
}
