// SPDX-License-Identifier: MPL-2.0
#pragma once

#include <Futurewalker.Application.Application.hpp>

namespace Futurewalker::ThemeViewer
{
class ThemeViewerApplication : public Application
{
public:
    static auto Make() -> Shared<ThemeViewerApplication>;

    ThemeViewerApplication(PassKey<Application> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
};
}
