// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.SharedArrayType.hpp"
#include "Futurewalker.Core.SharedArrayBuffer.hpp"
#include "Futurewalker.Core.Exception.hpp"

#include <new>
#include <span>
#include <utility>

namespace FW_DETAIL_NS
{
///
/// @brief Copy-on-write dynamic array with shared storage.
///
/// Dynamic array backed by a reference-counted contiguous block.
///
template <class T>
class SharedArray
{
public:
    using SizeType = SInt64;
    using IndexType = SInt64;

    ///
    /// @brief Default constructor.
    ///
    SharedArray() noexcept = default;

    ///
    /// @brief Copy constructor.
    ///
    SharedArray(SharedArray const& other) noexcept
      : _buffer {other._buffer}
    {
        if (_buffer)
        {
            _buffer->Retain();
        }
    }

    ///
    /// @brief Move constructor.
    ///
    SharedArray(SharedArray&& other) noexcept
      : _buffer {other._buffer}
    {
        other._buffer = nullptr;
    }

    ///
    /// @brief Copy assignment operator. 
    ///
    auto operator=(SharedArray const& other) noexcept -> SharedArray&
    {
        if (this != &other)
        {
            if (_buffer)
            {
                _buffer->Release();
            }

            if (other._buffer)
            {
                other._buffer->Retain();
            }
            _buffer = other._buffer;
        }
        return *this;
    }

    ///
    /// @brief Move assignment operator. 
    ///
    auto operator=(SharedArray&& other) noexcept -> SharedArray&
    {
        if (this != &other)
        {
            if (_buffer)
            {
                _buffer->Release();
            }
            _buffer = other._buffer;
            other._buffer = nullptr;
        }
        return *this;
    }

    ///
    /// @brief Destructor.
    ///
    ~SharedArray() noexcept
    {
        if (_buffer)
        {
            _buffer->Release();
        }
    }

    ///
    /// @brief Get number of elements in array.
    ///
    auto GetSize() const noexcept -> SizeType
    {
        if (_buffer)
        {
            return _buffer->GetSize();
        }
        return 0;
    }

    ///
    /// @brief Get capacity of elements in array.
    ///
    auto GetCapacity() const noexcept -> SizeType
    {
        if (_buffer)
        {
            return _buffer->GetCapacity();
        }
        return 0;
    }

    ///
    /// @brief Check if the array is empty.
    ///
    auto IsEmpty() const noexcept -> Bool
    {
        return GetSize() == 0;
    }

    ///
    /// @brief Push-back element.
    ///
    auto PushBack(T value) -> T&
    {
        auto const buffer = EnsureMutableBufferForSize(GetSize() + 1, true);
        auto const bufferSize = buffer->GetSize();
        auto const slot = buffer->GetData() + bufferSize;
        ::new (static_cast<void*>(slot)) T(std::move(value));
        buffer->SetSize(bufferSize + 1);
        return *slot;
    }

    ///
    /// @brief Reserve capacity.
    ///
    auto Reserve(SizeType const capacity) -> void
    {
        if (capacity > SizeType(0))
        {
            EnsureMutableBufferForSize(static_cast<std::int64_t>(capacity), false);
        }
    }

    ///
    /// @brief Get value at specified index.
    ///
    /// @throw Exception when index was out of range.
    ///
    auto GetValueAt(IndexType const index) const -> T const&
    {
        if (_buffer)
        {
            if (0 <= index && index < _buffer->GetSize())
            {
                return _buffer->GetData()[index];
            }
        }
        throw Exception(ErrorCode::InvalidArgument);
    }

    ///
    /// @brief Get view of elements.
    ///
    auto GetValues() const -> std::span<T const>
    {
        if (_buffer)
        {
            auto const data = static_cast<T const*>(_buffer->GetData());
            auto const size = static_cast<std::size_t>(_buffer->GetSize());
            return std::span<T const>(data, size);
        }
        return {};
    }

