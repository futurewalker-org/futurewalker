// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.StringType.hpp"
#include "Futurewalker.Core.StringView.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.PointerIterator.hpp"

#include <compare>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Container of UTF-8 code units.
///
/// Copy-on-write implementation of UTF-8 string buffer.
/// It also supports small string optimization.
///
/// ### Creating `String` from UTF-8 string
///
/// If input buffer is UTF-8 but not represented by array of `char8_t`, use `StringFunction::Utf8ToString()` or `StringFunction::Utf8ToStringUnchecked()`.
/// 
/// ```cpp
/// auto const buffer = std::string(...);
/// auto const string = StringFunction::Utf8ToString(buffer);
/// ```
///
class String final
{
public:
    using ValueType = char8_t;
    using SizeType = SInt64;
    using IndexType = SInt64;

public:
    String() noexcept;
    ~String() noexcept;

    String(ValueType c) noexcept;
    String(Pointer<ValueType const> chars);
    String(Pointer<ValueType const> chars, SizeType size);
    String(ValueType const* chars);
    String(ValueType const* chars, SizeType size);
    String(StringView view);
    String(String const& other);
    String(String&& other) noexcept;

    auto operator=(String const& other) -> String&;
    auto operator=(String&& other) noexcept -> String&;

    template <Concepts::ContiguousIterator Iter>
    String(Iter begin, Iter end);

    template <Concepts::Integral T>
    String(const T&) = delete;

    String(std::nullptr_t) = delete;

    operator StringView() const noexcept;

    [[nodiscard]] auto operator==(String const& other) const noexcept -> bool;
    [[nodiscard]] auto operator<=>(String const& other) const noexcept -> std::strong_ordering;

    [[nodiscard]] auto IsEmpty() const noexcept -> Bool;
    [[nodiscard]] auto GetView() const noexcept -> StringView;
    [[nodiscard]] auto GetChar(ValueType& value, IndexType pos) const noexcept -> Bool;
    [[nodiscard]] auto GetSubstring(IndexType begin, IndexType end) const noexcept -> String;
    [[nodiscard]] auto GetCString() const -> std::u8string;

    auto Resize(SizeType size) -> void;
    auto Reserve(SizeType capacity) -> void;
    auto Clear() -> void;
    auto Erase(IndexType begin, IndexType end) -> void;
    auto Replace(IndexType begin, IndexType end, StringView str) -> void;
    auto Insert(IndexType pos, StringView str) -> void;
    auto Swap(String& other) noexcept -> void;

private:
    auto IsSmallString() const -> Bool;
    auto SetSize(SizeType size) -> void;
    auto GetSize() const noexcept -> SizeType;
    auto GetCapacity() const noexcept -> SizeType;
    auto GetConstData() const -> Pointer<const char8_t>;
    auto GetMutableData() -> Pointer<char8_t>;

private:
    struct LargeString
    {
        std::shared_ptr<char8_t[]> data; ///< Large string buffer.
        int64_t capacity = 0;            ///< Capacity.
        int64_t size = 0;                ///< Size.
        uint8_t padding[7];              ///< Padding (unused).
        uint8_t flags = 0;               ///< Flag bits.
    };

    struct SmallString
    {
        char8_t data[sizeof(LargeString) - 1]; ///< Small string buffer.
        uint8_t size = 0;                      ///< Flag and size bits.
    };

    struct Internal;
    static_assert(sizeof(LargeString) == sizeof(SmallString));

    union
    {
        SmallString _small; ///< Small string data.
        LargeString _large; ///< Large string data.
    };
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
