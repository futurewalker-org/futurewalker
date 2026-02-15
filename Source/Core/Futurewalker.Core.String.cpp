// SPDX-License-Identifier: MPL-2.0
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.DataFunction.hpp"
#include "Futurewalker.Core.UnicodeFunction.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Create String from UTF-8 encoded string with runtime checks.
///
/// Copies content of incoming string into new String object.
/// Any invalid code points found in incoming string will be replaced with U+FFFD.
///
/// @param[in] sv A view of UTF-8 encoded string.
///
/// @return Constructed String.
///
/// @note This function will not perform conversion from narrow string to UTF-8 string.
///
auto String::MakeFromStdString(std::string_view const sv) -> String
{
    return String(reinterpret_cast<const char8_t*>(sv.data()), SInt64(sv.size()));
}

///
/// @brief Create String from UTF-8 encoded string with runtime checks.
///
/// Copies content of incoming string into new String object.
/// Any invalid code points found in incoming string will be replaced with U+FFFD.
///
/// @param[in] sv A view of UTF-8 encoded string.
///
/// @return Constructed String.
///
auto String::MakeFromStdU8String(std::u8string_view const sv) -> String
{
    return String(sv.data(), SInt64(sv.size()));
}

///
/// @brief Construct string from single character.
///
/// @param c Character.
///
/// @note If c is not a valid Unicode code point, it will be replaced with U+FFFD.
///
String::String(CharType c) noexcept
  : String()
{
    auto codeUnits = std::array<ValueType, 4>();
    _data.Resize(UnicodeFunction::GetCodeUnitsOrFFFD(codeUnits, c), false);
    DataFunction::MemoryCopy(_data.GetMutableData(), Pointer<char8_t const>(codeUnits.data()), _data.GetSize());
}

///
/// @brief Construct sting from single code unit value.
///
/// @param[in] v Code unit.
///
/// @note If v does not represent valid Unicode code point, it will be replaced with U+FFFD.
///
String::String(ValueType v) noexcept
  : String(StringView(&v, 1))
{
}

///
/// @brief Construct string from null terminated string.
///
/// @param[in] chars Null-terminated UTF8 string.
///
String::String(Pointer<ValueType const> chars)
  : String(StringView(chars))
{
}

///
/// @brief Construct string from string buffer.
///
/// @param[in] chars UTF8 string.
/// @param[in] size Size of UTF8 string.
///
String::String(Pointer<ValueType const> chars, SizeType size)
  : String(StringView(chars, size))
{
}

///
/// @brief Construct string from null terminated string.
///
/// @param[in] chars Null-terminated UTF8 string.
///
String::String(ValueType const* chars)
  : String(StringView(chars))
{
}

///
/// @brief Construct string from string buffer.
///
/// @param[in] chars UTF8 string.
/// @param[in] size Size of UTF8 string.
///
String::String(ValueType const* chars, SizeType size)
  : String(StringView(chars, size))
{
}

///
/// @brief Construct string from view.
///
/// @param[in] view A string view.
///
String::String(StringView view)
  : String()
{
    _data.Reserve(view.GetSize());
    auto index = SInt64(0);
    auto codeUnits = std::array<ValueType, 4>();
    while (index < view.GetSize())
    {
        auto const [codePoint, nextIndex] = UnicodeFunction::GetCodePointOrFFFDAndNextIndex(view.GetData(), index, view.GetSize());
        auto const codeUnitCount = UnicodeFunction::GetCodeUnitsUnsafe(codeUnits, codePoint);
        _data.Resize(_data.GetSize() + codeUnitCount, false);
        DataFunction::MemoryCopy(_data.GetMutableData() + (_data.GetSize() - codeUnitCount), Pointer<ValueType const>(codeUnits.data()), codeUnitCount);
        index = nextIndex;
    }
}

///
/// @brief StringView
///
String::operator StringView() const noexcept
{
    return GetView();
}

