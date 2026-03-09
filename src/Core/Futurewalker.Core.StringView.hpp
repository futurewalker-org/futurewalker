// SPDX-License-Identifier: MPL-2.0
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
/// @brief View on array of unicode code units.
///
/// Lightweight non-owning view for unicode span.
/// Contains pair of (usually non null-terminated) buffer and its length.
///
/// ### Recommendations
///
/// This class is mostly used for interfacing with literal strings or external libraries.
/// It is recommended to use String for most of the cases.
///
/// StringView should only be used for transient objects like function arguments.
/// Returning StringView from functions can lead to dangling pointers thus considered unsafe.
///
template <class Char>
class StringViewT
{
public:
    using ValueType = Char;
    using SizeType = SInt64;
    using IndexType = SInt64;
    using Pointer = FW_NS::Pointer<ValueType>;
    using ConstPointer = FW_NS::Pointer<ValueType const>;
    using Reference = ValueType&;
    using ConstReference = ValueType const&;
    using ConstIterator = ConstPointerIterator<ValueType, StringView>;
    using Iterator = ConstIterator;

public:
    constexpr StringViewT() noexcept = default;
    constexpr StringViewT(StringViewT const&) noexcept = default;
    constexpr StringViewT(ConstPointer str) noexcept;
    constexpr StringViewT(ConstPointer str, SizeType len) noexcept;
    constexpr StringViewT(ValueType const* str) noexcept;
    constexpr StringViewT(ValueType const* str, SizeType len) noexcept;

    template <Concepts::Integral T>
    StringViewT(T const&) = delete;

    StringViewT(std::nullptr_t) = delete;

    template <Concepts::ContiguousIterator It>
    constexpr StringViewT(It begin, It end);

    [[nodiscard]] constexpr auto operator[](IndexType index) const -> ConstReference;
    [[nodiscard]] constexpr auto operator==(StringViewT const& other) const noexcept -> bool;
    [[nodiscard]] constexpr auto operator<=>(StringViewT const& other) const noexcept -> std::strong_ordering;

    [[nodiscard]] constexpr auto begin() const noexcept -> Iterator;
    [[nodiscard]] constexpr auto cbegin() const noexcept -> ConstIterator;
    [[nodiscard]] constexpr auto end() const noexcept -> Iterator;
    [[nodiscard]] constexpr auto cend() const noexcept -> ConstIterator;

    [[nodiscard]] constexpr auto IsEmpty() const noexcept -> Bool;
    [[nodiscard]] constexpr auto GetSize() const noexcept -> SizeType;
    [[nodiscard]] constexpr auto GetData() const noexcept -> ConstPointer;

    [[nodiscard]] constexpr auto GetSubString(IndexType begin, IndexType end) const noexcept -> StringViewT;
    [[nodiscard]] constexpr auto Contains(StringViewT const& str) const noexcept -> Bool;
    [[nodiscard]] constexpr auto Count(StringViewT const& str) const noexcept -> SizeType;
    [[nodiscard]] constexpr auto StartsWith(StringViewT const& str) const noexcept -> Bool;
    [[nodiscard]] constexpr auto EndsWith(StringViewT const& str) const noexcept -> Bool;

private:
    ConstPointer _data = nullptr;
    SInt64 _size = 0;
};

///
/// @brief Construct StringView from null-terminated string.
///
template <class Char>
constexpr StringViewT<Char>::StringViewT(ConstPointer str) noexcept
  : _data {str}
  , _size {std::char_traits<ValueType>::length(static_cast<ValueType const*>(str))}
{
}

///
/// @brief Construct StringView from char span.
///
template <class Char>
constexpr StringViewT<Char>::StringViewT(ConstPointer str, SizeType len) noexcept
  : _data {str}
  , _size {len}
{
}

///
/// @brief Construct StringView from null-terminated string.
///
template <class Char>
constexpr StringViewT<Char>::StringViewT(ValueType const* str) noexcept
  : StringViewT(FW_NS::Pointer<ValueType const>(str))
{
}

