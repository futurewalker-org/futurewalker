// SPDX-License-Identifier: MPL-2.0
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.DataFunction.hpp"

namespace FW_DETAIL_NS
{
namespace
{
///
/// @brief Allocate new shared string buffer and copy data into it.
///
/// @param[in] data Pointer to input string.
/// @param[in] size Size of input string.
/// @param[in] capacity Size of buffer allocate.
///
auto AllocateSharedString(Pointer<char8_t const> const data, auto const size, auto const capacity) -> std::shared_ptr<char8_t[]>
{
    auto const ptr = std::make_shared<char8_t[]>(static_cast<size_t>(capacity));
    DataFunction::CopyMemory(Pointer(ptr.get()), data, size);
    return ptr;
}
}

struct String::Internal
{
    static auto constexpr smallStringCapacity = sizeof(SmallString::data); ///< Capacity of small string.
    static auto constexpr largeStringFlag = uint8_t(1 << 7);               ///< Flag used to determine large string mode.
};

///
/// @brief Construct empty string.
///
String::String() noexcept
  : _small {}
{
}

///
/// @brief Destructor.
///
String::~String() noexcept
{
    if (IsSmallString())
    {
        _small.~SmallString();
    }
    else
    {
        _large.~LargeString();
    }
}

///
/// @brief Construct sting from single character.
///
/// @param[in] c Character.
///
String::String(ValueType c) noexcept
  : String(StringView(&c, 1))
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
    Resize(view.GetSize());
    DataFunction::CopyMemory(GetMutableData(), view.GetData(), view.GetSize());
}

///
/// @brief Copy constructor.
///
String::String(String const& other)
  : String()
{
    if (other.IsSmallString())
    {
        _small = other._small;
    }
    else
    {
        _small.~SmallString();
        new (&_large) LargeString(other._large);
    }
}

///
/// @brief Move constructor.
///
String::String(String&& other) noexcept
  : String()
{
    Swap(other);
}

///
/// @brief operator=
///
String& String::operator=(String const& other)
{
    if (this != &other)
    {
        auto tmp = other;
        Swap(tmp);
    }
    return *this;
}

///
/// @brief operator=
///
String& String::operator=(String&& other) noexcept
{
    if (this != &other)
    {
        auto tmp = std::move(other);
        Swap(tmp);
    }
    return *this;
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
    if (IsSmallString())
    {
        return _small.size == uint8_t();
    }
    else
    {
        return _large.size == 0;
    }
}

///
/// @brief Get view of string.
///
/// @note Unsafe: Any modification to string will invalidate returned view.
///
auto String::GetView() const noexcept -> StringView
{
    return StringView(GetConstData(), GetSize());
}

///
/// @brief Get character at given position.
///
/// @param[in] pos Position of character.
///
auto String::GetChar(ValueType& value, IndexType const pos) const noexcept -> Bool
{
    if (pos < 0 || GetSize() <= pos)
    {
        return false;
    }
    value = GetConstData()[pos];
    return false;
}

///
/// @brief Extract range of sub string.
///
/// @param[in] begin Index of beginning of range.
/// @param[in] end Index of end of range.
///
auto String::GetSubString(IndexType const begin, IndexType const end) const noexcept -> String
{
    if (begin < 0 || begin > end || end > GetSize())
    {
        return {};
    }

    auto const newSize = (end - begin);
    auto const newCapacity = (GetCapacity() - begin);

    if (IsSmallString())
    {
        auto const data = GetConstData();
        auto const newData = (data + begin);
        auto const view = StringView(newData, newSize);
        return String(view);
    }
    else
    {
        auto ret = String();
        ret._small.~SmallString();
        auto const newData = _large.data.get() + static_cast<ptrdiff_t>(begin);
        new (&ret._large) LargeString {
            .data = std::shared_ptr<ValueType[]>(_large.data, newData),
            .capacity = static_cast<int64_t>(newCapacity),
            .size = static_cast<int64_t>(newSize),
            .padding = {},
            .flags = Internal::largeStringFlag,
        };
        return ret;
    }
}

///
/// @brief Resize string.
///
/// @param[in] size New size of string.
///
/// @note Invalidates previously returned iterators and data pointers.
///
/// @pre size must not be negative.
///
void String::Resize(SizeType const size)
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

///
/// @brief Reserve string.
///
/// @param[in] capacity New capacity.
///
/// @note Invalidates previously returned iterators and data pointers.
///
void String::Reserve(SizeType const capacity)
{
    if (IsSmallString())
    {
        if (capacity > GetCapacity())
        {
            auto const size = GetSize();
            auto const data = AllocateSharedString(GetConstData(), size, capacity);
            _small.~SmallString();
            new (&_large) LargeString {
                .data = data,
                .capacity = static_cast<int64_t>(capacity),
                .size = static_cast<int64_t>(size),
                .padding = {},
                .flags = Internal::largeStringFlag,
            };
        }
    }
    else
    {
        if (capacity > GetCapacity() || _large.data.use_count() != 1)
        {
            auto const cap = std::max(capacity, GetCapacity());
            _large.data = AllocateSharedString(GetConstData(), GetSize(), cap);
            _large.capacity = static_cast<int64_t>(cap);
        }
    }
}

