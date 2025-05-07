// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformInputEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Core.String.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformInputEvent : public EventParameter
{
public:
    class InsertText;
    class InsertCompositionText;
    class SelectionChange;
    class CompositionStart;
    class CompositionEnd;
    class CompositionUpdate;
};

class PlatformInputEvent::InsertText final : public PlatformInputEvent
{
public:
    auto GetText() const -> String { return _text; };
    auto SetText(String const& text) -> void { _text = text; }

private:
    String _text;
};

class PlatformInputEvent::InsertCompositionText final : public PlatformInputEvent
{
public:
    auto GetText() const -> String { return _text; }
    auto SetText(String const& text) -> void { _text = text; }

private:
    String _text;
};

class PlatformInputEvent::SelectionChange final : public PlatformInputEvent
{
};

class PlatformInputEvent::CompositionStart final : public PlatformInputEvent
{
};

class PlatformInputEvent::CompositionEnd final : public PlatformInputEvent
{
};

class PlatformInputEvent::CompositionUpdate final : public PlatformInputEvent
{
};
}
}
