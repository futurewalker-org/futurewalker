// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <list>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class FocusNode;
using FocusNodeList = std::list<Shared<FocusNode>>;

///
/// @brief Focus reason.
//
enum class FocusReason
{
    Other = 0,
    Pointer,  ///< Gained focus due to pointer interaction (e.g. mouse click or touch).
    Keyboard, ///< Gained focus due to keyboard navigation (e.g. Tab key).
};
}
}
