// SPDX-License-Identifier: MIT
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
    AttributeValue(const AttributeValue& other) noexcept;

    ///
    /// @brief Create value by copying from source.
    ///
    template <class T>
    explicit AttributeValue(const T& value)
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
        if (const auto ptr = _holder.Maybe<const Holder<T>>())
        {
            return ptr->value;
        }
        return {};
    }

    auto operator=(const AttributeValue& other) noexcept -> AttributeValue&;
    auto operator==(const AttributeValue& other) const noexcept -> bool;

private:
    struct HolderBase
    {
        virtual ~HolderBase() = default;
        virtual auto EqualsTo(const Shared<const HolderBase>& other) const -> bool = 0;
    };

    template <class T>
    struct Holder final : public HolderBase
    {
        T value;

        Holder(const T& value)
          : value {value}
        {
        }

        auto EqualsTo(const Shared<const HolderBase>& other) const -> bool override
        {
            if (const auto otherPtr = other.Maybe<const Holder<T>>())
            {
                return value == otherPtr->value;
            }
            return false;
        }
    };

    Shared<const HolderBase> _holder;
};
}
}
