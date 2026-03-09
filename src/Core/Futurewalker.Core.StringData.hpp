// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"
#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.DataFunction.hpp"

#include <memory>
#include <cstring>

namespace FW_DETAIL_NS
{
///
/// @brief String data storage.
///
template <class T>
class StringData
{
public:
    using ValueType = T;
    using SizeType = SInt64;

    ///
    /// @brief Constructor.
    ///
    constexpr StringData() noexcept
      : _small {}
    {
    }

    ///
    /// @brief Copy constructor.
    ///
    constexpr StringData(StringData const& other) noexcept
      : StringData()
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
    constexpr StringData(StringData&& other) noexcept
      : StringData()
    {
        Swap(other);
    }

    ///
    /// @brief Destructor.
    ///
    constexpr ~StringData() noexcept
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
    /// @brief Copy assignment operator.
    ///
    constexpr auto operator=(StringData const& other) noexcept -> StringData&
    {
        if (this != &other)
        {
            auto tmp = other;
            Swap(tmp);
        }
        return *this;
    }

    ///
    /// @brief Move assignment operator.
    ///
    constexpr auto operator=(StringData&& other) noexcept -> StringData&
    {
        if (this != &other)
        {
            auto tmp = std::move(other);
            Swap(tmp);
        }
        return *this;
    }

    ///
    /// @brief Small string?
    ///
    auto IsSmallString() const noexcept -> Bool
    {
        auto extra = uint8_t();
        DataFunction::MemoryCopy(Pointer(&extra), Pointer(reinterpret_cast<char const*>(&_small) + (sizeof(SmallString) - 1)), 1);
        return (extra & (1 << 7)) == 0U;
    }

    ///
    /// @brief Is empty string?
    ///
    auto IsEmpty() const noexcept -> Bool
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
    /// @brief Get size of string.
    ///
    auto GetSize() const noexcept -> SizeType
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
    auto GetCapacity() const noexcept -> SizeType
    {
        if (IsSmallString())
        {
            return static_cast<SizeType>(sizeof(SmallString::data) / sizeof(ValueType));
        }
        else
        {
            return _large.capacity;
        }
    }

    ///
    /// @brief Get immutable data pointer.
    ///
    auto GetImmutableData() const -> Pointer<ValueType const>
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
    auto GetMutableData() -> Pointer<ValueType>
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

    ///
    /// @brief Swap string data with other.
    ///
    auto Swap(StringData& other) noexcept -> void
    {
        if (this == &other)
        {
            return;
        }

        if (IsSmallString())
        {
            if (other.IsSmallString())
            {
                std::swap(_small, other._small);
            }
            else
            {
                auto tmp = std::move(other._large);
                other._large.~LargeString();
                new (&other._small) SmallString(std::move(_small));
                _small.~SmallString();
                new (&_large) LargeString(std::move(tmp));
            }
        }
        else
        {
            if (other.IsSmallString())
            {
                auto tmp = std::move(other._small);
                other._small.~SmallString();
                new (&other._large) LargeString(std::move(_large));
                _large.~LargeString();
                new (&_small) SmallString(std::move(tmp));
            }
            else
            {
                std::swap(_large, other._large);
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
    auto Reserve(SizeType const capacity) -> void
    {
        if (IsSmallString())
        {
            if (capacity > GetCapacity())
            {
                auto const size = GetSize();
                auto const data = AllocateSharedString(GetImmutableData(), size, capacity);
                _small.~SmallString();
                new (&_large) LargeString {
                    .data = data,
                    .capacity = static_cast<int64_t>(capacity),
                    .size = static_cast<int64_t>(size),
                    .flags = 1 << 7,
                };
            }
        }
        else
        {
            if (capacity > GetCapacity() || _large.data.use_count() != 1)
            {
                auto const cap = std::max(capacity, GetCapacity());
                _large.data = AllocateSharedString(GetImmutableData(), GetSize(), cap);
                _large.capacity = static_cast<int64_t>(cap);
            }
        }
    }

    ///
    /// @brief Resize string.
    ///
    /// @param[in] size New size of string.
    /// @param[in] clear Clear new elements if true.
    ///
    /// @note Invalidates previously returned iterators and data pointers.
    ///
    /// @pre size must not be negative.
    ///
    auto Resize(SizeType const size, Bool const clear) -> void
    {
        if (size < 0)
        {
            return;
        }

        Reserve(size);
        SetSize(size);

        if (clear)
        {
            if (auto const oldSize = GetSize(); oldSize < size)
            {
                auto const data = GetMutableData();
                for (auto i = oldSize; i < size; ++i)
                {
                    data[i] = ValueType();
                }
            }
        }
    }

    ///
    /// @brief Clear string.
    ///
    auto Clear() noexcept -> void
    {
        SetSize(0);
    }

    ///
    /// @brief
    ///
    /// @param begin
    /// @param end
    ///
    auto GetSubString(SizeType const& begin, SizeType const& end) const noexcept -> StringData
    {
        if (begin < 0 || begin > end || end > GetSize())
        {
            return {};
        }

        auto const data = GetImmutableData();
        auto const newSize = (end - begin);
        auto const newCapacity = (GetCapacity() - begin);

        if (IsSmallString())
        {
            auto ret = StringData();
            ret.Resize(newSize, false);
            auto const newData = (data + begin);
            DataFunction::MemoryCopy(ret.GetMutableData(), newData, newSize);
            return ret;
        }
        else
        {
            auto ret = StringData();
            ret._small.~SmallString();
            auto const newData = _large.data.get() + static_cast<ptrdiff_t>(begin);
            new (&ret._large) LargeString {
                .data = std::shared_ptr<ValueType[]>(_large.data, newData),
                .capacity = static_cast<int64_t>(newCapacity),
                .size = static_cast<int64_t>(newSize),
                .flags = 1 << 7,
            };
            return ret;
        }
    }

private:
    ///
    /// @brief Allocate new shared string buffer and copy data into it.
    ///
    /// @param[in] data Pointer to input string.
    /// @param[in] size Size of input string.
    /// @param[in] capacity Size of buffer allocate.
    ///
    static auto AllocateSharedString(Pointer<ValueType const> const data, auto const size, auto const capacity) -> std::shared_ptr<ValueType[]>
    {
        auto const ptr = std::make_shared<ValueType[]>(static_cast<size_t>(capacity));
        DataFunction::MemoryCopy(Pointer(ptr.get()), data, size);
        return ptr;
    }

    ///
    /// @brief Set size without modifying data.
    ///
    /// @param[in] size New size.
    ///
    auto SetSize(SizeType const size) -> void
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


private:
    struct LargeString
    {
        std::shared_ptr<ValueType[]> data; ///< Large string buffer.
        int64_t capacity = 0;              ///< Capacity of UTF-8 buffer.
        int64_t size = 0;                  ///< Size of UTF-8 buffer.
        uint8_t padding[7] = {0};          ///< Padding.
        uint8_t flags = 0;                 ///< Flag bits.
    };

    struct SmallString
    {
        ValueType data[sizeof(LargeString) / sizeof(ValueType) - 1] = {0}; ///< Small string buffer.
        ValueType size = 0;                                                ///< Size and flags.
    };

    static_assert(sizeof(LargeString) % sizeof(ValueType) == 0);
    static_assert(sizeof(LargeString) == sizeof(SmallString));

    union
    {
        SmallString _small; ///< Small string data.
        LargeString _large; ///< Large string data.
    };
};
}
