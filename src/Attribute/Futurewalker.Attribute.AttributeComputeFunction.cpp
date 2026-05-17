// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.AttributeComputeFunction.hpp"

#include "Futurewalker.Core.Overloaded.hpp"

namespace FW_DETAIL_NS
{
auto AttributeComputeFunction::MakeValue(AttributeValue const& value) -> AttributeComputeFunction
{
    return AttributeComputeFunction(value);
}

auto AttributeComputeFunction::MakeFunction(AttributeValue (*f)(std::span<AttributeValue const> const references)) -> AttributeComputeFunction
{
    return AttributeComputeFunction(
      ComputeFunction {
          .original = f,
          .wrapper = [](void* original, std::span<AttributeValue const> const references) -> AttributeValue { return static_cast<decltype(f)>(original)(references); },
      });
}

AttributeComputeFunction::operator bool() const noexcept
{
    return std::visit(
      Overloaded {
          [](ComputeFunction const& func) { return static_cast<bool>(func.original); },
          [](AttributeValue const&) { return true; },
          [](Blank const&) { return false; },
      },
      _var);
}

auto AttributeComputeFunction::operator()(std::span<AttributeValue const> const references) const -> AttributeValue
{
    return std::visit(
      Overloaded {
          [&](ComputeFunction const& func) {
              if (func.original && func.wrapper)
              {
                  return func.wrapper(func.original, references);
              }
              throw Exception(ErrorCode::InvalidOperation);
          },
          [](AttributeValue const& value) { return value; },
          [](Blank const&) -> AttributeValue { throw Exception(ErrorCode::InvalidOperation); },
      },
      _var);
}

auto AttributeComputeFunction::operator==(AttributeComputeFunction const& other) const -> bool
{
    return _var == other._var;
}

auto AttributeComputeFunction::ComputeFunction::operator==(ComputeFunction const& other) const noexcept -> bool
{
    return original == other.original;
}

AttributeComputeFunction::AttributeComputeFunction(ComputeFunction const& computeFunction)
  : _var {computeFunction}
{
}

AttributeComputeFunction::AttributeComputeFunction(AttributeValue const& value)
  : _var {value}
{
}

}
