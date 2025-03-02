// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.StringViewType.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.PointerIterator.hpp"

#include <compare>
#include <string_view>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief View on array of UTF8 code units.
///
/// Lightweight non-owning view for UTF8 strings.
/// Contains pair of (usually non null-terminated) buffer and its length.
///
/// ### Recommendations
///
/// This class is mostly used for interfacing with literal strings or external libraries.
/// It is recommended to use String for most of the cases.
///
/// StringView should only be used for transient objects like function arguments.
/// Returning StringView from functions can lead to dangling pointers thus not considered safe.
///
class StringView final
{
public:
    using ValueType = char8_t;
    using SizeType = SInt64;
    using IndexType = SInt64;
    using Pointer = FW_NS::Pointer<ValueType>;
    using ConstPointer = FW_NS::Pointer<const ValueType>;
    using Reference = ValueType&;
    using ConstReference = const ValueType&;
    using ConstIterator = ConstPointerIterator<ValueType, StringView>;
    using Iterator = ConstIterator;

public:
    constexpr StringView() noexcept = default;
    constexpr StringView(const StringView&) noexcept = default;
    constexpr StringView(ConstPointer str) noexcept;
    constexpr StringView(ConstPointer str, SizeType len) noexcept;
    constexpr StringView(const ValueType* str) noexcept;
    constexpr StringView(const ValueType* str, SizeType len) noexcept;

    template <Concepts::Integral T>
    StringView(const T&) = delete;

    StringView(std::nullptr_t) = delete;

    template <Concepts::ContiguousIterator It>
    constexpr StringView(It begin, It end);

    [[nodiscard]] constexpr auto operator[](IndexType index) const -> ConstReference;
    [[nodiscard]] constexpr auto operator==(const StringView& other) const noexcept -> bool;
    [[nodiscard]] constexpr auto operator<=>(const StringView& other) const noexcept -> std::strong_ordering;

    [[nodiscard]] constexpr auto begin() const noexcept -> Iterator;
    [[nodiscard]] constexpr auto cbegin() const noexcept -> ConstIterator;
    [[nodiscard]] constexpr auto end() const noexcept -> Iterator;
    [[nodiscard]] constexpr auto cend() const noexcept -> ConstIterator;

    [[nodiscard]] constexpr auto IsEmpty() const noexcept -> Bool;
    [[nodiscard]] constexpr auto GetSize() const noexcept -> SizeType;
    [[nodiscard]] constexpr auto GetData() const noexcept -> ConstPointer;

    [[nodiscard]] constexpr auto GetSubstring(IndexType begin, IndexType end) const noexcept -> StringView;
    [[nodiscard]] constexpr auto Contains(const StringView& str) const noexcept -> Bool;
    [[nodiscard]] constexpr auto Count(const StringView& str) const noexcept -> SizeType;
    [[nodiscard]] constexpr auto StartsWith(const StringView& str) const noexcept -> Bool;
    [[nodiscard]] constexpr auto EndsWith(const StringView& str) const noexcept -> Bool;

private:
    ConstPointer _data = nullptr;
    SInt64 _size = 0;
};

///
/// @brief Construct StringView from null-terminated string.
///
constexpr StringView::StringView(ConstPointer str) noexcept
  : _data {str}
  , _size {std::char_traits<char8_t>::length(static_cast<const char8_t*>(str))}
{
}

///
/// @brief Construct StringView from char span.
///
constexpr StringView::StringView(ConstPointer str, SizeType len) noexcept
  : _data {str}
  , _size {len}
{
}

///
/// @brief Construct StringView from null-terminated string.
///
constexpr StringView::StringView(const ValueType* str) noexcept
  : StringView(FW_NS::Pointer<const ValueType>(str))
{
}

///
/// @brief Construct StringView from char span.
///
constexpr StringView::StringView(const ValueType* str, SizeType len) noexcept
  : StringView(FW_NS::Pointer<const ValueType>(str), len)
{
}

///
/// @brief Construct StringView from pair of iterators.
///
template <Concepts::ContiguousIterator It>
constexpr StringView::StringView(It begin, It end)
  : _data {std::to_address(begin)}
  , _size {std::distance(begin, end)}
{
}

