// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.WindowAreaManagerEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief 
///
class WindowAreaManagerEvent : public EventParameter
{
public:
    class TitleBarAreaRectChanged;
    class TitleBarAvailableAreaRectChanged;
    class ContentAreaRectChanged;
};

///
/// @brief
///
class WindowAreaManagerEvent::TitleBarAreaRectChanged : public WindowAreaManagerEvent
{
};

///
/// @brief
///
class WindowAreaManagerEvent::TitleBarAvailableAreaRectChanged : public WindowAreaManagerEvent
{
};

///
/// @brief
///
class WindowAreaManagerEvent::ContentAreaRectChanged : public WindowAreaManagerEvent
{
};
}
}