///
/// @brief Clear string.
///
void String::Clear()
{
    SetSize(0);
}

///
/// @brief Erase range of string.
///
/// @param[in] begin Index of beginning of range to erase.
/// @param[in] end Index of end of range to erase (non-inclusive).
///
auto String::Erase(IndexType const begin, IndexType const end) -> void
{
    if (begin < 0 || begin > end || end > GetSize())
    {
        return;
    }

    if (IsEmpty())
    {
        return;
    }

    auto const data = GetMutableData();
    auto const copyCount = GetSize() - end;
    for (auto i = SizeType(0); i < copyCount; ++i)
    {
        data[begin + i] = data[end + i];
    }
    SetSize(begin + copyCount);
}

///
/// @brief Replace range of string into new string.
///
/// @param[in] begin Index of beginning of range to replace.
/// @param[in] end Index of end of range to replace.
/// @param[in] str String to replace range.
///
auto String::Replace(IndexType const begin, IndexType const end, StringView const str) -> void
{
    if (begin < 0 || begin > end || end > GetSize())
    {
        return;
    }

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

///
/// @brief Insert string into position.
///
/// @param[in] pos Position to insert string.
/// @param[in] str String to be inserted.
///
auto String::Insert(IndexType const pos, StringView const str) -> void
{
    if (pos < 0 || GetSize() < pos)
    {
        return;
    }

    auto const oldSize = GetSize();
    auto const newSize = oldSize + str.GetSize();

    if (oldSize == newSize)
    {
        return;
    }

    Reserve(newSize);

    auto data = GetMutableData();

    for (auto i = SizeType(0); i < str.GetSize(); ++i)
    {
        data[newSize - 1 - i] = data[oldSize - 1 - i];
    }

    for (auto i = SizeType(0); i < str.GetSize(); ++i)
    {
        data[pos + i] = str[i];
    }

    SetSize(newSize);
}

///
/// @brief Swap two string.
///
/// @param[in] other Other string object.
///
void String::Swap(String& other) noexcept
{
    if (this == &other)
    {
        return;
    }

    if (IsSmallString())
    {
        if (IsSmallString())
        {
            std::swap(_small, other._small);
        }
        else
        {
            auto large = std::move(other._large);
            other._large.~LargeString();
            new (&other._small) SmallString(std::move(_small));
            _small.~SmallString();
            new (&_large) LargeString(std::move(large));
        }
    }
    else
    {
        if (IsSmallString())
        {
            auto small = std::move(other._small);
            other._small.~SmallString();
            new (&other._large) LargeString(std::move(_large));
            _large.~LargeString();
            new (&_small) SmallString(std::move(small));
        }
        else
        {
            std::swap(_large, other._large);
        }
    }
}

///
/// @brief Small string?
///
auto String::IsSmallString() const -> Bool
{
    auto extra = uint8_t();
    DataFunction::CopyMemory(Pointer(&extra), Pointer(&_small.size), 1);
    return (extra & Internal::largeStringFlag) == 0U;
}

///
/// @brief Set size without modifying data.
///
/// @param[in] size New size.
///
auto String::SetSize(SizeType const size) -> void
{
    if (IsSmallString())
    {
        _small.size = static_cast<uint8_t>(size);
    }
    else
    {
        _large.size = static_cast<int64_t>(size);
    }
}

///
/// @brief Get size of string.
///
auto String::GetSize() const noexcept -> SizeType
{
    if (IsSmallString())
    {
        return _small.size;
    }
    else
    {
        return _large.size;
    }
}

///
/// @brief Get capacity of string.
///
auto String::GetCapacity() const noexcept -> SizeType
{
    if (IsSmallString())
    {
        return static_cast<SizeType>(Internal::smallStringCapacity);
    }
    else
    {
        return _large.capacity;
    }
}

///
/// @brief Get immutable data pointer.
///
auto String::GetConstData() const -> Pointer<ValueType const>
{
    if (IsSmallString())
    {
        return _small.data;
    }
    else
    {
        return _large.data.get();
    }
}

///
/// @brief Get mutable data pointer.
///
auto String::GetMutableData() -> Pointer<ValueType>
{
    if (IsSmallString())
    {
        return _small.data;
    }
    else
    {
        if (_large.data.use_count() != 1)
        {
            _large.data = AllocateSharedString(_large.data.get(), _large.size, _large.capacity);
        }
        return _large.data.get();
    }
}
}
