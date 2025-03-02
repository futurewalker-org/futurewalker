// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.Prelude.hpp"
#include "Futurewalker.Async.AwaitableTraits.hpp"

#include "Futurewalker.Core.Concepts.hpp"

#include <chrono>
#include <coroutine>
#include <stop_token>

namespace FW_CONCEPTS_DETAIL_NS
{
// clang-format off
struct AwaitTransformCheckerHelper { void await_transform() {} };
template <class Promise>
struct AwaitTransformChecker : AwaitTransformCheckerHelper, Promise {};
template <class T>
concept AwaitResumeResult = SameAs<T ,void> || ImplicitlyConvertibleTo<T, bool> || SpecializationOf<T, std::coroutine_handle>;
// clang-format on

namespace FW_EXPORT
{
///
/// @brief Concept of types which can be awaited by member operator.
///
template <class A>
concept AwaitableByMember = requires(A&& a) { std::forward<A>(a).operator co_await(); };

///
/// @brief Concept of types which can be awaited by free operator.
///
template <class A>
concept AwaitableByFree = requires(A&& a) { operator co_await(std::forward<A>(a)); };


///
/// @brief Concept of types which can be awaited without operator co_await.
///
template <class A, class P>
concept Awaiter =  requires(A&& a, P& p) {
    // TODO: test return type of these functions.
    { std::forward<A>(a).await_ready() } -> ImplicitlyConvertibleTo<bool>; // must be convertible to bool.
    { std::forward<A>(a).await_resume() } -> AwaitResumeResult; // -> void or bool or std::coroutine_handle<Z>
    { std::forward<A>(a).await_suspend(std::declval<std::coroutine_handle<P>>()) };
};

///
/// @brief Concept of promise types which have await_transform.
///
/// @note Based on implementation described in P1288R0.
///
template <class P>
concept HasAwaitTransform = !requires { &AwaitTransformChecker<P>::await_transform; };

///
/// @brief Concept of types which can be awaited by await_transform.
///
template <class A, class P>
// clang-format off
concept AwaitableByTransform =
    requires(A&& a, P& p) { { p.await_transform(std::forward<A>(a)) } -> AwaitableByMember; } ||
    requires(A&& a, P& p) { { p.await_transform(std::forward<A>(a)) } -> AwaitableByFree; } ||
    requires(A&& a, P& p) { { p.await_transform(std::forward<A>(a)) } -> Awaiter<P>; };
// clang-format on

///
/// @brief Concept of types which can be awaited.
///
template <class A, class P>
concept Awaitable = AwaitableByTransform<A, P> || (!HasAwaitTransform<P> && (AwaitableByFree<A> || AwaitableByMember<A> || Awaiter<A, P>));

///
/// @brief Will Awaitable resumes on the same thread?
///
template <class A>
concept ResumesOnSameThread = AwaitableTraits<std::decay_t<A>>::ResumesOnSameThread;

///
/// @brief Concept of promise type which supports cancellation.
///
template <class P>
concept CancellablePromise = requires(P p) {
    { p.get_stop_token() } -> SameAs<std::stop_token>;
    { p.cancel() } -> ImplicitlyConvertibleTo<std::coroutine_handle<>>;
};
}
}
