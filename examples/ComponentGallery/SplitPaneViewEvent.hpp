#pragma once

#include <Futurewalker.Application.ViewEvent.hpp>

namespace Futurewalker::ComponentGallery
{
class SplitPaneViewEvent : public ViewEvent
{
public:
    auto GetIndex() const -> SInt64
    {
        return _index;
    }

    auto SetIndex(SInt64 const index) -> void
    {
        _index = index;
    }

private:
    SInt64 _index = 0;
};
}
