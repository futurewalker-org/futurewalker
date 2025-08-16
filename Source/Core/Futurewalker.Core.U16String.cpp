// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Core.U16String.hpp"
#include "Futurewalker.Core.DataFunction.hpp"

namespace FW_DETAIL_NS
{
U16String::U16String() noexcept
{
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
  : _data(std::make_shared<ValueType[]>(static_cast<size_t>(view.GetSize())))
  , _size(view.GetSize())
  , _capacity(_size)
{
    DataFunction::CopyMemory(Pointer(_data.get()), view.GetData(), SInt64(sizeof(ValueType)) * _size);
}

U16String::U16String(U16String const& other)
  : _data(other._data)
  , _size(other._size)
  , _capacity(other._capacity)
{
}

U16String::U16String(U16String&& other) noexcept
  : U16String()
{
    Swap(other);
}

auto U16String::operator=(U16String const& other) -> U16String&
{
    if (this != &other)
    {
        auto tmp = other;
        Swap(tmp);
    }
    return *this;
}

auto U16String::operator=(U16String&& other) noexcept -> U16String&
{
    if (this != &other)
    {
        auto tmp = std::move(other);
        Swap(tmp);
    }
    return *this;
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

auto U16String::GetView() const noexcept -> U16StringView
{
    return U16StringView(GetImmutableData(), GetSize());
}

auto U16String::GetChar(ValueType& value, IndexType pos) const noexcept -> Bool
{
    if (0 <= pos && pos < GetSize())
    {
        value = GetImmutableData()[pos];
        return true;
    }
    return false;
}

auto U16String::GetSubString(IndexType begin, IndexType end) const noexcept -> U16String
{
    if (begin < 0 || begin > end || end > GetSize())
    {
        return {};
    }

    auto const newSize = (end - begin);
    auto const newCapacity = GetCapacity() - begin;
    auto const newData = _data.get() + static_cast<ptrdiff_t>(begin);

    auto ret = U16String();
    ret.SetSize(newSize);
    ret.SetData(std::shared_ptr<ValueType[]>(_data, newData), newCapacity);
    return ret;
}

auto U16String::Resize(SizeType const size) -> void
{
    if (size < 0)
    {
        return;
    }
    Reserve(size);
    SetSize(size);

    if (auto const oldSize = GetSize(); oldSize < size)
    {
        auto const data = GetMutableData();
        for (auto i = oldSize; i < size; ++i)
        {
            data[i] = ValueType();
        }
    }
}

auto U16String::Reserve(SizeType const capacity) -> void
{
    if (capacity > _capacity)
    {
        auto newData = std::make_shared<ValueType[]>(static_cast<size_t>(capacity));
        if (auto const data = GetImmutableData())
        {
            DataFunction::CopyMemory(Pointer(newData.get()), data, SInt64(sizeof(ValueType)) * _size);
        }
        SetData(newData, capacity);
    }
}

auto U16String::Replace(IndexType begin, IndexType end, U16StringView str) -> void
{
    begin = IndexType::Clamp(begin, 0, GetSize());
    end = IndexType::Clamp(end, begin, GetSize());

    auto const oldSize = GetSize();
    auto const newSize = oldSize - (end - begin) + str.GetSize();
    Reserve(newSize);

    auto const data = GetMutableData();
    auto const copyCount = oldSize - end;

    auto const dstSize = end - begin;
    auto const srcSize = str.GetSize();

    if (dstSize > srcSize)
    {
        for (auto i = SizeType(0); i < copyCount; ++i)
        {
            data[begin + srcSize + i] = data[end + i];
        }
    }
    else if (dstSize < srcSize)
    {
        for (auto i = SizeType(0); i < copyCount; ++i)
        {
            data[newSize - 1 - i] = data[oldSize - 1 - i];
        }
    }

    for (auto i = SizeType(0); i < srcSize; ++i)
    {
        data[begin + i] = str[i];
    }

    SetSize(newSize);
}

void U16String::Swap(U16String& other) noexcept
{
    std::swap(_data, other._data);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
}

auto U16String::GetImmutableData() const -> Pointer<ValueType const>
{
    return Pointer(_data.get());
}

auto U16String::GetMutableData() -> Pointer<ValueType>
{
    if (_data)
    {
        if (_data.use_count() != 1)
        {
            auto data = std::make_shared<ValueType[]>(static_cast<size_t>(_size));
            DataFunction::CopyMemory(Pointer(data.get()), GetImmutableData(), SInt64(sizeof(ValueType)) * _size);
            _data = data;
        }
        return Pointer(_data.get());
    }
    return nullptr;
}

auto U16String::GetSize() const -> SizeType
{
    return _size;
}

auto U16String::GetCapacity() const -> SizeType
{
    return _capacity;
}

auto U16String::SetData(std::shared_ptr<ValueType[]> const& data, SizeType const capacity) -> void
{
    _data = data;
    _capacity = capacity;
}

auto U16String::SetSize(SizeType size) -> void
{
    _size = size;
}
}
