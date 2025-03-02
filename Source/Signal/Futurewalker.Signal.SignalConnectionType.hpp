// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class SignalConnection;
class ScopedSignalConnection;

///
/// @brief Position of signal connection.
///
enum class SignalConnectPosition
{
    front, ///< Before other connections.
    back,  ///< After other connections.
};
}
}
