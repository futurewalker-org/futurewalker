// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.View.hpp>

namespace Futurewalker::Gallery
{
class TypographyPage : public View
{
public:
    static auto Make() -> Shared<TypographyPage>;

    TypographyPage(PassKey<View> key);

private:
    auto Initialize() -> void override;
};
}
