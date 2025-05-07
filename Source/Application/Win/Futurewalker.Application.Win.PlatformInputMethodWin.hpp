// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.PlatformInputMethodWinType.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWinType.hpp"
#include "Futurewalker.Application.PlatformInputMethod.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Input method.
///
class PlatformInputMethodWin : public PlatformInputMethod
{
public:
    auto SetEditable(Shared<PlatformInputMethodEditable> const& editable) -> void override;

    auto SetTextStore(Weak<PlatformInputMethodTextStoreWin> textStore) -> void;

private:
    auto GetTextStore() -> Shared<PlatformInputMethodTextStoreWin>;

private:
    Weak<PlatformInputMethodTextStoreWin> _textStore;
};
}
}
