// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Core.U16String.hpp"
#include "Futurewalker.Core.DataFunction.hpp"
#include "Futurewalker.Core.UnicodeFunction.hpp"

#include <array>

namespace FW_DETAIL_NS
{
U16String::U16String(CharType const c)
  : U16String()
{
    auto codeUnits = std::array<ValueType, 2>();
    _data.Resize(UnicodeFunction::GetCodeUnitsOrFFFD(codeUnits, c), false);
    DataFunction::MemoryCopy(_data.GetMutableData(), Pointer<char16_t const>(codeUnits.data()), _data.GetSize());
}

U16String::U16String(ValueType const c)
  : U16String(U16StringView(&c, 1))
{
}

U16String::U16String(Pointer<ValueType const> chars)
  : U16String(U16StringView(chars))
{
}

U16String::U16String(Pointer<ValueType const> chars, SizeType size)
  : U16String(U16StringView(chars, size))
{
}

U16String::U16String(ValueType const* chars)
  : U16String(U16StringView(chars))
{
}

U16String::U16String(ValueType const* chars, SizeType size)
  : U16String(U16StringView(chars, size))
{
}

U16String::U16String(U16StringView view)
  : U16String()
{
    _data.Reserve(view.GetSize());
    auto index = SInt64(0);
    auto codeUnits = std::array<ValueType, 2>();
    while (index < view.GetSize())
    {
        auto const [codePoint, nextIndex] = UnicodeFunction::GetCodePointOrFFFDAndNextIndex(view.GetData(), index, view.GetSize());
        auto const codeUnitCount = UnicodeFunction::GetCodeUnitsUnsafe(codeUnits, codePoint);
        _data.Resize(_data.GetSize() + codeUnitCount, false);
        DataFunction::MemoryCopy(_data.GetMutableData() + (_data.GetSize() - codeUnitCount), Pointer<ValueType const>(codeUnits.data()), codeUnitCount);
        index = nextIndex;
    }
}

U16String::operator U16StringView() const noexcept
{
    return GetView();
}

auto U16String::operator==(U16String const& other) const noexcept -> bool
{
    return GetView() == other.GetView();
}

auto U16String::operator<=>(U16String const& other) const noexcept -> std::strong_ordering
{
    return GetView() <=> other.GetView();
}

auto U16String::IsEmpty() const noexcept -> Bool
{
    return GetView().IsEmpty();
}

auto U16String::GetBegin() const -> IndexType
{
    return IndexType(0);
}

auto U16String::GetEnd() const -> IndexType
{
    return IndexType(_data.GetSize());
}

auto U16String::GetNext(IndexType const& index, SInt64 const offset) const -> IndexType
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

auto U16String::GetPrev(IndexType const& index, SInt64 const offset) const -> IndexType
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

auto U16String::GetChar(IndexType const& index, CharType& value) const -> Bool
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

auto U16String::GetChar(IndexType const& index, CharType& value, IndexType& next) const -> Bool
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

auto U16String::GetSubString(IndexType const& begin, IndexType const& end) const noexcept -> U16String
{
    if (begin < GetBegin() || begin > end || end > GetEnd())
    {
        return {};
    }

    auto const data = _data.GetImmutableData();
    auto const beginOffset = UnicodeFunction::GetCodePointStartUnsafe(data, SInt64(GetEnd()), SInt64(begin));
    auto const endOffset = UnicodeFunction::GetCodePointLimitUnsafe(data, SInt64(GetEnd()), SInt64(end));
    return U16String(_data.GetSubString(beginOffset, endOffset));
}

auto U16String::GetView() const noexcept -> U16StringView
{
    return U16StringView(_data.GetImmutableData(), _data.GetSize());
}

auto U16String::Clear() -> void
{
    _data.Clear();
}

auto U16String::Replace(IndexType const& begin, IndexType const& end, U16String const& str) -> void
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

auto U16String::Insert(IndexType const& pos, U16String const& str) -> void
{
    Replace(pos, pos, str);
}

auto U16String::Append(U16String const& string) -> void
{
    auto const end = GetEnd();
    Replace(end, end, string);
}

auto U16String::Swap(U16String& other) noexcept -> void
{
    _data.Swap(other._data);
}

U16String::U16String(StringData<ValueType>&& data) noexcept
  : _data {std::move(data)}
{
}
}
