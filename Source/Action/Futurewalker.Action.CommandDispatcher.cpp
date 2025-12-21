// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Action.CommandDispatcher.hpp"
#include "Futurewalker.Action.CommandDispatcherContext.hpp"
#include "Futurewalker.Action.Action.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto CommandDispatcher::Make() -> Unique<CommandDispatcher>
{
    return Unique<CommandDispatcher>::Make();
}

auto CommandDispatcher::Execute(CommandId const& command) -> Bool
{
    auto context = Locator::ResolveWithDefault<CommandDispatcherContext>();
    auto dispatcher = context->GetFirstDispatcher({});
    while (dispatcher)
    {
        if (auto const action = dispatcher->InternalFindAction(command))
        {
            auto enabled = False;
            auto toggled = False;
            if (action->State(enabled, toggled))
            {
                if (enabled)
                {
                    action->Execute();
                    return true;
                }
            }
        }

        if (dispatcher->IsBlocking())
        {
            break;
        }
        dispatcher = dispatcher->_nextDispatcher;
    }
    return false;
}

auto CommandDispatcher::IsEnabled(CommandId const& command) -> Bool
{
    auto context = Locator::ResolveWithDefault<CommandDispatcherContext>();
    auto dispatcher = context->GetFirstDispatcher({});
    while (dispatcher)
    {
        if (auto const action = dispatcher->InternalFindAction(command))
        {
            auto enabled = False;
            auto toggled = False;
            if (action->State(enabled, toggled))
            {
                return enabled;
            }
        }
        dispatcher = dispatcher->_nextDispatcher;
    }
    return false;
}

auto CommandDispatcher::IsToggled(CommandId const& command) -> Bool
{
    auto context = Locator::ResolveWithDefault<CommandDispatcherContext>();
    auto dispatcher = context->GetFirstDispatcher({});
    while (dispatcher)
    {
        if (auto const action = dispatcher->InternalFindAction(command))
        {
            auto enabled = False;
            auto toggled = False;
            if (action->State(enabled, toggled))
            {
                return toggled;
            }
        }
        dispatcher = dispatcher->_nextDispatcher;
    }
    return false;
}

auto CommandDispatcher::GetObserver(CommandId const& command) -> Unique<CommandObserver>
{
    auto context = Locator::ResolveWithDefault<CommandDispatcherContext>();
    return context->GetObserver({}, command);
}

CommandDispatcher::CommandDispatcher()
{
    _context = Locator::ResolveWithDefault<CommandDispatcherContext>();
}

CommandDispatcher::~CommandDispatcher()
{
    Detach();
}

auto CommandDispatcher::Attach() -> void
{
    Detach();

    if (!_prevDispatcher && !_nextDispatcher)
    {
        if (auto const firstDispatcher = _context->GetFirstDispatcher({}))
        {
            firstDispatcher->_prevDispatcher = this;
            _nextDispatcher = firstDispatcher;
        }
        _context->SetFirstDispatcher({}, this);
    }
    else
    {
        FW_DEBUG_ASSERT(false);
    }

    for (auto const& [command, _] : _actionMap)
    {
        InternalNotifyStateChanged(command);
    }
}

auto CommandDispatcher::Detach() -> void
{
    if (_prevDispatcher)
    {
        _prevDispatcher->_nextDispatcher = _nextDispatcher;
    }

    if (auto firstDispatcher = _context->GetFirstDispatcher({}))
    {
        if (firstDispatcher == this)
        {
            _context->SetFirstDispatcher({}, _nextDispatcher);
        }
    }

    if (_nextDispatcher)
    {
        _nextDispatcher->_prevDispatcher = _prevDispatcher;
    }

    _prevDispatcher = nullptr;
    _nextDispatcher = nullptr;

    for (auto const& [command, _] : _actionMap)
    {
        InternalNotifyStateChanged(command);
    }
}

auto CommandDispatcher::IsBlocking() const -> Bool
{
    return _blocking;
}

auto CommandDispatcher::SetBlocking(Bool const blocking) -> void
{
    if (_blocking != blocking)
    {
        _blocking = blocking;
    }
}

auto CommandDispatcher::AddAction(CommandId const& command, Shared<Action> const& action) -> void
{
    if (command != 0U)
    {
        _actionMap.insert_or_assign(command, action);
        InternalNotifyStateChanged(command);
    }
}

auto CommandDispatcher::RemoveAction(CommandId const& command) -> void
{
    if (command != 0U)
    {
        _actionMap.erase(command);
        InternalNotifyStateChanged(command);
    }
}

auto CommandDispatcher::InternalFindAction(CommandId const& command) -> Shared<Action>
{
    if (auto const it = _actionMap.find(command); it != _actionMap.end())
    {
        return it->second;
    }
    return nullptr;
}

auto CommandDispatcher::InternalSendCommand(CommandId const& command) -> Bool
{
    if (auto const action = InternalFindAction(command))
    {
        auto enabled = False;
        auto toggled = False;
        if (action->State(enabled, toggled))
        {
            if (enabled)
            {
                action->Execute();
                return true;
            }
        }
    }
    return false;
}

auto CommandDispatcher::InternalNotifyStateChanged(CommandId const& command) -> void
{
    _context->NotifyStateChanged({}, command);
}
}
