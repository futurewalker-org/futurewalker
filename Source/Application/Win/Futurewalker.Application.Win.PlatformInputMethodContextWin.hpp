// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformInputMethodContextWinType.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWin.hpp"
#include "Futurewalker.Application.PlatformInputMethodContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Pointer.hpp"

#include <wrl/client.h>
#include <msctf.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Input method context.
///
class PlatformInputMethodContextWin : public PlatformInputMethodContext
{
public:
    static auto Make() -> Shared<PlatformInputMethodContextWin>;

    PlatformInputMethodContextWin(PassKey<PlatformInputMethodContextWin>);
    ~PlatformInputMethodContextWin();

    auto MakeEditable(PlatformInputMethodEditable::Delegate const& delegate) -> Shared<PlatformInputMethodEditable> override;

    auto GetClientId() const -> TfClientId;
    auto CreateDocumentMgr() -> Microsoft::WRL::ComPtr<ITfDocumentMgr>;
    auto AssociateFocus(HWND hwnd, Pointer<ITfDocumentMgr> documentMgr) -> void;

    auto MakeTextStore(HWND hwnd) -> Shared<PlatformInputMethodTextStoreWin>;

private:
    auto GetSelf() -> Shared<PlatformInputMethodContextWin>;
    auto SetSelf(Shared<PlatformInputMethodContextWin> self) -> void;

private:
    TfClientId _clientId = 0;
    Weak<PlatformInputMethodContextWin> _self;
    Microsoft::WRL::ComPtr<ITfThreadMgr> _threadMgr;
    Microsoft::WRL::ComPtr<ITfSource> _threadMgrSource;
};

template <>
struct Locator::Resolver<PlatformInputMethodContextWin>
{
    using Interface = PlatformInputMethodContext;
    static auto Resolve() -> Shared<PlatformInputMethodContextWin>;
};
}
}