///
/// @brief operator==
///
auto String::operator==(String const& other) const noexcept -> bool
{
    return GetView() == other.GetView();
}

///
/// @brief operator<=>
///
auto String::operator<=>(String const& other) const noexcept -> std::strong_ordering
{
    return GetView() <=> other.GetView();
}

///
/// @brief Check if string is empty.
///
auto String::IsEmpty() const noexcept -> Bool
{
    return _data.IsEmpty();
}

///
/// @brief Get index of first character.
///
auto String::GetBegin() const -> IndexType
{
    return IndexType(0);
}

///
/// @brief Get index of one past last character.
///
auto String::GetEnd() const -> IndexType
{
    return IndexType(_data.GetSize());
}

///
/// @brief Get next character index.
///
/// @param index Current character index.
/// @param offset Offset to move.
///
auto String::GetNext(IndexType const& index, SInt64 const offset) const -> IndexType
{
    if (auto const data = _data.GetImmutableData())
    {
        if (offset > 0)
        {
            return IndexType(UnicodeFunction::GetNextCodePointIndex(data, SInt64(index), _data.GetSize(), SInt64(offset)));
        }
        else
        {
            return GetPrev(index, -offset);
        }
    }
    return index;
}

///
/// @brief Get previous character index.
///
/// @param index Current character index.
/// @param offset Offset to move.
///
auto String::GetPrev(IndexType const& index, SInt64 const offset) const -> IndexType
{
    if (auto const data = _data.GetImmutableData())
    {
        if (offset > 0)
        {
            return IndexType(UnicodeFunction::GetPrevCodePointIndex(data, SInt64(index), 0, SInt64(offset)));
        }
        else
        {
            return GetNext(index, -offset);
        }
    }
    return index;
}

///
/// @brief Get character at index.
///
/// @param index Character index.
/// @param value Output character.
///
/// @return True if character was successfully retrieved, false otherwise.
///
auto String::GetChar(IndexType const& index, CharType& value) const -> Bool
{
    if (GetBegin() <= index && index < GetEnd())
    {
        if (auto const data = _data.GetImmutableData())
        {
            if (auto const c = UnicodeFunction::GetCodePoint(data, SInt64(index), 0, _data.GetSize()))
            {
                value = static_cast<CharType>(*c);
                return true;
            }
        }
    }
    return false;
}

///
/// @brief Get character at index and next index.
///
/// @param[in] index Character index.
/// @param[out] value Output character.
/// @param[out] next Next character index.
///
/// @return True if character was successfully retrieved, false otherwise.
///
/// @note index and next can overlap.
///
auto String::GetChar(IndexType const& index, CharType& value, IndexType& next) const -> Bool
{
    if (GetBegin() <= index && index < GetEnd())
    {
        if (auto const data = _data.GetImmutableData())
        {
            auto const [c, nextIndex] = UnicodeFunction::GetCodePointAndNextIndex(data, SInt64(index), _data.GetSize());
            next = IndexType(nextIndex);
            if (c)
            {
                value = static_cast<CharType>(*c);
                return true;
            }
        }
    }
    return false;
}

///
/// @brief
///
/// @param begin
/// @param end
///
auto String::GetSubString(IndexType const& begin, IndexType const& end) const noexcept -> String
{
    if (begin < GetBegin() || begin > end || end > GetEnd())
    {
        return {};
    }

    auto const data = _data.GetImmutableData();
    auto const beginOffset = UnicodeFunction::GetCodePointStartUnsafe(data, SInt64(GetEnd()), SInt64(begin));
    auto const endOffset = UnicodeFunction::GetCodePointLimitUnsafe(data, SInt64(GetEnd()), SInt64(end));
    return String(_data.GetSubString(beginOffset, endOffset));
}

///
/// @brief Get string view.
///
auto String::GetView() const noexcept -> StringView
{
    return StringView(_data.GetImmutableData(), _data.GetSize());
}

///
/// @brief Clear string.
///
void String::Clear()
{
    _data.Clear();
}

