// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeValueType.hpp"
#include "Futurewalker.Attribute.AttributeId.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Optional.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Type erased, copy-on-write value container for attributes.
///
class AttributeValue
{
public:
    AttributeValue() = delete;
    AttributeValue(AttributeValue const& other) noexcept;

    ///
    /// @brief Create value by copying from source.
    ///
    template <class T>
    explicit AttributeValue(T const& value)
    {
        _holder = Shared<Holder<T>>::Make(value);
    }

    ///
    /// @brief Get contained value.
    ///
    /// @tparam T Type of value to get
    ///
    /// @return Non-null value when contained value matches given type.
    ///
    template <class T>
    auto GetValue() const -> Optional<T>
    {
        if (auto const ptr = _holder.TryAs<Holder<T> const>())
        {
            return ptr->value;
        }
        return {};
    }

    auto operator=(AttributeValue const& other) noexcept -> AttributeValue&;
    auto operator==(AttributeValue const& other) const noexcept -> bool;

private:
    struct HolderBase
    {
        virtual ~HolderBase() = default;
        virtual auto EqualsTo(Shared<HolderBase const> const& other) const -> bool = 0;
    };

    template <class T>
    struct Holder final : public HolderBase
    {
        T value;

        Holder(T const& value)
          : value {value}
        {
        }

        auto EqualsTo(Shared<HolderBase const> const& other) const -> bool override
        {
            if (auto const otherPtr = other.TryAs<Holder<T> const>())
            {
                return value == otherPtr->value;
            }
            return false;
        }
    };

    Shared<HolderBase const> _holder;
};
}
}
