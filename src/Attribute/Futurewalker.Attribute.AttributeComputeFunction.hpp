// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeValue.hpp"

#include "Futurewalker.Core.Blank.hpp"

#include <variant>
#include <span>

namespace FW_DETAIL_NS
{
///
/// @brief Function object for slot compute function.
///
/// Contains either simple function returning fixed AttributeValue or arbitrary stateless function.
///
class AttributeComputeFunction
{
public:
    AttributeComputeFunction() = default;

    template <class T, class... References, class F>
    static auto MakeFunctionWrapper(F f) -> AttributeComputeFunction;
    static auto MakeFunction(AttributeValue (*f)(std::span<AttributeValue const> const references)) -> AttributeComputeFunction;
    static auto MakeValue(AttributeValue const& value) -> AttributeComputeFunction;

    explicit operator bool() const noexcept;

    auto operator()(std::span<AttributeValue const> const references) const -> AttributeValue;

    auto operator==(AttributeComputeFunction const& other) const -> bool;
    auto operator!=(AttributeComputeFunction const& other) const -> bool = default;

private:
    struct ComputeFunction
    {
        void (*original)() = nullptr;
        AttributeValue (*wrapper)(void(*original)(), std::span<AttributeValue const> const references) = nullptr;
        auto operator==(ComputeFunction const& other) const noexcept -> bool;
    };
    explicit AttributeComputeFunction(ComputeFunction const& computeFunction);
    explicit AttributeComputeFunction(AttributeValue const& value);
    std::variant<Blank, ComputeFunction, AttributeValue> _var;
};

///
/// @brief Wraps arbitrary stateless function as compute function.
///
/// @param f Stateless object which can be converted to function pointer.
///
template <class T, class... References, class F>
auto AttributeComputeFunction::MakeFunctionWrapper(F f) -> AttributeComputeFunction
{
    using types = std::tuple<References...>;
    static auto constexpr indices = std::make_index_sequence<sizeof...(References)>();
    static auto constexpr argumentMapper = []<size_t... Seq>(std::index_sequence<Seq...>, auto& args, auto f) { return f(*args[Seq].template GetValue<std::tuple_element_t<Seq, types>>()...); };

    // You might have passed generic lambda object to this function, which is unsupported.
    // `f` needs to be stateless and convertible to function pointer.
    auto const fp = +f;

    return AttributeComputeFunction(
      ComputeFunction {
          .original = reinterpret_cast<void(*)()>(fp),
          .wrapper = [](void (*original)(), std::span<AttributeValue const> const args) { return AttributeValue(T(argumentMapper(indices, args, reinterpret_cast<decltype(fp)>(original)))); },
      });
}
}
