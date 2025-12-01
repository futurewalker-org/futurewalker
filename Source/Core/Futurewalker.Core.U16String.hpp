// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.U16StringType.hpp"
#include "Futurewalker.Core.StringData.hpp"
#include "Futurewalker.Core.StringUnit.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.StringView.hpp"

#include <memory>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Container of Unicode characters.
///
/// Holds unicode code points encoded in valid sequence of UTF-16 code units.
/// Designed to be cheap to copy with copy-on-write and small string optimization.
///
class U16String
{
public:
    using IndexType = CodeUnit;
    using ValueType = char16_t;
    using CharType = char32_t;
    using SizeType = SInt64;

    U16String() noexcept = default;
    U16String(U16String const& other) noexcept = default;
    U16String(U16String&& other) noexcept = default;
    auto operator=(U16String const& other) noexcept -> U16String& = default;
    auto operator=(U16String&& other) noexcept -> U16String& = default;

    U16String(CharType const c);
    U16String(ValueType const v);
    U16String(Pointer<ValueType const> chars);
    U16String(Pointer<ValueType const> chars, SizeType size);
    U16String(ValueType const* chars);
    U16String(ValueType const* chars, SizeType size);
    U16String(U16StringView view);

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
    auto GetBegin() const -> IndexType;
    auto GetEnd() const -> IndexType;
    auto GetNext(IndexType const& index, SInt64 const offset = 1) const -> IndexType;
    auto GetPrev(IndexType const& index, SInt64 const offset = 1) const -> IndexType;
    auto GetChar(IndexType const& index, CharType& value) const -> Bool;
    auto GetChar(IndexType const& index, CharType& value, IndexType& next) const -> Bool;
    auto GetSubString(IndexType const& begin, IndexType const& end) const noexcept -> U16String;

    auto GetView() const noexcept -> U16StringView;
    auto Clear() -> void;
    auto Replace(IndexType const& begin, IndexType const& end, U16String const& str) -> void;
    auto Insert(IndexType const& index, U16String const& str) -> void;
    auto Append(U16String const& string) -> void;
    auto Swap(U16String& other) noexcept -> void;

private:
    U16String(StringData<ValueType> && data) noexcept;
    StringData<ValueType> _data;
};
}
}