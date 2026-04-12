// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWinType.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodContextWinType.hpp"
#include "Futurewalker.Application.Win.PlatformInputEditableWinType.hpp"

#include "Futurewalker.Event.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Range.hpp"
#include "Futurewalker.Core.String.hpp"

#include <wrl/client.h>
#include <msctf.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief TSF text store implementation.
///
/// TODO: Move implementation to PLatformInputMethodWin.
///
class PlatformInputMethodTextStoreWin : NonCopyable
{
public:
    struct Delegate
    {
        Function<Bool(Event<>&)> sendKeyEventDetached;
    };
    static auto Make(Delegate const& delegate, Shared<PlatformInputMethodContextWin> context, HWND hwnd) -> Shared<PlatformInputMethodTextStoreWin>;

    PlatformInputMethodTextStoreWin(PassKey<PlatformInputMethodTextStoreWin>, Delegate const& delegate, Shared<PlatformInputMethodContextWin> context, HWND hwnd);
    ~PlatformInputMethodTextStoreWin();

    auto InsertTextFromKeyEvent(String const& text) -> void;

    auto GetEditable() -> Shared<PlatformInputEditableWin>;
    auto SetEditable(Weak<PlatformInputEditableWin> const& client) -> void;

    auto NotifySelectionChange() -> void;
    auto NotifyTextChange(CodeUnit begin, CodeUnit oldEnd, CodeUnit newEnd) -> void;
    auto NotifyLayoutChange() -> void;

    auto CancelProcessKeyDown() -> void;

private:
    auto GetSelf() -> Shared<PlatformInputMethodTextStoreWin>;
    auto UpdateDocumentMgrFocus() -> void;

    class TextStoreImpl;

private:
    Delegate _delegate;
    Weak<PlatformInputMethodTextStoreWin> _self;
    HWND _hwnd = NULL;
    TfEditCookie _editCookie = TF_INVALID_EDIT_COOKIE;
    Shared<PlatformInputMethodContextWin> _platformContext;
    Weak<PlatformInputEditableWin> _editable;
    Microsoft::WRL::ComPtr<ITfDocumentMgr> _documentMgr;
    Microsoft::WRL::ComPtr<ITfContext> _context;
    Microsoft::WRL::ComPtr<ITextStoreACP2> _textStore;
};
}
}
