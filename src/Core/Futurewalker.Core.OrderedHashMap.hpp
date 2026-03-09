// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Optional.hpp"

#include <unordered_map>

#include <boost/unordered/unordered_flat_map.hpp>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A hash map that preserves insertion order.
///
/// @tparam K The type of the keys.
/// @tparam V The type of the values.
/// @tparam H The hash function type.
/// @tparam P The key equality predicate type.
///
template <class K, class V, class H = std::hash<K>, class P = std::equal_to<K>>
class OrderedHashMap
{
public:
    using KeyType = K;
    using ValueType = V;
    using SizeType = SInt64;
    using PairType = std::pair<K const, V>;

    OrderedHashMap() = default;
    OrderedHashMap(OrderedHashMap const&) = default;
    OrderedHashMap(OrderedHashMap&&) noexcept = default;

    ///
    /// @brief Assignment operator.
    ///
    auto operator=(OrderedHashMap const& other) -> OrderedHashMap&
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
    auto operator=(OrderedHashMap&& other) noexcept -> OrderedHashMap&
    {
        if (this != &other)
        {
            auto tmp = std::move(other);
            Swap(tmp);
        }
        return *this;
    }

    ///
    /// @brief Insert a new key-value pair.
    ///
    /// @param key Key.
    /// @param value Value.
    ///
    /// @return True if inserted, false if key already exists.
    ///
    auto Insert(KeyType const& key, ValueType const& value) -> Bool
    {
        auto const result = _map.emplace(key, GetSize());
        if (result.second)
        {
            try
            {
                _list.emplace_back(key, value);
            }
            catch (...)
            {
                _map.erase(key);
                throw;
            }
            return true;
        }
        return false;
    }

    ///
    /// @brief Replace value for existing key.
    ///
    /// @param key Key.
    /// @param value New value.
    ///
    /// @return True if replaced, false if key does not exist.
    ///
    auto Replace(KeyType const& key, ValueType const& value) -> Bool
    {
        auto const it = _map.find(key);
        if (it != _map.end())
        {
            _list[it->second].second = value;
            return true;
        }
        return false;
    }

    ///
    /// @brief Replace value for existing key if predicate returns true.
    ///
    /// @param key Key.
    /// @param value New value.
    /// @param f Predicate function that takes the existing value and returns a Bool.
    ///
    /// @return True if replaced, false if key does not exist or predicate returned false.
    ///
    template <class F>
    auto ReplaceIf(KeyType const& key, ValueType const& value, F&& f) -> Bool
    {
        auto const it = _map.find(key);
        if (it != _map.end())
        {
            if (std::invoke(f, _list[it->second].second))
            {
                _list[it->second].second = value;
                return true;
            }
        }
        return false;
    }

    ///
    /// @brief Check if the container contains the given key.
    ///
    /// @param key Key to check.
    ///
    auto Contains(KeyType const& key) const -> Bool
    {
        return _map.contains(key);
    }

    ///
    /// @brief Get size of the container.
    ///
    auto GetSize() const noexcept -> SizeType
    {
        return std::ssize(_list);
    }

    ///
    /// @brief Check if the container is empty.
    ///
    auto IsEmpty() const noexcept -> Bool
    {
        return _list.empty();
    }

    ///
    /// @brief Clear the container.
    ///
    auto Clear() -> void
    {
        _list.clear();
        _map.clear();
    }

    ///
    /// @brief Swap contents with another OrderedHashMap.
    ///
    auto Swap(OrderedHashMap& other) noexcept -> void
    {
        _list.swap(other._list);
        _map.swap(other._map);
    }

    ///
    /// @brief Iterate over all key-value pairs in insertion order.
    ///
    /// @param self The container instance.
    /// @param f The function to invoke for each key-value pair.
    ///
    template <class Self, class F>
    auto ForEach(this Self&& self, F&& f)
    {
        for (auto&& pair : std::forward_like<Self>(self._list))
        {
            std::invoke(f, pair);
        }
    }

private:
    std::vector<PairType> _list;
    boost::unordered_flat_map<K, size_t, H, P> _map;
};
}
}