///
/// @brief operator[]
///
/// @param[in] index Index of character.
///
constexpr auto StringView::operator[](IndexType index) const -> ConstReference
{
    return _data[index];
}

///
/// @brief operator==
///
constexpr auto StringView::operator==(const StringView& other) const noexcept -> bool
{
    return operator<=>(other) == std::strong_ordering::equal;
}

///
/// @brief operator<=>
///
constexpr auto StringView::operator<=>(const StringView& other) const noexcept -> std::strong_ordering
{
    const auto ord = (GetSize() <=> other.GetSize());
    if (ord == std::strong_ordering::equal)
    {
        const auto data = static_cast<const char8_t*>(GetData());
        const auto otherData = static_cast<const char8_t*>(other.GetData());
        const auto r = std::char_traits<char8_t>::compare(data, otherData, static_cast<size_t>(GetSize()));
        return (r == 0) ? std::strong_ordering::equal : ((r < 0) ? std::strong_ordering::less : std::strong_ordering::greater);
    }
    return ord;
}

///
/// @brief Get begin iterator.
///
constexpr auto StringView::begin() const noexcept -> Iterator
{
    return cbegin();
}

///
/// @brief Get begin iterator.
///
constexpr auto StringView::cbegin() const noexcept -> ConstIterator
{
    return _data;
}

///
/// @brief Get end iterator.
///
constexpr auto StringView::end() const noexcept -> Iterator
{
    return cend();
}

///
/// @brief Get end iterator.
///
constexpr auto StringView::cend() const noexcept -> ConstIterator
{
    return _data + _size;
}

///
/// @brief Get length of string.
///
constexpr auto StringView::GetSize() const noexcept -> SizeType
{
    return _size;
}

///
/// @brief Empty string?
///
constexpr auto StringView::IsEmpty() const noexcept -> Bool
{
    return GetSize() <= 0;
}

///
/// @brief Get data.
///
constexpr auto StringView::GetData() const noexcept -> ConstPointer
{
    return _data;
}

///
/// @brief Get substring.
///
/// @param begin Index of beginning of the substring.
/// @param end Index of one past the end of the substring.
///
constexpr auto StringView::GetSubstring(IndexType begin, IndexType end) const noexcept -> StringView
{
    const auto b = Utility::Clamp(begin, IndexType(0), GetSize());
    const auto e = Utility::Clamp(end, IndexType(0), GetSize());
    return StringView(GetData() + b, Utility::Max(e - b, SizeType(0)));
}

///
/// @brief Returns true if the string contains the specified string.
///
constexpr auto StringView::Contains(const StringView& str) const noexcept -> Bool
{
    auto i1 = SizeType(0);
    auto i2 = SizeType(0);

    while (i1 < GetSize() && i2 < str.GetSize())
    {
        if ((*this)[i1++] == str[i2++])
        {
            if (i2 == str.GetSize())
            {
                return true;
            }
        }
        else
        {
            i2 = 0;
        }
    }
    return false;
}

///
/// @brief Count string.
///
constexpr auto StringView::Count(const StringView& str) const noexcept -> SizeType
{
    auto i1 = SizeType(0);
    auto i2 = SizeType(0);

    auto count = SizeType(0);
    while (i1 < GetSize() && i2 < str.GetSize())
    {
        if ((*this)[i1++] == str[i2++])
        {
            if (i2 == str.GetSize())
            {
                ++count;
                i2 = 0;
            }
        }
        else
        {
            i2 = 0;
        }
    }
    return count;
}

///
/// @brief Starts with string?
///
constexpr auto StringView::StartsWith(const StringView& str) const noexcept -> Bool
{
    if (GetSize() < str.GetSize())
    {
        return false;
    }

    for (SizeType i = 0; i < str.GetSize(); ++i)
    {
        if ((*this)[i] != str[i])
        {
            return false;
        }
    }
    return true;
}

///
/// @brief Ends with string?
///
constexpr auto StringView::EndsWith(const StringView& str) const noexcept -> Bool
{
    if (GetSize() < str.GetSize())
    {
        return false;
    }

    const auto base = GetSize() - str.GetSize();
    for (SizeType i = 0; i < str.GetSize(); ++i)
    {
        if ((*this)[base + i] != str[i])
        {
            return false;
        }
    }
    return true;
}
}
}
