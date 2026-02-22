// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformInputMethodWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputEditableWin.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Set editable.
///
auto PlatformInputMethodWin::SetEditable(Shared<PlatformInputEditable> const& editable) -> void
{
    if (auto textStore = GetTextStore())
    {
        textStore->SetEditable(editable.TryAs<PlatformInputEditableWin>());
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
