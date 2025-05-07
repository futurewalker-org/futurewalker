// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformWindowContextType.hpp"
#include "Futurewalker.Application.PlatformWindow.hpp"

#include "Futurewalker.Application.WindowType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Window context.
///
class PlatformWindowContext : NonCopyable
{
public:
    virtual ~PlatformWindowContext() = 0;
    virtual auto MakePlatformWindow(PlatformWindowOptions const& options, PlatformWindow::Delegate const& delegate) -> Shared<PlatformWindow> = 0;
};
}
}
