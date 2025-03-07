// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"

#include <cstring>

namespace FW_DETAIL_NS 
{
namespace FW_EXPORT
{
///
/// @brief Data utilities.
///
class DataFunction final : NonConstructible
{
public:
    template <class T, class U>
    static void CopyMemory(Pointer<U> dst, Pointer<T const> src, SInt64 const size) noexcept;
};

///
/// @brief Copy range of memory.
///
/// A wrapper of memcpy, which is easier to work with Pointer<T>.
///
/// @param[in] dst Destination memory location.
/// @param[in] src Source memory location.
/// @param[in] size Bytes to copy.
///
/// @note `dst` and `src` should be non-null.
/// @note `size` should be greater than zero.
///
template <class T, class U>
void DataFunction::CopyMemory(Pointer<U> dst, Pointer<T const> src, SInt64 const size) noexcept
{
    if (dst && src && size > 0)
    {
        std::memcpy(static_cast<U*>(dst), static_cast<T const*>(src), static_cast<size_t>(size));
    }
}
}
}
