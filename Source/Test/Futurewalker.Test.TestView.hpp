// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TestView : public View
{
public:
    SInt64 _visibleChangeCount = 0;
    SInt64 _enabledChangeCount = 0;
    SInt64 _activeChangeCount = 0;
    SInt64 _displayScaleChangeCount = 0;
    SInt64 _backingScaleChangeCount = 0;
    SInt64 _layoutDirectionChangeCount = 0;
    SInt64 _attachCount = 0;
    SInt64 _detachCount = 0;

    static auto Make() -> Shared<TestView>
    {
        return View::MakeDerived<TestView>();
    }

    TestView(PassKey<View> key)
      : View(key)
    {
    }

    void Initialize() override
    {
        EventReceiver::Connect(*this, *this, &TestView::ReceiveEvent);
    }

    auto ReceiveEvent(Event& event) -> Async<Bool>
    {
        if (event.Is<ViewEvent::VisibleChanged>())
        {
            ++_visibleChangeCount;
        }
        else if (event.Is<ViewEvent::EnabledChanged>()) 
        {
            ++_enabledChangeCount;
        }
        else if (event.Is<ViewEvent::ActiveChanged>()) 
        {
            ++_activeChangeCount;
        }
        else if (event.Is<ViewEvent::DisplayScaleChanged>()) 
        {
            ++_displayScaleChangeCount;
        }
        else if (event.Is<ViewEvent::BackingScaleChanged>()) 
        {
            ++_backingScaleChangeCount;
        }
        else if (event.Is<ViewEvent::LayoutDirectionChanged>()) 
        {
            ++_layoutDirectionChangeCount;
        }
        else if (event.Is<ViewEvent::Attaching>()) 
        {
            ++_attachCount;
        }
        else if (event.Is<ViewEvent::Detaching>()) 
        {
            ++_detachCount;
        }
        co_return false;
    }

    using View::InvalidateLayout;
    using View::InvalidateVisual;

    using View::GetChildCount;
    using View::GetChildIndex;
    using View::GetChildAt;
    using View::GetChildren;

    using View::AddChildAt;
    using View::AddChildBack;
    using View::AddChildFront;
};
}
}
