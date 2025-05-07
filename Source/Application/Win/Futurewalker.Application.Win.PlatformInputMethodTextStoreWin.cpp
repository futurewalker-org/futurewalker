// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodEditableWin.hpp"
#include "Futurewalker.Application.PlatformInputMethodEditable.hpp"
#include "Futurewalker.Application.PlatformTextInputState.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

#include <wrl/module.h>
#include <olectl.h>

namespace FW_DETAIL_NS
{
class PlatformInputMethodTextStoreWin::TextStoreImpl : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, ITextStoreACP2, ITfContextOwnerCompositionSink, ITfMouseTrackerACP>
{
public:
    TextStoreImpl(PlatformInputMethodTextStoreWin& owner);

    /* ITextStoreACP2 */
    STDMETHODIMP AdviseSink(REFIID riid, IUnknown* punk, DWORD dwMask) override;
    STDMETHODIMP UnadviseSink(IUnknown* punk) override;
    STDMETHODIMP RequestLock(DWORD dwLockFlags, HRESULT* phrSession) override;
    STDMETHODIMP GetStatus(TS_STATUS* pdcs) override;
    STDMETHODIMP QueryInsert(LONG acpTestStart, LONG acpTestEnd, ULONG cch, LONG* pacpResultStart, LONG* pacpResultEnd) override;
    STDMETHODIMP GetSelection(ULONG ulIndex, ULONG ulCount, TS_SELECTION_ACP* pSelection, ULONG* pcFetched) override;
    STDMETHODIMP SetSelection(ULONG ulCount, const TS_SELECTION_ACP* pSelection) override;
    STDMETHODIMP GetText(LONG acpStart, LONG acpEnd, WCHAR* pchPlain, ULONG cchPlainReq, ULONG* pcchPlainRet, TS_RUNINFO* prgRunInfo, ULONG cRunInfoReq, ULONG* pcRunInfoRet, LONG* pacpNext) override;
    STDMETHODIMP SetText(DWORD dwFlags, LONG acpStart, LONG acpEnd, const WCHAR* pchText, ULONG cch, TS_TEXTCHANGE* pChange) override;
    STDMETHODIMP GetFormattedText(LONG acpStart, LONG acpEnd, IDataObject** ppDataObject) override;
    STDMETHODIMP GetEmbedded(LONG acpPos, REFGUID rguidService, REFIID riid, IUnknown** ppunk) override;
    STDMETHODIMP QueryInsertEmbedded(const GUID* pguidService, const FORMATETC* pFormatEtc, BOOL* pfInsertable) override;
    STDMETHODIMP InsertEmbedded(DWORD dwFlags, LONG acpStart, LONG acpEnd, IDataObject* pDataObject, TS_TEXTCHANGE* pChange) override;
    STDMETHODIMP InsertTextAtSelection(DWORD dwFlags, const WCHAR* pchText, ULONG cch, LONG* pacpStart, LONG* pacpEnd, TS_TEXTCHANGE* pChange) override;
    STDMETHODIMP InsertEmbeddedAtSelection(DWORD dwFlags, IDataObject* pDataObject, LONG* pacpStart, LONG* pacpEnd, TS_TEXTCHANGE* pChange) override;
    STDMETHODIMP RequestSupportedAttrs(DWORD dwFlags, ULONG cFilterAttrs, const TS_ATTRID* paFilterAttrs) override;
    STDMETHODIMP RequestAttrsAtPosition(LONG acpPos, ULONG cFilterAttrs, const TS_ATTRID* paFilterAttrs, DWORD dwFlags) override;
    STDMETHODIMP RequestAttrsTransitioningAtPosition(LONG acpPos, ULONG cFilterAttrs, const TS_ATTRID* paFilterAttrs, DWORD dwFlags) override;
    STDMETHODIMP FindNextAttrTransition(LONG acpStart, LONG acpHalt, ULONG cFilterAttrs, const TS_ATTRID* paFilterAttrs, DWORD dwFlags, LONG* pacpNext, BOOL* pfFound, LONG* plFoundOffset) override;
    STDMETHODIMP RetrieveRequestedAttrs(ULONG ulCount, TS_ATTRVAL* paAttrVals, ULONG* pcFetched) override;
    STDMETHODIMP GetEndACP(LONG* pacp) override;
    STDMETHODIMP GetActiveView(TsViewCookie* pvcView) override;
    STDMETHODIMP GetACPFromPoint(TsViewCookie vcView, const POINT* ptScreen, DWORD dwFlags, LONG* pacp) override;
    STDMETHODIMP GetTextExt(TsViewCookie vcView, LONG acpStart, LONG acpEnd, RECT* prc, BOOL* pfClipped) override;
    STDMETHODIMP GetScreenExt(TsViewCookie vcView, RECT* prc) override;

