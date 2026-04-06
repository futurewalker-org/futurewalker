// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputEditableWin.hpp"
#include "Futurewalker.Application.PlatformInputEditable.hpp"
#include "Futurewalker.Application.PlatformKeyEvent.hpp"
#include "Futurewalker.Application.Key.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.MonotonicClock.hpp"

#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

#include <wrl/module.h>
#include <olectl.h>

namespace FW_DETAIL_NS
{
class PlatformInputMethodTextStoreWin::TextStoreImpl : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, ITextStoreACP2, ITfContextOwnerCompositionSink, ITfMouseTrackerACP, ITfKeyTraceEventSink>
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

    /* ITfKeyTraceEventSink */
    STDMETHODIMP OnKeyTraceDown(WPARAM wParam, LPARAM lParam) override;
    STDMETHODIMP OnKeyTraceUp(WPARAM wParam, LPARAM lParam) override;

    auto NotifySelectionChange() -> void;
    auto NotifyTextChange(CodeUnit begin, CodeUnit oldEnd, CodeUnit newEnd) -> void;
    auto NotifyLayoutChange() -> void;

    auto CancelProcessKeyDown() -> void;

private:
    PlatformInputMethodTextStoreWin& _owner;

private:
    Microsoft::WRL::ComPtr<ITextStoreACPSink> _sink;
    DWORD _sinkMask = 0;
    WPARAM _keyTraceDownWParam = 0;
    LPARAM _keyTraceDownLParam = 0;
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
            auto const selectionDirection = editable->GetSelectionDirection();
            auto const selectionRange = editable->GetU16SelectionRange();
            pSelection[0].acpStart = static_cast<LONG>(selectionRange.GetBegin());
            pSelection[0].acpEnd = static_cast<LONG>(selectionRange.GetEnd());
            pSelection[0].style.ase = (selectionDirection == TextSelectionDirection::Backward) ? TS_AE_START : TS_AE_END;
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
    FW_DEBUG_LOG_INFO("PlatformInputMethodTextStoreWin::TextStoreImpl::SetSelection()");
    if (pSelection && ulCount > 0)
    {
        if (auto editable = _owner.GetEditable())
        {
            auto const& selection = pSelection[0];
            auto const direction = (selection.style.ase == TS_AE_START) ? TextSelectionDirection::Backward : TextSelectionDirection::Forward;
            editable->SetU16SelectionRange({selection.acpStart, selection.acpEnd}, direction, true);
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

        auto const textRange = editable->GetU16StringRange();

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

        auto const text = editable->GetU16String(queryRange);

        if (cchPlainReq > 0)
        {
            auto const length = std::min<size_t>(cchPlainReq, static_cast<size_t>(text.GetView().GetSize()));
            std::copy(text.GetView().begin(), text.GetView().end(), pchPlain);
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
    FW_DEBUG_LOG_INFO("PlatformInputMethodTextStoreWin::TextStoreImpl::SetText: acpStart={}, acpEnd={}, cch={}", acpStart, acpEnd, cch);

    if (auto editable = _owner.GetEditable())
    {
        auto text = std::u16string(cch, 0);
        std::memcpy(text.data(), pchText, text.size() * sizeof(char16_t));

        editable->SetU16SelectionRange({acpStart, acpEnd}, TextSelectionDirection::Forward, true);
        editable->InsertU16String({text.begin(), text.end()}, 1, true, false);
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
    FW_DEBUG_LOG_INFO("PlatformInputMethodTextStoreWin::TextStoreImpl::InsertTextAtSelection: cch={}", cch);

    if (auto const editable = _owner.GetEditable())
    {
        auto const selection = editable->GetU16SelectionRange();
        auto const text = std::u16string_view(reinterpret_cast<const char16_t*>(pchText), cch);

        if (dwFlags & TF_IAS_QUERYONLY)
        {
            *pacpStart = static_cast<LONG>(selection.GetBegin());
            *pacpEnd = *pacpStart + static_cast<LONG>(text.size());
        }
        else
        {
            editable->InsertU16String({text.begin(), text.end()}, 1, true, false);
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
        auto const range = editable->GetU16StringRange();
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
    (void)vcView;
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
        auto const& text = editable->GetText();
        auto const& layoutInfo = editable->GetLayoutInfo();

        auto const begin = text.GetCodePointIndexByU16Index(acpStart);
        auto const end = text.GetCodePointIndexByU16Index(acpEnd);
        auto const glyphBegin = layoutInfo.GetGlyphIndex(begin);
        auto const glyphEnd = layoutInfo.GetGlyphIndex(end);
        auto const posBegin = layoutInfo.GetGlyphPosition(glyphBegin).GetValueOrDefault();
        auto const posEnd = layoutInfo.GetGlyphPosition(glyphEnd).GetValueOrDefault();

        if (prc)
        {
            auto const hwnd = _owner._hwnd;

            auto windowRect = RECT();
            ::GetWindowRect(hwnd, &windowRect);

            auto layoutRect = editable->GetLayoutRect();

            auto const left = layoutRect.GetLeft() + posBegin.x;
            auto const right = layoutRect.GetLeft() + posEnd.x;
            layoutRect.SetLeft(left);
            layoutRect.SetRight(right);

            auto const displayScale = static_cast<DisplayScale>(GetDpiForWindow(hwnd)) / USER_DEFAULT_SCREEN_DPI;
            auto const textRect = UnitFunction::ConvertDpToVp(layoutRect, displayScale);

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
    FW_DEBUG_LOG_INFO("PlatformInputMethodTextStoreWin::TextStoreImpl::OnStartComposition()");
    (void)pComposition;
    if (auto const editable = _owner.GetEditable())
    {
        editable->CompositionStart();
    }
    *pfOk = TRUE;
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::OnUpdateComposition(ITfCompositionView* pComposition, ITfRange* pRangeNew)
{
    FW_DEBUG_LOG_INFO("PlatformInputMethodTextStoreWin::TextStoreImpl::OnUpdateComposition()");
    (void)pComposition;
    (void)pRangeNew;

    if (_keyTraceDownWParam != 0 && _keyTraceDownLParam != 0)
    {
        auto const wParam = std::exchange(_keyTraceDownWParam, 0);
        auto const lParam = std::exchange(_keyTraceDownLParam, 0);

        auto const virtualKeyCode = static_cast<DWORD>(wParam);
        auto const keyFlags = HIWORD(lParam);
        auto const isRepeat = (keyFlags & KF_REPEAT) != 0;
        auto const eventCount = LOWORD(lParam);

        // TODO: Get unmodified key.
        (void)virtualKeyCode;

        auto const timestamp = MonotonicClock::GetNow();
        auto const key = Key::Process;
        auto const unmodifiedKey = Key::Unidentified;
        auto const text = String(); 
        auto const modifiers = ModifierKeyFlags::None; // TODO: Get actual modifier state.

        for (auto i = 0; i < eventCount; ++i)
        {
            try
            {
                auto parameter = Event<PlatformKeyEvent::Down>::Make();
                parameter->SetKey(key);
                parameter->SetText(text);
                parameter->SetUnmodifiedKey(unmodifiedKey);
                parameter->SetModifiers(modifiers);
                parameter->SetRepeat(isRepeat);
                parameter->SetTimestamp(timestamp);

                auto event = Event<>(std::move(parameter));

                if (_owner._delegate.sendKeyEventDetached)
                {
                    _owner._delegate.sendKeyEventDetached(event);
                }
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
        }
    }

    if (auto const editable = _owner.GetEditable())
    {
        editable->CompositionUpdate();
    }
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::OnEndComposition(ITfCompositionView* pComposition)
{
    FW_DEBUG_LOG_INFO("PlatformInputMethodTextStoreWin::TextStoreImpl::OnEndComposition()");
    (void)pComposition;
    if (auto const editable = _owner.GetEditable())
    {
        editable->CompositionEnd();
    }
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

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::OnKeyTraceDown(WPARAM wParam, LPARAM lParam)
{
    //FW_DEBUG_LOG_INFO("PlatformInputMethodTextStoreWin::TextStoreImpl::OnKeyTraceDown() wParam: {}, lParam: {}", wParam, lParam);
    _keyTraceDownWParam = wParam;
    _keyTraceDownLParam = lParam;
    return S_OK;
}

STDMETHODIMP PlatformInputMethodTextStoreWin::TextStoreImpl::OnKeyTraceUp(WPARAM wParam, LPARAM lParam)
{
    //FW_DEBUG_LOG_INFO("PlatformInputMethodTextStoreWin::TextStoreImpl::OnKeyTraceUp() wParam: {}, lParam: {}", wParam, lParam);
    (void) wParam;
    (void) lParam;
    CancelProcessKeyDown();
    return S_OK;
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

auto PlatformInputMethodTextStoreWin::TextStoreImpl::CancelProcessKeyDown() -> void
{
    _keyTraceDownWParam = 0;
    _keyTraceDownLParam = 0;
}

///
/// @brief Make instance.
///
auto PlatformInputMethodTextStoreWin::Make(Delegate const& delegate, Shared<PlatformInputMethodContextWin> context, HWND hwnd) -> Shared<PlatformInputMethodTextStoreWin>
{
    auto self = Shared<PlatformInputMethodTextStoreWin>::Make(PassKey<PlatformInputMethodTextStoreWin>(), delegate, context, hwnd);
    self->_self = self;
    return self;
}

///
/// @brief Constructor.
///
PlatformInputMethodTextStoreWin::PlatformInputMethodTextStoreWin(PassKey<PlatformInputMethodTextStoreWin>, Delegate const& delegate, Shared<PlatformInputMethodContextWin> context, HWND hwnd)
  : _delegate {delegate}
  , _hwnd {hwnd}
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

    Microsoft::WRL::ComPtr<ITfSource> source;
    hr = context->GetThreadMgr().As(&source);
    if (FAILED(hr))
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    DWORD unadviseCookie = 0;
    hr = source->AdviseSink(IID_ITfKeyTraceEventSink, _textStore.Get(), &unadviseCookie);
    if (FAILED(hr))
    {
        FW_DEBUG_ASSERT(false);
        return;
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
        editable->InsertText(text, 1);
    }
}

///
/// @brief Emit input event from key event.
///
auto PlatformInputMethodTextStoreWin::InputKeyFromKeyEvent(String const& key) -> void
{
    if (auto const editable = GetEditable())
    {
        auto const selection = editable->GetU16SelectionRange();
        if (key == Key::Backspace || key == Key::Delete)
        {
            if (selection.GetLength() == 0)
            {
                if (key == Key::Backspace)
                {
                    editable->DeleteSurroundingText(1, 0);
                }
                else
                {
                    editable->DeleteSurroundingText(0, 1);
                }
            }
            else
            {
                editable->InsertText({}, 0);
            }
        }
        else if (key == Key::Enter)
        {
            editable->InsertLineBreak();
        }
    }
}

///
/// @brief Get editable object.
///
auto PlatformInputMethodTextStoreWin::GetEditable() -> Shared<PlatformInputEditableWin>
{
    return _editable.Lock();
}

///
/// @brief Set editable object.
///
auto PlatformInputMethodTextStoreWin::SetEditable(Weak<PlatformInputEditableWin> const& editable) -> void
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
            oldTextRange = oldEditable->GetU16StringRange();
            oldEditable->SetTextStore(nullptr);
        }

        auto newTextRange = Range<CodeUnit>();
        if (newEditable)
        {
            newEditable->SetTextStore(GetSelf());
            newTextRange = newEditable->GetU16StringRange();
        }
        NotifyTextChange(oldTextRange.GetBegin(), oldTextRange.GetEnd(), newTextRange.GetEnd());
        NotifySelectionChange();
        UpdateDocumentMgrFocus();
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
/// @brief Cancel pending process key down event.
///
auto PlatformInputMethodTextStoreWin::CancelProcessKeyDown() -> void
{
    if (auto textStore = static_cast<TextStoreImpl*>(_textStore.Get()))
    {
        textStore->CancelProcessKeyDown();
    }
}

///
/// @brief Get self.
///
auto PlatformInputMethodTextStoreWin::GetSelf() -> Shared<PlatformInputMethodTextStoreWin>
{
    return _self.Lock();
}

///
/// @brief Update focus association of document manager.
///
auto PlatformInputMethodTextStoreWin::UpdateDocumentMgrFocus() -> void
{
    // We use AssociateFocus() instead of manually calling SetFocus() on WM_SETFOCUS.
    // FIXME: Figure out why initial focus set in WM_SETFOCUS is immediately replaced by TSF right before exiting DefWindowProc for WM_ACTIVATE.
    // This does not happen in TsfPad, so we might be doing something wrong here?
    if (!_editable.IsExpired())
    {
        _platformContext->AssociateFocus(_hwnd, _documentMgr.Get());
    }
    else
    {
        _platformContext->AssociateFocus(_hwnd, nullptr);
    }
}
}
