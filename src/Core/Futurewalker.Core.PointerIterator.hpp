// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"
#include "Futurewalker.Core.Pointer.hpp"

#include <boost/stl_interfaces/iterator_interface.hpp>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief An iterator type for pointers which iterate over contiguous buffer.
///
template <class ValueType, class TagType>
class PointerIterator : public boost::stl_interfaces::v2::iterator_interface<PointerIterator<ValueType, TagType>, std::contiguous_iterator_tag, ValueType>
{
    ValueType* _it = nullptr;

public:
    ///
    /// @brief Default constructor.
    ///
    constexpr PointerIterator() noexcept = default;

    ///
    /// @brief Construct from raw pointer. 
    ///
    constexpr PointerIterator(ValueType* it) noexcept
      : _it(it)
    {
    }

    ///
    /// @brief Construct from pointer.
    ///
    constexpr PointerIterator(Pointer<ValueType> ptr) noexcept
      : _it(static_cast<ValueType*>(ptr))
    {
    }

    ///
    /// @brief Conversion from other iterator.
    ///
    template <typename T, class U>
    requires std::is_convertible_v<T*, ValueType*>
    constexpr PointerIterator(PointerIterator<T, U> other) noexcept
      : _it(other._it)
    {
    }

    ///
    /// @brief operator*()
    ///
    constexpr auto& operator*() const noexcept
    {
        return *_it;
    }

    ///
    /// @brief operator+=
    ///
    constexpr auto& operator+=(std::ptrdiff_t i) noexcept
    {
        _it += i;
        return *this;
    }

    ///
    /// @brief operator-
    ///
    constexpr auto operator-(PointerIterator other) const noexcept
    {
        return _it - other._it;
    }

    ///
    /// @brief operator++(int)
    ///
    using boost::stl_interfaces::v2::iterator_interface<PointerIterator<ValueType, TagType>, std::contiguous_iterator_tag, ValueType>::operator++;
};

///
/// @brief An iterator type for pointers which iterate over contiguous buffer.
///
template <class ValueType, class TagType>
using ConstPointerIterator = PointerIterator<std::add_const_t<ValueType>, TagType>;

}
}