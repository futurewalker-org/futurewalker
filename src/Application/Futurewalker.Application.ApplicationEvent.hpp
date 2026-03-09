// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Event.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Application event.
///
class ApplicationEvent : public EventParameter
{
public:
    ApplicationEvent() = default;
    ApplicationEvent(ApplicationEvent const&) = default;
    ApplicationEvent& operator=(ApplicationEvent const&) = default;

public:
    class Started;
    class ExitRequested;
    class Exited;
    class ActiveChanged;
    class ForegroundChanged;
};

///
/// @brief
///
class ApplicationEvent::Started final : public ApplicationEvent
{
};

///
/// @brief
///
class ApplicationEvent::ExitRequested final : public ApplicationEvent
{
public:
    auto IsCancelled() const -> Bool;
    auto SetCancelled(Bool const cancelled) -> void;

private:
    Bool _cancelled = false;
};

///
/// @brief
///
class ApplicationEvent::Exited final : public ApplicationEvent
{
};

///
/// @brief
///
class ApplicationEvent::ActiveChanged final : public ApplicationEvent
{
public:
    auto IsActive() const -> Bool;
    auto SetActive(Bool const active) -> void;

private:
    Bool _active = false;
};

///
/// @brief
///
class ApplicationEvent::ForegroundChanged final : public ApplicationEvent
{
public:
    auto IsForeground() const -> Bool;
    auto SetForeground(Bool const foreground) -> void;

private:
    Bool _foreground = false;
};
}
}
