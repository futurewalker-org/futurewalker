// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformInputMethodContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputEditableWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

#include <wrl/module.h>
#include <olectl.h>
#include <tsattrs.h>

namespace FW_DETAIL_NS
{
auto PlatformInputMethodContextWin::Make() -> Shared<PlatformInputMethodContextWin>
{
    auto context = Shared<PlatformInputMethodContextWin>::Make(PassKey<PlatformInputMethodContextWin>());
    context->SetSelf(context);
    return context;
}

PlatformInputMethodContextWin::PlatformInputMethodContextWin(PassKey<PlatformInputMethodContextWin>)
{
    auto hr = ::CoCreateInstance(CLSID_TF_ThreadMgr, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(_threadMgr.GetAddressOf()));
    if (FAILED(hr) || !_threadMgr)
    {
        FW_DEBUG_LOG_ERROR("Failed to create ITfThreadMgr");
        throw Exception(ErrorCode::Failure);
    }

    hr = _threadMgr->QueryInterface(IID_PPV_ARGS(_threadMgrSource.GetAddressOf()));
    if (FAILED(hr) || !_threadMgrSource)
    {
        FW_DEBUG_LOG_ERROR("Failed to activate ITfThreadMgr");
    }

    hr = _threadMgr->Activate(&_clientId);
    if (FAILED(hr))
    {
        FW_DEBUG_LOG_ERROR("Failed to activate ITfThreadMgr");
    }
}

PlatformInputMethodContextWin::~PlatformInputMethodContextWin()
{
    _threadMgr->Deactivate();
}

auto PlatformInputMethodContextWin::MakeEditable(PlatformInputEditable::Delegate const& delegate) -> Shared<PlatformInputEditable>
{
    return PlatformInputEditableWin::Make(delegate);
}

auto PlatformInputMethodContextWin::GetClientId() const -> TfClientId
{
    return _clientId;
}

auto PlatformInputMethodContextWin::GetThreadMgr() const -> Microsoft::WRL::ComPtr<ITfThreadMgr>
{
    return _threadMgr;
}

auto PlatformInputMethodContextWin::CreateDocumentMgr() -> Microsoft::WRL::ComPtr<ITfDocumentMgr>
{
    Microsoft::WRL::ComPtr<ITfDocumentMgr> documentMgr;
    auto const hr = _threadMgr->CreateDocumentMgr(documentMgr.GetAddressOf());
    if (FAILED(hr) || !documentMgr)
    {
        FW_DEBUG_LOG_ERROR("Failed to create ITfDocumentMgr");
        return {};
    }
    return documentMgr;
}

auto PlatformInputMethodContextWin::AssociateFocus(HWND hwnd, Pointer<ITfDocumentMgr> documentMgr) -> void
{
    Microsoft::WRL::ComPtr<ITfDocumentMgr> previousDocumentMgr;
    auto const hr = _threadMgr->AssociateFocus(hwnd, static_cast<ITfDocumentMgr*>(documentMgr), previousDocumentMgr.GetAddressOf());
    if (FAILED(hr))
    {
        FW_DEBUG_LOG_ERROR("Failed to associate focus with ITfDocumentMgr");
    }
}

auto PlatformInputMethodContextWin::MakeTextStore(PlatformInputMethodTextStoreWin::Delegate const& delegate, HWND hwnd) -> Shared<PlatformInputMethodTextStoreWin>
{
    return PlatformInputMethodTextStoreWin::Make(delegate, GetSelf(), hwnd);
}

auto PlatformInputMethodContextWin::GetSelf() -> Shared<PlatformInputMethodContextWin>
{
    return _self.Lock();
}

auto PlatformInputMethodContextWin::SetSelf(Shared<PlatformInputMethodContextWin> self) -> void
{
    _self = self;
}

auto Locator::Resolver<PlatformInputMethodContextWin>::Resolve() -> Shared<PlatformInputMethodContextWin>
{
    return PlatformInputMethodContextWin::Make();
}
}
