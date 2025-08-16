// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.U16StringType.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.StringView.hpp"

#include <memory>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Holds buffer of UTF-16 code units.
///
class U16String
{
public:
    using ValueType = char16_t;
    using SizeType = SInt64;
    using IndexType = SInt64;

    U16String() noexcept;
    U16String(ValueType const c);
    U16String(Pointer<ValueType const> chars);
    U16String(Pointer<ValueType const> chars, SizeType size);
    U16String(ValueType const* chars);
    U16String(ValueType const* chars, SizeType size);
    U16String(U16StringView view);
    U16String(U16String const& other);
    U16String(U16String&& other) noexcept;

    auto operator=(U16String const& other) -> U16String&;
    auto operator=(U16String&& other) noexcept -> U16String&;

    template <Concepts::ContiguousIterator Iter>
    U16String(Iter first, Iter last)
      : U16String(U16StringView(first, last))
    {
    }

    template <Concepts::Integral T>
    U16String(T const&) = delete;

    operator U16StringView() const noexcept;

    [[nodiscard]] auto operator==(U16String const& other) const noexcept -> bool;
    [[nodiscard]] auto operator<=>(U16String const& other) const noexcept -> std::strong_ordering;

    auto IsEmpty() const noexcept -> Bool;
    auto GetView() const noexcept -> U16StringView;
    auto GetChar(ValueType& value, IndexType pos) const noexcept -> Bool;
    auto GetSubString(IndexType begin, IndexType end) const noexcept -> U16String;

    auto Resize(SizeType const size) -> void;
    auto Reserve(SizeType const capacity) -> void;
    auto Replace(IndexType begin, IndexType end, U16StringView str) -> void;
    void Swap(U16String& other) noexcept;

private:
    auto GetImmutableData() const -> Pointer<ValueType const>;
    auto GetMutableData() -> Pointer<ValueType>;
    auto GetSize() const -> SizeType;
    auto GetCapacity() const -> SizeType;

    auto SetData(std::shared_ptr<ValueType[]> const& data, SizeType const capacity) -> void;
    auto SetSize(SizeType size) -> void;

private:
    std::shared_ptr<ValueType[]> _data;
    SizeType _size = 0;
    SizeType _capacity = 0;
};
}
}