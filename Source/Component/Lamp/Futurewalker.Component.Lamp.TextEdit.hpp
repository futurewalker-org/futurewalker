// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.TextEditType.hpp"
#include "Futurewalker.Component.Lamp.TextEditStyle.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.TextEditType.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class TextEdit : public View
{
public:
    static auto Make() -> Shared<TextEdit>;

    auto GetText() const -> String;
    auto SetText(String const& text) -> void;

    TextEdit(PassKey<View> key);

protected:
    auto Initialize() -> void override;

private:
    Shared<::FW_NS::TextEdit> _textEdit;
};
}
}
