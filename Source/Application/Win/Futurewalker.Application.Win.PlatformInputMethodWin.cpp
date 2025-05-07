// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformInputMethodWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodEditableWin.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Set editable.
///
auto PlatformInputMethodWin::SetEditable(Shared<PlatformInputMethodEditable> const& editable) -> void
{
    if (auto textStore = GetTextStore())
    {
        if (auto win = editable.Maybe<PlatformInputMethodEditableWin>())
        {
            textStore->SetEditable(win);
        }
    }
}

///
/// @brief Set text store.
///
auto PlatformInputMethodWin::SetTextStore(Weak<PlatformInputMethodTextStoreWin> textStore) -> void
{
    _textStore = textStore;
}

///
/// @brief Get text store.
///
auto PlatformInputMethodWin::GetTextStore() -> Shared<PlatformInputMethodTextStoreWin>
{
    return _textStore.Lock();
}
}
}
