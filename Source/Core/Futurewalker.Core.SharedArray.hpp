// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.SharedArrayType.hpp"
#include "Futurewalker.Core.PointerIterator.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Exception.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
template <class T>
class SharedArray
{
public:
    using SizeType = SInt64;
    using IndexType = SInt64; 

    SharedArray() noexcept = default;
    SharedArray(SharedArray const&) noexcept = default;
    SharedArray(SharedArray&&) noexcept = default;

    auto operator=(SharedArray const&) noexcept -> SharedArray& = default;
    auto operator=(SharedArray&&) noexcept -> SharedArray& = default;

    auto GetSize() const noexcept -> SizeType
    {
        if (auto const state = GetImmutableState())
        {
            return std::ssize(*state);
        }
        return 0;
    }

    auto IsEmpty() const noexcept -> Bool
    {
        return GetSize() == 0;
    }

    auto PushBack(T const& value) -> void
    {
        GetOrCreateMutableState().push_back(value);
    }

    auto PushBack(T&& value) -> void
    {
        GetOrCreateMutableState().push_back(std::move(value));
    }

    auto GetValueAt(IndexType const index) const -> T const&
    {
        if (auto const state = GetImmutableState())
        {
            if (0 <= index && index < std::ssize(*state))
            {
                return (*state)[static_cast<size_t>(index)];
            }
        }
        throw Exception(ErrorCode::InvalidArgument);
    }

    auto Erase(IndexType const index) -> void
    {
        if (auto const state = GetMutableState())
        {
            if (0 <= index && index < std::ssize(*state))
            {
                state->erase(state->begin() + static_cast<size_t>(index));
            }
        }
    }

    auto Clear() -> void
    {
        if (auto const state = GetMutableState())
        {
            state->clear();
        }
    }

private:
    using State = std::vector<T>;
    Shared<State> _state;

private:
    auto GetImmutableState() const -> Pointer<State const>
    {
        return _state.GetPointer();
    }

    auto GetMutableState() -> Pointer<State>
    {
        if (_state)
        {
            if (_state.GetUseCount() == 1)
            {
                return _state.GetPointer();
            }
            _state = Shared<State>::Make(*_state);
            return _state.GetPointer();
        }
        return nullptr;
    }

    auto GetOrCreateMutableState() -> State&
    {
        if (_state)
        {
            if (_state.GetUseCount() == 1)
            {
                return *_state;
            }
            _state = Shared<State>::Make(*_state);
            return *_state;
        }
        _state = Shared<State>::Make();
        return *_state;
    }
};
}
