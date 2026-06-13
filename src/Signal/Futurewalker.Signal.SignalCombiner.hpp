// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Signal.Prelude.hpp"

#include "Futurewalker.Async.Async.hpp"
#include "Futurewalker.Async.Lazy.hpp"
#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.TypeTraits.hpp"

#include <cassert>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class>
struct SignalCombinerVoid
{
    using ResultType = void;

    template <class... Args>
    auto operator()(auto&& slots, Args&&... args) -> ResultType
    {
        for (auto&& slot : slots)
        {
            try
            {
                slot(args...);
            }
            catch (...)
            {
                assert(false);
            }
        }
    }
};

///
/// @brief A signal combiner which returns disjunction of each slot's result.
///
template <class>
struct SignalCombinerAnyOf
{
    using ResultType = Bool;

    template <class... Args>
    auto operator()(auto&& slots, Args&&... args) -> ResultType
    {
        auto result = False;
        for (auto&& slot : slots)
        {
            try
            {
                if (auto r = slot(args...))
                {
                    if (*r)
                    {
                        result = true;
                    }
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
template <class>
struct AsyncSignalCombinerAnyOf
{
    using ResultType = Lazy<Bool>;

    template <class... Args>
    auto operator()(auto&& slots, Args&&... args) const -> ResultType
    {
        auto result = []() -> Lazy<Bool> { co_return false; }();
        for (auto&& slot : slots)
        {
            try
            {
                if (auto awaitable = slot(args...))
                {
                    // NOTE: We're referencing stack-allocated caches at this point, so making this function coroutine causes stack use-after-free.
                    // Instead of co_awaiting result directly, return new coroutine which awaits each awaitable when co_awaited.
                    result = AsyncFunction::Fn([result = std::move(result), awaitable = std::move(*awaitable)]() mutable -> Lazy<Bool> {
                        auto const b1 = co_await std::move(result);
                        auto const b2 = co_await std::move(awaitable);
                        co_return b1 || b2;
                    });
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
}
}