///
/// @brief Erase range of string.
///
/// @param[in] begin IndexType of beginning of range to erase.
/// @param[in] end IndexType of end of range to erase (non-inclusive).
///
auto String::Erase(IndexType const& begin, IndexType const& end) -> void
{
    if (IsEmpty())
    {
        return;
    }

    if (begin < GetBegin() || begin > end || end > GetEnd())
    {
        return;
    }

    auto const data = _data.GetMutableData();
    auto const beginOffset = UnicodeFunction::GetCodePointStartUnsafe(data, _data.GetSize(), SInt64(begin));
    auto const endOffset = UnicodeFunction::GetCodePointLimitUnsafe(data, _data.GetSize(), SInt64(end));

    auto const copyCount = _data.GetSize() - endOffset;
    for (auto i = SizeType(0); i < copyCount; ++i)
    {
        data[beginOffset + i] = data[endOffset + i];
    }
    _data.Resize(beginOffset + copyCount, false);
}

///
/// @brief Replace range of string into new string.
///
/// @param[in] begin IndexType of beginning of range to replace.
/// @param[in] end IndexType of end of range to replace.
/// @param[in] str String to replace range.
///
auto String::Replace(IndexType const& begin, IndexType const& end, String const& str) -> void
{
    if (begin < GetBegin() || begin > end || end > GetEnd())
    {
        return;
    }

    auto const oldData = _data.GetImmutableData();
    auto const oldSize = _data.GetSize();
    auto const beginOffset = UnicodeFunction::GetCodePointStartUnsafe(oldData, oldSize, SInt64(begin));
    auto const endOffset = UnicodeFunction::GetCodePointLimitUnsafe(oldData, oldSize, SInt64(end));
    auto const newSize = oldSize - (endOffset - beginOffset) + str._data.GetSize();
    _data.Resize(newSize, false); // Assuming capacity does not decrease on resize.

    auto const data = _data.GetMutableData();
    auto const copyCount = oldSize - endOffset;

    auto const dstSize = endOffset - beginOffset;
    auto const srcSize = str._data.GetSize();

    if (dstSize > srcSize)
    {
        for (auto i = SizeType(0); i < copyCount; ++i)
        {
            data[beginOffset + srcSize + i] = data[endOffset + i];
        }
    }
    else if (dstSize < srcSize)
    {
        for (auto i = SizeType(0); i < copyCount; ++i)
        {
            data[newSize - 1 - i] = data[oldSize - 1 - i];
        }
    }

    auto const strData = str._data.GetImmutableData();
    for (auto i = SizeType(0); i < srcSize; ++i)
    {
        data[beginOffset + i] = strData[i];
    }
}

///
/// @brief Insert string into position.
///
/// @param[in] pos Position to insert string.
/// @param[in] str String to be inserted.
///
auto String::Insert(IndexType const& pos, String const& str) -> void
{
    Replace(pos, pos, str);
}

///
/// @brief Append string at the end.
///
auto String::Append(String const& string) -> void
{
    auto const end = GetEnd();
    Replace(end, end, string);
}

///
/// @brief Swap two string.
///
/// @param[in] other Other string object.
///
void String::Swap(String& other) noexcept
{
    _data.Swap(other._data);
}

///
/// @brief Convert String to std::string.
///
auto String::ToStdString() const -> std::string
{
    if (auto const data = _data.GetImmutableData())
    {
        return std::string(reinterpret_cast<const char*>(static_cast<char8_t const*>(data)), static_cast<size_t>(_data.GetSize()));
    }
    return {};
}

///
/// @brief Convert String to std::u8string.
///
auto String::ToStdU8String() const -> std::u8string
{
    if (auto const data = _data.GetImmutableData())
    {
        return std::u8string(static_cast<char8_t const*>(data), static_cast<size_t>(_data.GetSize()));
    }
    return {};
}

///
/// @brief Private constructor from StringData.
///
String::String(StringData<ValueType>&& data) noexcept
  : _data(std::move(data))
{
}
}
