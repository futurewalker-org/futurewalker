// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.StringType.hpp"
#include "Futurewalker.Core.StringData.hpp"
#include "Futurewalker.Core.StringUnit.hpp"
#include "Futurewalker.Core.StringView.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.PointerIterator.hpp"

#include <compare>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Container of Unicode characters.
///
/// Holds unicode code points encoded in valid sequence of UTF-8 code units.
/// Designed to be cheap to copy with copy-on-write and small string optimization.
///
class String
{
public:
    using IndexType = CodeUnit;
    using ValueType = char8_t;
    using CharType = char32_t;
    using SizeType = SInt64;

    static auto MakeFromStdString(std::string_view const sv) -> String;
    static auto MakeFromStdU8String(std::u8string_view const sv) -> String;

    String() noexcept = default;
    String(String const& other) noexcept = default;
    String(String&& other) noexcept = default;
    auto operator=(String const& other) noexcept -> String& = default;
    auto operator=(String&& other) noexcept -> String& = default;

    String(CharType c) noexcept;
    String(ValueType v) noexcept;
    String(Pointer<ValueType const> chars);
    String(Pointer<ValueType const> chars, SizeType size);
    String(ValueType const* chars);
    String(ValueType const* chars, SizeType size);
    String(StringView view);

    template <Concepts::ContiguousIterator Iter>
    String(Iter begin, Iter end);

    template <Concepts::Integral T>
    String(T const&) = delete;

    String(std::nullptr_t) = delete;

    operator StringView() const noexcept;

    auto operator==(String const& other) const noexcept -> bool;
    auto operator<=>(String const& other) const noexcept -> std::strong_ordering;

    auto IsEmpty() const noexcept -> Bool;
    auto GetBegin() const -> IndexType;
    auto GetEnd() const -> IndexType;
    auto GetNext(IndexType const& index, SInt64 const offset = 1) const -> IndexType;
    auto GetPrev(IndexType const& index, SInt64 const offset = 1) const -> IndexType;
    auto GetChar(IndexType const& index, CharType& value) const -> Bool;
    auto GetChar(IndexType const& index, CharType& value, IndexType& next) const -> Bool;
    auto GetSubString(IndexType const& begin, IndexType const& end) const noexcept -> String;

    auto GetView() const noexcept -> StringView;
    auto Clear() -> void;
    auto Erase(IndexType const& begin, IndexType const& end) -> void;
    auto Replace(IndexType const& begin, IndexType const& end, String const& string) -> void;
    auto Insert(IndexType const& position, String const& string) -> void;
    auto Append(String const& string) -> void;
    auto Swap(String& other) noexcept -> void;

    auto ToStdString() const -> std::string;
    auto ToStdU8String() const -> std::u8string;

private:
    String(StringData<ValueType>&& data) noexcept;
    StringData<ValueType> _data;
};

///
/// @brief Construct String from pair of input iterators.
///
template <Concepts::ContiguousIterator Iter>
String::String(Iter begin, Iter end)
  : String(StringView(begin, end))
{
}
}
}
