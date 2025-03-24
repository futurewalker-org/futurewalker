// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.ResumesOnSameThreadTaskType.hpp"
#include "Futurewalker.Async.AwaitableTraits.hpp"
#include "Futurewalker.Async.LazyTask.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Wrapper type for a task which will resume on the same thread.
///
template <class T>
struct ResumesOnSameThreadTask : public LazyTask<T>
{
    ResumesOnSameThreadTask(LazyTask<T> task)
      : LazyTask<T> {std::move(task)}
    {
    }
};

///
/// @brief Specialization of AwaitableTraits.
///
template <class T>
struct AwaitableTraits<ResumesOnSameThreadTask<T>>
{
    static constexpr bool ResumesOnSameThread = true;
};
}
