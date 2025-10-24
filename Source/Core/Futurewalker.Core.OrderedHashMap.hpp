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
    using PairType = std::pair<K, V>;

    OrderedHashMap() = default;
    OrderedHashMap(OrderedHashMap const&) = default;
    OrderedHashMap(OrderedHashMap&&) noexcept = default;

    auto operator=(OrderedHashMap const&) -> OrderedHashMap& = default;
    auto operator=(OrderedHashMap&&) noexcept -> OrderedHashMap& = default;

    auto Insert(SizeType const& index, KeyType const& key, ValueType const& value) -> Bool
    {
        if (0 <= index && index <= GetSize())
        {
            auto const result = _map.emplace(key, index);
            if (result.second)
            {
                _list.insert(_list.begin() + static_cast<ptrdiff_t>(index), std::make_pair(key, value));
                for (size_t i = static_cast<size_t>(index) + 1; i < _list.size(); ++i)
                {
                    _map[_list[i].first] = i;
                }
                return true;
            }
        }
        return false;
    }

    auto Replace(SizeType const& index, KeyType const& key, ValueType const& value) -> Bool
    {
        if (0 <= index && index < GetSize())
        {
            auto const i = static_cast<size_t>(index);
            auto const it = _map.find(key);
            if (it != _map.end())
            {
                if (it->second == i)
                {
                    _list[i].second = value;
                    return true;
                }
            }
            else
            {
                _map.erase(_list[i].first);
                _list[i] = std::make_pair(key, value);
                _map[key] = i;
                return true;
            }
        }
        return false;
    }

    auto Erase(SizeType const& index) -> Bool
    {
        if (0 <= index && index < GetSize())
        {
            auto const i = static_cast<size_t>(index);
            _map.erase(_list[i].first);
            _list.erase(_list.begin() + static_cast<ptrdiff_t>(i));
            for (size_t j = i; j < _list.size(); ++j)
            {
                _map[_list[j].first] = j;
            }
            return true;
        }
        return false;
    }

    auto GetValue(SizeType const& index) const -> ValueType const&
    {
        return _list.at(static_cast<size_t>(index)).second;
    }

    auto GetValue(SizeType const& index) -> ValueType&
    {
        return _list.at(static_cast<size_t>(index)).second;
    }

    auto GetKey(SizeType const& index) const -> KeyType const&
    {
        return _list.at(static_cast<size_t>(index)).first;
    }

    auto Contains(KeyType const& key) const -> Bool
    {
        return _map.find(key) != _map.end();
    }

    auto Find(KeyType const& key) const -> Optional<SizeType>
    {
        auto const it = _map.find(key);
        if (it != _map.end())
        {
            return static_cast<SizeType>(it->second);
        }
        return {};
    }

    auto begin(this auto&& self)
    {
        return self._list.begin();
    }

    auto end(this auto&& self)
    {
        return self._list.end();
    }

    auto GetSize() const noexcept -> SizeType
    {
        return std::ssize(_list);
    }

    auto IsEmpty() const noexcept -> Bool
    {
        return _list.empty();
    }

    auto Clear() -> void
    {
        _list.clear();
        _map.clear();
    }

    auto operator[](SizeType const index) const -> PairType const&
    {
        return _list.at(static_cast<size_t>(index));
    }

private:
    std::vector<std::pair<K, V>> _list;
    boost::unordered_flat_map<K, size_t, H, P> _map;
};
}
}
