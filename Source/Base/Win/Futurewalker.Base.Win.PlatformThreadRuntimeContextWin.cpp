// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Win.PlatformThreadRuntimeContextWin.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include <roapi.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
PlatformThreadRuntimeContextWin::PlatformThreadRuntimeContextWin(PlatformThreadRuntimeInitTypeWin initType)
{
    auto const roInit = initType == PlatformThreadRuntimeInitTypeWin::SingleThreaded ? RO_INIT_SINGLETHREADED : RO_INIT_MULTITHREADED;
    auto const hr = Windows::Foundation::Initialize(roInit);
    if (SUCCEEDED(hr))
    {
        if (hr == S_FALSE)
        {
            FW_DEBUG_LOG_WARNING("Windows::Foundation::Initialize(): Current thread was already initialized.");
        }
    }
    else
    {
        FW_DEBUG_LOG_ERROR("Windows::Foundation::Initialize() failed: hr={:x}", hr);
        throw Exception(ErrorCode::Failure);
    }
}

PlatformThreadRuntimeContextWin::~PlatformThreadRuntimeContextWin()
{
    Windows::Foundation::Uninitialize();
}
}
}