    /* ITfContextOwnerCompositionSink */
    STDMETHODIMP OnStartComposition(ITfCompositionView* pComposition, BOOL* pfOk) override;
    STDMETHODIMP OnUpdateComposition(ITfCompositionView* pComposition, ITfRange* pRangeNew) override;
    STDMETHODIMP OnEndComposition(ITfCompositionView* pComposition) override;

    /* ITfMouseTrackerACP */
    STDMETHODIMP AdviseMouseSink(ITfRangeACP* range, ITfMouseSink* pSink, DWORD* pdwCookie) override;
    STDMETHODIMP UnadviseMouseSink(DWORD dwCookie) override;

    auto NotifySelectionChange() -> void;
    auto NotifyTextChange(CodeUnit begin, CodeUnit oldEnd, CodeUnit newEnd) -> void;
    auto NotifyLayoutChange() -> void;

private:
    PlatformInputMethodTextStoreWin& _owner;

private:
    Microsoft::WRL::ComPtr<ITextStoreACPSink> _sink;
    DWORD _sinkMask = 0;
};

PlatformInputMethodTextStoreWin::TextStoreImpl::TextStoreImpl(PlatformInputMethodTextStoreWin& owner)
  : _owner {owner}
{
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::AdviseSink(REFIID riid, IUnknown* punk, DWORD dwMask)
{
    if (_sink)
    {
        // We expect at most one advise sink.
        FW_DEBUG_ASSERT(false);
        return CONNECT_E_ADVISELIMIT;
    }

    if (!::InlineIsEqualGUID(riid, IID_ITextStoreACPSink))
    {
        FW_DEBUG_LOG_ERROR("PlatformInputMethodTextStoreWin::TextStoreImpl::AdviceSink() Failed");
        return E_INVALIDARG;
    }

    if (!punk)
    {
        FW_DEBUG_LOG_ERROR("PlatformInputMethodTextStoreWin::TextStoreImpl::AdviceSink() Failed");
        return E_UNEXPECTED;
    }

    Microsoft::WRL::ComPtr<ITextStoreACPSink> sink;
    if (FAILED(punk->QueryInterface(IID_PPV_ARGS(sink.GetAddressOf()))))
    {
        FW_DEBUG_LOG_ERROR("PlatformInputMethodTextStoreWin::TextStoreImpl::AdviceSink() Failed");
        return E_UNEXPECTED;
    }

    _sink = sink;
    _sinkMask = dwMask;
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::UnadviseSink(IUnknown* punk)
{
    if (_sink.Get() == punk)
    {
        _sink = nullptr;
        _sinkMask = 0;
    }
    else
    {
        FW_DEBUG_LOG_WARNING("PlatfomTextStoreWin::UnadviseSink(): Sink was not installed");
    }
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::RequestLock(DWORD dwLockFlags, HRESULT* phrSession)
{
    if (_sink)
    {
        // TODO: Support proper locking mechanics.
        *phrSession = _sink->OnLockGranted(dwLockFlags);
        return S_OK;
    }
    FW_DEBUG_LOG_INFO("PlatformInputMethodTextStoreWin::TextStoreImpl::RequestLock() Failed");
    return E_UNEXPECTED;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::GetStatus(TS_STATUS* pdcs)
{
    if (pdcs)
    {
        FW_DEBUG_LOG_INFO("PlatformInputMethodTextStoreWin::TextStoreImpl::GetStatus()");
        // Fills TS_STATUS structure.
        // https://learn.microsoft.com/en-us/windows/win32/api/textstor/ns-textstor-ts_status
        pdcs->dwDynamicFlags = 0;
        pdcs->dwStaticFlags = 0;
        return S_OK;
    }
    FW_DEBUG_ASSERT(false);
    return E_INVALIDARG;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::QueryInsert(LONG acpTestStart, LONG acpTestEnd, ULONG cch, LONG* pacpResultStart, LONG* pacpResultEnd)
{
    (void)acpTestEnd;

    if (pacpResultStart && pacpResultEnd)
    {
        *pacpResultStart = acpTestStart;
        *pacpResultEnd = acpTestStart + cch;
        return S_OK;
    }
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::GetSelection(ULONG ulIndex, ULONG ulCount, TS_SELECTION_ACP* pSelection, ULONG* pcFetched)
{
    if (!pSelection || !pcFetched)
    {
        return S_OK;
    }

    if (ulIndex != TF_DEFAULT_SELECTION)
    {
        FW_DEBUG_LOG_ERROR("PlatformInputMethodTextStoreWin::TextStoreImpl::GetSelection() Failed");
        return TS_E_NOSELECTION;
    }

    if (ulCount > 0)
    {
        if (auto editable = _owner.GetEditable())
        {
            auto const selectedRange = editable->GetU16SelectedRange();
            pSelection[0].acpStart = static_cast<LONG>(selectedRange.GetBegin());
            pSelection[0].acpEnd = static_cast<LONG>(selectedRange.GetEnd());
            pSelection[0].style.ase = TS_AE_END;
            pSelection[0].style.fInterimChar = FALSE;
            *pcFetched = 1;
            return S_OK;
        }
    }
    FW_DEBUG_LOG_ERROR("PlatformInputMethodTextStoreWin::TextStoreImpl::GetSelection() Failed");
    return TS_E_NOSELECTION;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::SetSelection(ULONG ulCount, const TS_SELECTION_ACP* pSelection)
{
    if (pSelection && ulCount > 0)
    {
        if (auto editable = _owner.GetEditable())
        {
            auto const& selection = pSelection[0];
            editable->SetU16SelectedRange({selection.acpStart, selection.acpEnd});
        }
    }
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::GetText(
  LONG acpStart,
  LONG acpEnd,
  WCHAR* pchPlain,
  ULONG cchPlainReq,
  ULONG* pcchPlainRet,
  TS_RUNINFO* prgRunInfo,
  ULONG cRunInfoReq,
  ULONG* pcRunInfoRet,
  LONG* pacpNext)
{
    if (auto editable = _owner.GetEditable())
    {
        auto queryRange = Range<CodeUnit>(acpStart, acpEnd);

        auto const textRange = editable->GetU16TextRange();

        if (queryRange.GetEnd() == -1)
        {
            queryRange.SetEnd(textRange.GetEnd());
        }

        if (queryRange.GetEnd() < queryRange.GetBegin())
        {
            return TS_E_INVALIDPOS;
        }

        if (queryRange.GetEnd() < textRange.GetBegin() || textRange.GetEnd() < queryRange.GetBegin())
        {
            return TS_E_INVALIDPOS;
        }

        auto const text = editable->GetU16Text(queryRange);

        if (cchPlainReq > 0)
        {
            auto const length = std::min<size_t>(cchPlainReq, text.size());
            std::copy(text.begin(), text.end(), pchPlain);
            *pcchPlainRet = static_cast<ULONG>(length);
        }

        if (cRunInfoReq > 0)
        {
            prgRunInfo[0].type = TS_RT_PLAIN;
            prgRunInfo[0].uCount = static_cast<ULONG>(queryRange.GetLength());
            *pcRunInfoRet = 1;
        }
        *pacpNext = acpEnd;
    }
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::SetText(DWORD dwFlags, LONG acpStart, LONG acpEnd, const WCHAR* pchText, ULONG cch, TS_TEXTCHANGE* pChange)
{
    (void)dwFlags;

    if (auto editable = _owner.GetEditable())
    {
        auto text = std::u16string(cch, 0);
        std::memcpy(text.data(), pchText, text.size() * sizeof(char16_t));

        editable->SetU16SelectedRange({acpStart, acpEnd});
        editable->InsertU16Text(text, 1);
        pChange->acpStart = acpStart;
        pChange->acpOldEnd = acpEnd;
        pChange->acpNewEnd = acpStart + static_cast<LONG>(text.size());
    }
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::GetFormattedText(LONG acpStart, LONG acpEnd, IDataObject** ppDataObject)
{
    (void)acpStart;
    (void)acpEnd;
    (void)ppDataObject;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::GetEmbedded(LONG acpPos, REFGUID rguidService, REFIID riid, IUnknown** ppunk)
{
    (void)acpPos;
    (void)rguidService;
    (void)riid;
    (void)ppunk;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::QueryInsertEmbedded(const GUID* pguidService, const FORMATETC* pFormatEtc, BOOL* pfInsertable)
{
    (void)pguidService;
    (void)pFormatEtc;
    (void)pfInsertable;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::InsertEmbedded(DWORD dwFlags, LONG acpStart, LONG acpEnd, IDataObject* pDataObject, TS_TEXTCHANGE* pChange)
{
    (void)dwFlags;
    (void)acpStart;
    (void)acpEnd;
    (void)pDataObject;
    (void)pChange;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::InsertTextAtSelection(DWORD dwFlags, const WCHAR* pchText, ULONG cch, LONG* pacpStart, LONG* pacpEnd, TS_TEXTCHANGE* pChange)
{
    if (auto const editable = _owner.GetEditable())
    {
        auto const selection = editable->GetU16SelectedRange();
        auto const text = std::u16string_view(reinterpret_cast<const char16_t*>(pchText), cch);

        if (dwFlags & TF_IAS_QUERYONLY)
        {
            *pacpStart = static_cast<LONG>(selection.GetBegin());
            *pacpEnd = *pacpStart + static_cast<LONG>(text.size());
        }
        else
        {
            editable->InsertU16Text(text, 1);
            pChange->acpStart = static_cast<LONG>(selection.GetBegin());
            pChange->acpOldEnd = static_cast<LONG>(selection.GetEnd());
            pChange->acpNewEnd = pChange->acpStart + static_cast<LONG>(text.size());
            *pacpStart = pChange->acpNewEnd;
            *pacpEnd = pChange->acpNewEnd;
            return S_OK;
        }
        return S_OK;
    }
    return TS_E_NOSELECTION;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::InsertEmbeddedAtSelection(DWORD dwFlags, IDataObject* pDataObject, LONG* pacpStart, LONG* pacpEnd, TS_TEXTCHANGE* pChange)
{
    (void)dwFlags;
    (void)pDataObject;
    (void)pacpStart;
    (void)pacpEnd;
    (void)pChange;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::RequestSupportedAttrs(DWORD dwFlags, ULONG cFilterAttrs, const TS_ATTRID* paFilterAttrs)
{
    (void)dwFlags;
    (void)cFilterAttrs;
    (void)paFilterAttrs;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::RequestAttrsAtPosition(LONG acpPos, ULONG cFilterAttrs, const TS_ATTRID* paFilterAttrs, DWORD dwFlags)
{
    (void)acpPos;
    (void)cFilterAttrs;
    (void)paFilterAttrs;
    (void)dwFlags;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::RequestAttrsTransitioningAtPosition(LONG acpPos, ULONG cFilterAttrs, const TS_ATTRID* paFilterAttrs, DWORD dwFlags)
{
    (void)acpPos;
    (void)cFilterAttrs;
    (void)paFilterAttrs;
    (void)dwFlags;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::FindNextAttrTransition(
  LONG acpStart,
  LONG acpHalt,
  ULONG cFilterAttrs,
  const TS_ATTRID* paFilterAttrs,
  DWORD dwFlags,
  LONG* pacpNext,
  BOOL* pfFound,
  LONG* plFoundOffset)
{
    (void)acpStart;
    (void)acpHalt;
    (void)cFilterAttrs;
    (void)paFilterAttrs;
    (void)dwFlags;
    (void)pacpNext;
    (void)pfFound;
    (void)plFoundOffset;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::RetrieveRequestedAttrs(ULONG ulCount, TS_ATTRVAL* paAttrVals, ULONG* pcFetched)
{
    (void)ulCount;
    (void)paAttrVals;
    (void)pcFetched;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::GetEndACP(LONG* pacp)
{
    if (auto const editable = _owner.GetEditable())
    {
        auto const range = editable->GetU16TextRange();
        *pacp = static_cast<LONG>(range.GetEnd());
    }
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::GetActiveView(TsViewCookie* pvcView)
{
    *pvcView = 0;
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::GetACPFromPoint(TsViewCookie vcView, const POINT* ptScreen, DWORD dwFlags, LONG* pacp)
{
    (void)vcView;;
    (void)ptScreen;
    (void)dwFlags;
    (void)pacp;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::GetTextExt(TsViewCookie vcView, LONG acpStart, LONG acpEnd, RECT* prc, BOOL* pfClipped)
{
    if (vcView != 0)
    {
        return E_INVALIDARG;
    }

    if (auto editable = _owner.GetEditable())
    {
        auto const& layoutInfo = editable->GetLayoutInfo();
        auto const range = editable->GetRangeFromU16Range(Range<CodeUnit>(acpStart, acpEnd));
        auto const glyphBegin = layoutInfo.GetGlyphIndex(range.GetBegin());
        auto const glyphEnd = layoutInfo.GetGlyphIndex(range.GetEnd());
        auto const posBegin = layoutInfo.GetGlyphPosition(glyphBegin);
        auto const posEnd = layoutInfo.GetGlyphPosition(glyphEnd);

        if (prc)
        {
            auto const hwnd = _owner._hwnd;

            auto windowRect = RECT();
            ::GetWindowRect(hwnd, &windowRect);

            auto layoutRect = editable->GetLayoutRect();

            auto const left = layoutRect.GetLeft() + posBegin.GetX();
            auto const right = layoutRect.GetLeft() + posEnd.GetX();
            layoutRect.SetLeft(left);
            layoutRect.SetRight(right);

            auto const displayScale = static_cast<DisplayScale>(GetDpiForWindow(hwnd)) / USER_DEFAULT_SCREEN_DPI;
            auto const textRect = UnitFunction::ConvertDpToSp(layoutRect, displayScale);

            prc->left = windowRect.left + static_cast<LONG>(textRect.GetLeft());
            prc->top = windowRect.top + static_cast<LONG>(textRect.GetTop());
            prc->right = windowRect.left + static_cast<LONG>(textRect.GetRight());
            prc->bottom = windowRect.top + static_cast<LONG>(textRect.GetBottom());

            // TODO: query caret size.
            if (acpStart == acpEnd || ::IsRectEmpty(prc))
            {
                *pfClipped = TRUE;
            }
        }
    }
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::GetScreenExt(TsViewCookie vcView, RECT* prc)
{
    if (vcView != 0)
    {
        return E_INVALIDARG;
    }

    if (prc)
    {
        auto const monitor = ::MonitorFromWindow(_owner._hwnd, MONITOR_DEFAULTTONEAREST);
        auto monitorInfo = MONITORINFO();
        monitorInfo.cbSize = sizeof(MONITORINFO);
        if (::GetMonitorInfoW(monitor, &monitorInfo))
        {
            *prc = monitorInfo.rcMonitor;
        }
    }
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::OnStartComposition(ITfCompositionView* pComposition, BOOL* pfOk)
{
    // TODO: Send notification
    (void)pComposition;
    *pfOk = TRUE;
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::OnUpdateComposition(ITfCompositionView* pComposition, ITfRange* pRangeNew)
{
    // TODO: Send notification
    (void)pComposition;
    (void)pRangeNew;
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::OnEndComposition(ITfCompositionView* pComposition)
{
    // TODO: Send notification
    (void)pComposition;
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::AdviseMouseSink(ITfRangeACP* range, ITfMouseSink* pSink, DWORD* pdwCookie)
{
    // TODO: Implement mouse control.
    (void)range;
    (void)pSink;
    (void)pdwCookie;
    return E_NOTIMPL;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::UnadviseMouseSink(DWORD dwCookie)
{
    // TODO: Implement mouse control.
    (void)dwCookie;
    return E_NOTIMPL;
}

auto PlatformInputMethodTextStoreWin::TextStoreImpl::NotifySelectionChange() -> void
{
    if (_sink)
    {
        _sink->OnSelectionChange();
    }
}

auto PlatformInputMethodTextStoreWin::TextStoreImpl::NotifyTextChange(CodeUnit begin, CodeUnit oldEnd, CodeUnit newEnd) -> void
{
    if (_sink)
    {
        auto const textChange = TS_TEXTCHANGE {
            .acpStart = static_cast<LONG>(begin),
            .acpOldEnd = static_cast<LONG>(oldEnd),
            .acpNewEnd = static_cast<LONG>(newEnd),
        };
        _sink->OnTextChange(0, &textChange);
    }
}

auto PlatformInputMethodTextStoreWin::TextStoreImpl::NotifyLayoutChange() -> void
{
    if (_sink)
    {
        _sink->OnLayoutChange(TS_LC_CHANGE, 0);
    }
}

///
/// @brief Make instance.
///
auto PlatformInputMethodTextStoreWin::Make(Shared<PlatformInputMethodContextWin> context, HWND hwnd) -> Shared<PlatformInputMethodTextStoreWin>
{
    auto self = Shared<PlatformInputMethodTextStoreWin>::Make(PassKey<PlatformInputMethodTextStoreWin>(), context, hwnd);
    self->_self = self;
    return self;
}

///
/// @brief Constructor.
///
PlatformInputMethodTextStoreWin::PlatformInputMethodTextStoreWin(PassKey<PlatformInputMethodTextStoreWin>, Shared<PlatformInputMethodContextWin> context, HWND hwnd)
  : _hwnd {hwnd}
  , _platformContext {context}
{
    if (!_platformContext)
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    _documentMgr = context->CreateDocumentMgr();
    if (!_documentMgr)
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    _textStore = Microsoft::WRL::Make<TextStoreImpl>(*this);
    if (!_textStore)
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    auto const clientId = _platformContext->GetClientId();
    auto hr = _documentMgr->CreateContext(clientId, 0, _textStore.Get(), _context.GetAddressOf(), &_editCookie);
    if (FAILED(hr) || !_context)
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    hr = _documentMgr->Push(_context.Get());
    if (FAILED(hr))
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    if (_platformContext)
    {
        // We use AssociateFocus() instead of manually calling SetFocus() on WM_SETFOCUS.
        // FIXME: Figure out why initial focus set in WM_SETFOCUS is immediately replaced by TSF right before exiting DefWindowProc for WM_ACTIVATE.
        // This does not happen in TsfPad, so we might be doing something wrong here?
        _platformContext->AssociateFocus(hwnd, _documentMgr.Get());
    }
}

///
/// @brief Destructor.
///
PlatformInputMethodTextStoreWin::~PlatformInputMethodTextStoreWin()
{
    if (_documentMgr)
    {
        _documentMgr->Pop(TF_POPF_ALL);
    }
}

///
/// @brief Insert text from keyboard event.
///
auto PlatformInputMethodTextStoreWin::InsertTextFromKeyEvent(String const& text) -> void
{
    if (auto const editable = GetEditable())
    {
        auto const selection = editable->GetU16SelectedRange();
        editable->InsertText(text, 1);
        NotifyTextChange(selection.GetBegin(), selection.GetEnd(), editable->GetU16SelectedRange().GetEnd());
        NotifySelectionChange();
    }
}

///
/// @brief Get editable object.
///
auto PlatformInputMethodTextStoreWin::GetEditable() -> Shared<PlatformInputMethodEditableWin>
{
    return _editable.Lock();
}

///
/// @brief Set editable object.
///
auto PlatformInputMethodTextStoreWin::SetEditable(Weak<PlatformInputMethodEditableWin> const& editable) -> void
{
    auto const oldEditable = GetEditable();
    auto const newEditable = editable.Lock();
    if (oldEditable != newEditable)
    {
        FW_DEBUG_LOG_INFO("PlatformInputMethodTextStoreWin::SetEditable");

        _editable = newEditable;

        auto oldTextRange = Range<CodeUnit>();
        if (oldEditable)
        {
            oldTextRange = oldEditable->GetU16TextRange();
            oldEditable->SetTextStore(nullptr);
        }

        auto newTextRange = Range<CodeUnit>();
        if (newEditable)
        {
            newEditable->SetTextStore(GetSelf());
            newTextRange = newEditable->GetU16TextRange();
        }
        NotifyTextChange(oldTextRange.GetBegin(), oldTextRange.GetEnd(), newTextRange.GetEnd());
        NotifySelectionChange();
    }
}

///
/// @brief Notify selection change.
///
auto PlatformInputMethodTextStoreWin::NotifySelectionChange() -> void
{
    if (auto textStore = static_cast<TextStoreImpl*>(_textStore.Get()))
    {
        textStore->NotifySelectionChange();
    }
}

///
/// @brief Notify text change.
///
auto PlatformInputMethodTextStoreWin::NotifyTextChange(CodeUnit begin, CodeUnit oldEnd, CodeUnit newEnd) -> void
{
    if (auto textStore = static_cast<TextStoreImpl*>(_textStore.Get()))
    {
        textStore->NotifyTextChange(begin, oldEnd, newEnd);
    }
}

///
/// @brief Notify text layout change.
///
auto PlatformInputMethodTextStoreWin::NotifyLayoutChange() -> void
{
    if (auto textStore = static_cast<TextStoreImpl*>(_textStore.Get()))
    {
        textStore->NotifyLayoutChange();
    }
}

///
/// @brief Get self.
///
auto PlatformInputMethodTextStoreWin::GetSelf() -> Shared<PlatformInputMethodTextStoreWin>
{
    return _self.Lock();
}
}
