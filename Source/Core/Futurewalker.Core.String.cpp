// SPDX-License-Identifier: MIT
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
auto AllocateSharedString(Pointer<const char8_t> data, const auto size, const auto capacity) -> std::shared_ptr<char8_t[]>
{
    auto ptr = std::make_shared<char8_t[]>(static_cast<size_t>(capacity));
    DataFunction::CopyMemory(Pointer(ptr.get()), data, size);
    return ptr;
}
}

struct String::Internal
{
    static constexpr auto smallStringCapacity = sizeof(SmallString::data); ///< Capacity of small string.
    static constexpr auto largeStringFlag = uint8_t(1 << 7);               ///< Flag used to determine large string mode.
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
String::String(const ValueType c) noexcept
  : String(StringView(&c, 1))
{
}

///
/// @brief Construct string from null terminated string.
///
/// @param[in] chars Null-terminated UTF8 string.
///
String::String(Pointer<const ValueType> chars)
  : String(StringView(chars))
{
}

///
/// @brief Construct string from string buffer.
///
/// @param[in] chars UTF8 string.
/// @param[in] size Size of UTF8 string.
///
String::String(Pointer<const ValueType> chars, const SizeType size)
  : String(StringView(chars, size))
{
}

///
/// @brief Construct string from null terminated string.
///
/// @param[in] chars Null-terminated UTF8 string.
///
String::String(const ValueType* chars)
  : String(StringView(chars))
{
}

///
/// @brief Construct string from string buffer.
///
/// @param[in] chars UTF8 string.
/// @param[in] size Size of UTF8 string.
///
String::String(const ValueType* chars, const SizeType size)
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
String::String(const String& other)
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
String& String::operator=(const String& other)
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
auto String::operator==(const String& other) const noexcept -> bool
{
    return GetView() == other.GetView();
}

///
/// @brief operator<=>
///
auto String::operator<=>(const String& other) const noexcept -> std::strong_ordering
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
/// @note Any modification to string will invalidate returned view.
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
auto String::GetChar(ValueType& value, const IndexType pos) const noexcept -> Bool
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
auto String::GetSubstring(const IndexType begin, const IndexType end) const noexcept -> String
{
    if (begin < 0 || begin > end || end > GetSize())
    {
        return {};
    }

    const auto newSize = (end - begin);
    const auto newCapacity = (GetCapacity() - begin);

    if (IsSmallString())
    {
        const auto data = GetConstData();
        const auto newData = (data + begin);
        const auto view = StringView(newData, newSize);
        return String(view);
    }
    else
    {
        auto ret = String();
        ret._small.~SmallString();
        const auto newData = _large.data.get() + static_cast<ptrdiff_t>(begin);
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
/// @brief Get C-style string.
///
auto String::GetCString() const -> std::u8string
{
    const auto ptr = static_cast<const ValueType*>(GetConstData());
    const auto len = static_cast<size_t>(GetSize());
    return std::u8string(ptr, len);
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
void String::Resize(const SizeType size)
{
    if (size < 0)
    {
        return;
    }

    Reserve(size);
    SetSize(size);

    if (const auto oldSize = GetSize(); oldSize < size)
    {
        const auto data = GetMutableData();
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
void String::Reserve(const SizeType capacity)
{
    if (IsSmallString())
    {
        if (capacity > GetCapacity())
        {
            const auto size = GetSize();
            const auto data = AllocateSharedString(GetConstData(), size, capacity);
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
            const auto cap = std::max(capacity, GetCapacity());
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
auto String::Erase(const IndexType begin, const IndexType end) -> void
{
    if (begin < 0 || begin > end || end > GetSize())
    {
        return;
    }

    if (IsEmpty())
    {
        return;
    }

    const auto data = GetMutableData();
    const auto copyCount = GetSize() - end;
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
auto String::Replace(const IndexType begin, const IndexType end, const StringView str) -> void
{
    if (begin < 0 || begin > end || end > GetSize())
    {
        return;
    }

    if (IsEmpty())
    {
        return;
    }

    const auto oldSize = GetSize();
    const auto newSize = oldSize - (end - begin) + str.GetSize();
    Reserve(newSize);

    const auto data = GetMutableData();
    const auto copyCount = oldSize - end;

    const auto dstSize = end - begin;
    const auto srcSize = str.GetSize();

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
auto String::Insert(const IndexType pos, const StringView str) -> void
{
    if (pos < 0 || GetSize() < pos)
    {
        return;
    }

    const auto oldSize = GetSize();
    const auto newSize = oldSize + str.GetSize();

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
auto String::SetSize(const SizeType size) -> void
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
auto String::GetConstData() const -> Pointer<const ValueType>
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