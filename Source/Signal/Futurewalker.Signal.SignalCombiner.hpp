// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Signal.Prelude.hpp"

#include "Futurewalker.Async.Async.hpp"
#include "Futurewalker.Async.Lazy.hpp"
#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.TypeTraits.hpp"

#include <boost/signals2.hpp>

#include <cassert>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A signal combiner which returns disjunction of each slot's result.
///
template <class T>
struct SignalCombinerAnyOf
{
    using result_type = Bool;

    template <class It>
    auto operator()(It begin, It end) const -> Bool
    {
        Bool result = false;
        for (auto it = begin; it != end; ++it)
        {
            try
            {
                if (*it)
                {
                    result = true;
                }
            }
            catch (...)
            {
                assert(false);
            }
        }
        return result;
    }
};

///
/// @brief A signal combiner which returns disjunction of each slot's result.
///
template <class T>
struct AsyncSignalCombinerAnyOf
{
    using result_type = Lazy<Bool>;

    template <class It>
    auto operator()(It begin, It end) const -> Lazy<Bool>
    {
        auto result = []() -> Lazy<Bool> { co_return false; }();
        for (auto it = begin; it != end; ++it)
        {
            try
            {
                // HACK: Waiting patch to support move-only types.
                auto& ref = const_cast<TypeTraits::RemoveCV<decltype(*it)>>(*it);

                // NOTE: We're referencing stack-allocated caches at this point, so making this function coroutine causes stack use-after-free.
                // Instead of co_awaiting result directly, return new coroutine which awaits each awaitable when co_awaited.
                result = AsyncFunction::Fn([result = std::move(result), awaitable = std::move(ref)]() mutable -> Lazy<Bool> {
                    auto const b1 = co_await std::move(result);
                    auto const b2 = co_await std::move(awaitable);
                    co_return b1 || b2;
                });
            }
            catch (...)
            {
                assert(false);
            }
        }
        return result;
    }
};
}
}
