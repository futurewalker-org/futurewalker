// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformMainThread.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include <thread>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Win32 implementation of PlatformMainThread.
///
class PlatformMainThreadWin : public PlatformMainThread
{
public:
    PlatformMainThreadWin();
    auto IsMainThread() const -> Bool override;
    auto Schedule() -> AsyncTask<void> override;
    auto ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void> override;
};

///
/// @brief
///
template <>
struct Locator::Resolver<PlatformMainThreadWin>
{
    using Interface = PlatformMainThread;
    static auto Resolve() -> Shared<PlatformMainThreadWin>;
};
}
}
