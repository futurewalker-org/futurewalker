// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformApplicationContextType.hpp"
#include "Futurewalker.Application.PlatformApplication.hpp"
#include "Futurewalker.Application.PlatformMainThread.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform application context.
///
class PlatformApplicationContext : NonCopyable
{
public:
    virtual ~PlatformApplicationContext() = 0;
    virtual auto MakeApplication(PlatformApplication::Delegate delegate) -> Shared<PlatformApplication> = 0;
    virtual auto GetCurrentApplication() -> Shared<PlatformApplication> = 0;
    virtual auto RunApplication(Shared<PlatformApplication> app, Function<void()> cleanup) -> Async<void> = 0;
};
}
}
