// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.Concepts.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.TypeTraits.hpp"
#include "Futurewalker.Core.Concepts.hpp"

namespace FW_TYPE_TRAITS_DETAIL_NS
{
///
/// @brief Get awaiter object by co_await on an awaitable object.
///
template <class Awaitable, class Promise>
requires Concepts::Awaitable<Awaitable, Promise>
auto GetAwaiter(Awaitable&& awaitable, Promise& promise) -> decltype(auto)
{
    if constexpr (Concepts::HasAwaitTransform<Promise>)
    {
        return promise.await_transform(std::forward<Awaitable>(awaitable));
    }
    else if constexpr (Concepts::AwaitableByMember<Awaitable>)
    {
        return std::forward<Awaitable>(awaitable).operator co_await();
    }
    else if constexpr (Concepts::AwaitableByFree<Awaitable>)
    {
        return operator co_await(std::forward<Awaitable>(awaitable));
    }
    else
    {
        return std::forward<Awaitable>(awaitable);
    }
}

namespace FW_EXPORT
{
///
/// @brief Returns awaiter type of co_await on awaitable type.
///
/// @tparam Awaitable An awaitable type.
/// @tparam Promise promise_type of current coroutine frame.
///
template <class Awaitable, class Promise>
using AwaiterOf = decltype(GetAwaiter<Awaitable, Promise>(std::declval<Awaitable>(), std::declval<Promise&>()));

///
/// @brief Returns result of co_await on awaitable type.
///
/// @tparam Awaitable An awaitable type.
/// @tparam Promise promise_type of current coroutine frame.
///
template <class Awaitable, class Promise>
using AwaitResultOf = decltype(std::declval<AwaiterOf<Awaitable, Promise>>().await_resume());
}
}