    ///
    /// @brief Erase element at specific index.
    ///
    auto Erase(IndexType const index) -> void
    {
        if (!_buffer)
        {
            return;
        }

        auto const size = _buffer->GetSize();
        if (index < 0 || size <= index)
        {
            return;
        }

        auto const buffer = EnsureMutableBufferForSize(size, false);
        auto const data = buffer->GetData();
        for (auto i = index; i < size - 1; ++i)
        {
            data[i] = std::move(data[i + 1]);
        }
        data[size - 1].~T();
        buffer->SetSize(size - 1);
    }

    ///
    /// @brief Erase elements based on condition.
    ///
    template <class F>
    auto EraseIf(F&& f) -> void
    {
        if (!_buffer || _buffer->GetSize() == 0)
        {
            return;
        }

        auto const buffer = EnsureMutableBufferForSize(_buffer->GetSize(), false);
        auto const data = buffer->GetData();
        auto const size = buffer->GetSize();
        auto write = SizeType(0);
        for (auto read = SizeType(0); read < size; ++read)
        {
            if (f(data[read]))
            {
                continue;
            }
            if (write != read)
            {
                data[write] = std::move(data[read]);
            }
            ++write;
        }
        for (auto k = write; k < size; ++k)
        {
            data[k].~T();
        }
        buffer->SetSize(write);
    }

    ///
    /// @brief Erase element by single swap with last element.
    ///
    auto EraseUnordered(IndexType const index) -> void
    {
        if (!_buffer)
        {
            return;
        }

        auto const size = _buffer->GetSize();
        if (index < 0 || size <= index)
        {
            return;
        }

        auto const buffer = EnsureMutableBufferForSize(size, false);
        auto const data = buffer->GetData();
        auto const last = size - 1;
        if (index != last)
        {
            data[index] = std::move(data[last]);
        }
        data[last].~T();
        buffer->SetSize(last);
    }

    ///
    /// @brief Clear elements.
    ///
    auto Clear() -> void
    {
        if (_buffer)
        {
            if (_buffer->GetUseCount() == 1)
            {
                _buffer->Clear();
            }
            else
            {
                _buffer->Release();
                _buffer = nullptr;
            }
        }
    }

private:
    using BufferType = SharedArrayBuffer<T>;

    ///
    /// @brief Ensure the buffer is uniquely owned with capacity for at least requiredSize elements.
    ///
    /// @param[in] requiredSize New required size of the buffer. Should be larger than current size of buffer.
    /// @param[in] allowGeometricGrowth Allow capacity to glow exponentially to cover required size.
    ///
    /// @return Latest value of _buffer.
    ///
    /// @note This function only resizes the buffer, so requiredSize can be used to assign new capacity with allowGeometricGrowth==false.
    /// @note Caller must assign new capacity to the buffer after constructing new element on returned buffer.
    ///
    auto EnsureMutableBufferForSize(SizeType requiredSize, Bool const allowGeometricGrowth) -> Pointer<BufferType>
    {
        auto const size = _buffer ? _buffer->GetSize() : 0;
        if (requiredSize < size)
        {
            requiredSize = size;
        }

        if (_buffer)
        {
            auto const isUnique = _buffer->GetUseCount() == 1;
            auto const capacity = _buffer->GetCapacity();

            if (isUnique && capacity >= requiredSize)
            {
                return _buffer;
            }

            auto requiredCapacity = requiredSize;
            if (allowGeometricGrowth)
            {
                requiredCapacity = capacity == 0 ? 1 : capacity;
                while (requiredSize > requiredCapacity)
                {
                    requiredCapacity *= 2;
                }
            }

            auto buffer = Pointer<BufferType>(nullptr);
            if (isUnique)
            {
                buffer = BufferType::CreateAndMoveFrom(*_buffer, requiredCapacity);
            }
            else
            {
                buffer = BufferType::CreateAndCopyFrom(*_buffer, requiredCapacity);
            }
            _buffer->Release();
            _buffer = buffer;
        }
        else
        {
            _buffer = BufferType::Create(requiredSize);
        }
        return _buffer;
    }

private:
    Pointer<BufferType> _buffer = nullptr;
};
}
