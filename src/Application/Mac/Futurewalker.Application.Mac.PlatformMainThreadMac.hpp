// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformMainThread.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief macOS implementation of the main thread.
///
class PlatformMainThreadMac : public PlatformMainThread
{
public:
    auto IsMainThread() const -> Bool override;
    auto Schedule() -> AsyncTask<void> override;
    auto ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void> override;
};

template <>
struct Locator::Resolver<PlatformMainThreadMac>
{
    using Interface = PlatformMainThread;
    static auto Resolve() -> Shared<PlatformMainThreadMac>;
};
}
}
