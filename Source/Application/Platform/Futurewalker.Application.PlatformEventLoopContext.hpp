// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformEventLoopContextType.hpp"
#include "Futurewalker.Application.PlatformEventLoopType.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT 
{
///
/// @brief Event loop context.
///
class PlatformEventLoopContext : NonCopyable
{
public:
    virtual ~PlatformEventLoopContext() = 0;

    ///
    /// @brief Obtain event loop for current thread.
    ///
    virtual auto GetEventLoopForThread() -> Shared<PlatformEventLoop> = 0;
};
}
}
