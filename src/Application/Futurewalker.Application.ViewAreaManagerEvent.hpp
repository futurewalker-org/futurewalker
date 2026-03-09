// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ViewAreaManagerEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class ViewAreaManagerEvent : public EventParameter
{
public:
    class GeometryChanged;
};

///
/// @brief
///
class ViewAreaManagerEvent::GeometryChanged final : public ViewAreaManagerEvent
{
};
}
}
