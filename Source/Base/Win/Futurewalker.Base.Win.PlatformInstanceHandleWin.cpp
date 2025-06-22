// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Win.PlatformInstanceHandleWin.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Constructor.
///
/// @param instanceHandle Instance handle of current process.
///
PlatformInstanceHandleWin::PlatformInstanceHandleWin(HINSTANCE instanceHandle)
  : _instanceHandle {instanceHandle}
{
}

///
/// @brief Get instance handle of current process.
///
/// @return Instance handle.
///
auto PlatformInstanceHandleWin::GetInstanceHandle() const -> HINSTANCE
{
    return _instanceHandle;
}

///
/// @brief Resolve dependency.
///
/// @param instanceHandle
///
auto Locator::Resolver<PlatformInstanceHandleWin>::Resolve(HINSTANCE instanceHandle) -> Shared<PlatformInstanceHandleWin>
{
    return Shared<PlatformInstanceHandleWin>::Make(instanceHandle);
}
}
