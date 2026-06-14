// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Pointer.hpp"

#include <atomic>
#include <cstddef>
#include <new>
#include <utility>

namespace FW_DETAIL_NS
{
///
/// @brief Single-allocation, reference-counted, contiguous storage block backing SharedArray.
///
/// The block is reference counted and element lifetimes in the trailing storage are managed manually by SharedArray.
///
/// The block is one allocation laid out as:
///
///     [ atomic refcount | size | capacity ] [ padding to alignof(T) ] [ T[capacity] ]
///
template <class T>
class SharedArrayBuffer
{
public:
    SharedArrayBuffer(SharedArrayBuffer const&) = delete;
    auto operator=(SharedArrayBuffer const&) -> SharedArrayBuffer& = delete;

    ///
    /// @brief Allocate a block with the given capacity and zero size.
    ///
    /// @note Elements are not constructed.
    ///
    static auto Create(SInt64 const capacity) -> Pointer<SharedArrayBuffer>
    {
        auto const cap = capacity < 0 ? 0 : capacity;
        auto const raw = Allocate(cap);
        return ::new (static_cast<void*>(raw)) SharedArrayBuffer(cap);
    }

    ///
    /// @brief Allocate a block of newCapacity and copy-construct the elements of src into it.
    ///
    static auto CreateAndCopyFrom(SharedArrayBuffer const& src, SInt64 const newCapacity) -> Pointer<SharedArrayBuffer>
    {
        auto const dst = Create(newCapacity);
        auto const from = src.GetData();
        auto const to = dst->GetData();
        auto i = SInt64(0);
        try
        {
            for (; i < src._size; ++i)
            {
                ::new (static_cast<void*>(to + i)) T(from[i]);
            }
        }
        catch (...)
        {
            DestroyRange(to, i);
            Deallocate(dst);
            throw;
        }
        dst->_size = src._size;
        return dst;
    }

    ///
    /// @brief Allocate a block of newCapacity and move-construct the elements of src into it.
    ///
    /// @note src's elements are left in a moved-from (but still alive) state.
    ///
    static auto CreateAndMoveFrom(SharedArrayBuffer& src, SInt64 const newCapacity) -> Pointer<SharedArrayBuffer>
    {
        auto const dst = Create(newCapacity);
        auto const from = src.GetData();
        auto const to = dst->GetData();
        auto i = SInt64(0);
        try
        {
            for (; i < src._size; ++i)
            {
                ::new (static_cast<void*>(to + i)) T(std::move_if_noexcept(from[i]));
            }
        }
        catch (...)
        {
            DestroyRange(to, i);
            Deallocate(dst);
            throw;
        }
        dst->_size = src._size;
        return dst;
    }

    ///
    /// @brief Increment the reference count.
    ///
    auto Retain() noexcept -> void
    {
        _refcount.fetch_add(1, std::memory_order_relaxed);
    }

    ///
    /// @brief Decrement the reference count, destroying the block when it reaches zero.
    ///
    auto Release() noexcept -> void
    {
        if (_refcount.fetch_sub(1, std::memory_order_acq_rel) == 1)
        {
            DestroyRange(GetData(), _size);
            Deallocate(this);
        }
    }

    ///
    /// @brief Current reference count.
    ///
    auto GetUseCount() const noexcept -> SInt64
    {
        return _refcount.load(std::memory_order_acquire);
    }

    ///
    /// @brief Number of constructed elements.
    ///
    auto GetSize() const noexcept -> SInt64
    {
        return _size;
    }

    ///
    /// @brief Number of elements that fit without reallocating.
    ///
    auto GetCapacity() const noexcept -> SInt64
    {
        return _capacity;
    }

    ///
    /// @brief Set the number of constructed elements.
    ///
    /// @note Caller is responsible for the matching construction/destruction.
    ///
    auto SetSize(SInt64 const size) noexcept -> void
    {
       _size = size;
    }

    ///
    /// @brief Pointer to the first element slot.
    ///
    auto GetData() noexcept -> Pointer<T>
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::byte*>(this) + GetDataOffset());
    }

    ///
    /// @brief Pointer to the first element slot.
    ///
    auto GetData() const noexcept -> Pointer<T const>
    {
        return reinterpret_cast<T const*>(reinterpret_cast<std::byte const*>(this) + GetDataOffset());
    }

    ///
    /// @brief Destroy all elements, keeping the allocation and capacity.
    ///
    auto Clear() noexcept -> void
    {
        DestroyRange(GetData(), _size);
        _size = 0;
    }

private:
    explicit SharedArrayBuffer(SInt64 const capacity) noexcept
      : _refcount {1}
      , _size {0}
      , _capacity {capacity}
    {
    }

    static consteval auto GetDataOffset() noexcept -> std::size_t
    {
        constexpr std::size_t header = sizeof(SharedArrayBuffer);
        constexpr std::size_t align = alignof(T);
        return (header + align - 1) / align * align;
    }

    static consteval auto GetAlignment() noexcept -> std::size_t
    {
        return alignof(T) > alignof(SharedArrayBuffer) ? alignof(T) : alignof(SharedArrayBuffer);
    }

    static auto GetAllocSize(SInt64 const capacity) noexcept -> std::size_t
    {
        return GetDataOffset() + sizeof(T) * static_cast<std::size_t>(capacity);
    }

    static auto Allocate(SInt64 const capacity) -> Pointer<SharedArrayBuffer>
    {
        return static_cast<SharedArrayBuffer*>(::operator new(GetAllocSize(capacity), std::align_val_t(GetAlignment())));
    }

    static auto Deallocate(Pointer<SharedArrayBuffer> const buffer) noexcept -> void
    {
        ::operator delete(static_cast<void*>(buffer), std::align_val_t(GetAlignment()));
    }

    static auto DestroyRange(Pointer<T> const first, SInt64 const count) noexcept -> void
    {
        for (auto i = SInt64(0); i < count; ++i)
        {
            first[i].~T();
        }
    }

private:
    std::atomic<int64_t> _refcount = 0;
    SInt64 _size = 0;
    SInt64 _capacity = 0;
};
}