///
/// @brief Construct StringView from char span.
///
template <class Char>
constexpr StringViewT<Char>::StringViewT(ValueType const* str, SizeType len) noexcept
  : StringViewT(FW_NS::Pointer<ValueType const>(str), len)
{
}

///
/// @brief Construct StringView from pair of iterators.
///
template <class Char>
template <Concepts::ContiguousIterator It>
constexpr StringViewT<Char>::StringViewT(It begin, It end)
  : _data {std::to_address(begin)}
  , _size {std::distance(begin, end)}
{
}

///
/// @brief operator[]
///
/// @param[in] index Index of character.
///
template <class Char>
constexpr auto StringViewT<Char>::operator[](IndexType index) const -> ConstReference
{
    return _data[index];
}

///
/// @brief operator==
///
template <class Char>
constexpr auto StringViewT<Char>::operator==(StringViewT const& other) const noexcept -> bool
{
    return operator<=>(other) == std::strong_ordering::equal;
}

///
/// @brief operator<=>
///
template <class Char>
constexpr auto StringViewT<Char>::operator<=>(StringViewT const& other) const noexcept -> std::strong_ordering
{
    auto const ord = (GetSize() <=> other.GetSize());
    if (ord == std::strong_ordering::equal)
    {
        auto const data = static_cast<ValueType const*>(GetData());
        auto const otherData = static_cast<ValueType const*>(other.GetData());
        auto const r = std::char_traits<ValueType>::compare(data, otherData, static_cast<size_t>(GetSize()));
        return (r == 0) ? std::strong_ordering::equal : ((r < 0) ? std::strong_ordering::less : std::strong_ordering::greater);
    }
    return ord;
}

///
/// @brief Get begin iterator.
///
template <class Char>
constexpr auto StringViewT<Char>::begin() const noexcept -> Iterator
{
    return cbegin();
}

///
/// @brief Get begin iterator.
///
template <class Char>
constexpr auto StringViewT<Char>::cbegin() const noexcept -> ConstIterator
{
    return _data;
}

///
/// @brief Get end iterator.
///
template <class Char>
constexpr auto StringViewT<Char>::end() const noexcept -> Iterator
{
    return cend();
}

///
/// @brief Get end iterator.
///
template <class Char>
constexpr auto StringViewT<Char>::cend() const noexcept -> ConstIterator
{
    return _data + _size;
}

///
/// @brief Get length of string.
///
template <class Char>
constexpr auto StringViewT<Char>::GetSize() const noexcept -> SizeType
{
    return _size;
}

///
/// @brief Empty string?
///
template <class Char>
constexpr auto StringViewT<Char>::IsEmpty() const noexcept -> Bool
{
    return GetSize() <= 0;
}

///
/// @brief Get data.
///
template <class Char>
constexpr auto StringViewT<Char>::GetData() const noexcept -> ConstPointer
{
    return _data;
}

///
/// @brief Get substring.
///
/// @param begin Index of beginning of the substring.
/// @param end Index of one past the end of the substring.
///
template <class Char>
constexpr auto StringViewT<Char>::GetSubString(IndexType begin, IndexType end) const noexcept -> StringViewT
{
    auto const b = Utility::Clamp(begin, IndexType(0), GetSize());
    auto const e = Utility::Clamp(end, IndexType(0), GetSize());
    return StringView(GetData() + b, Utility::Max(e - b, SizeType(0)));
}

///
/// @brief Returns true if the string contains the specified string.
///
template <class Char>
constexpr auto StringViewT<Char>::Contains(StringViewT const& str) const noexcept -> Bool
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
template <class Char>
constexpr auto StringViewT<Char>::Count(StringViewT const& str) const noexcept -> SizeType
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
template <class Char>
constexpr auto StringViewT<Char>::StartsWith(StringViewT const& str) const noexcept -> Bool
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
template <class Char>
constexpr auto StringViewT<Char>::EndsWith(StringViewT const& str) const noexcept -> Bool
{
    if (GetSize() < str.GetSize())
    {
        return false;
    }

    auto const base = GetSize() - str.GetSize();
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
