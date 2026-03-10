// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformPointerEventFunctionWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto PlatformPointerEventFunctionWin::ConvertMouseEventParamToPointerButtonFlags(WPARAM const wParam) -> PointerButtonFlags
{
    auto buttons = PointerButtonFlags::None;
    auto const low = LOWORD(wParam);
    if (low & MK_LBUTTON)
    {
        buttons |= PointerButtonFlags::Button1;
    }
    if (low & MK_RBUTTON)
    {
        buttons |= PointerButtonFlags::Button2;
    }
    if (low & MK_MBUTTON)
    {
        buttons |= PointerButtonFlags::Button3;
    }
    if (low & MK_XBUTTON1)
    {
        buttons |= PointerButtonFlags::ButtonX1;
    }
    if (low & MK_XBUTTON2)
    {
        buttons |= PointerButtonFlags::ButtonX2;
    }
    return buttons;
}

auto PlatformPointerEventFunctionWin::ConvertMouseEventParamToModifierKeyFlags(WPARAM const wParam) -> ModifierKeyFlags
{
    auto modifiers = ModifierKeyFlags::None;
    auto const low = LOWORD(wParam);
    if (low & MK_CONTROL)
    {
        modifiers |= ModifierKeyFlags::Control;
    }
    if (low & MK_SHIFT)
    {
        modifiers |= ModifierKeyFlags::Shift;
    }
    return modifiers;
}

auto PlatformPointerEventFunctionWin::ConvertPointerButtonChangeTypeToPointerButton(UINT32 const buttonChangeType) -> PointerButton
{
    auto button = PointerButton::Unknown;
    if (buttonChangeType == POINTER_CHANGE_FIRSTBUTTON_DOWN || buttonChangeType == POINTER_CHANGE_FIRSTBUTTON_UP)
    {
        button = PointerButton::Button1;
    }
    else if (buttonChangeType == POINTER_CHANGE_SECONDBUTTON_DOWN || buttonChangeType == POINTER_CHANGE_SECONDBUTTON_UP)
    {
        button = PointerButton::Button2;
    }
    else if (buttonChangeType == POINTER_CHANGE_THIRDBUTTON_DOWN || buttonChangeType == POINTER_CHANGE_THIRDBUTTON_UP)
    {
        button = PointerButton::Button3;
    }
    else if (buttonChangeType == POINTER_CHANGE_FOURTHBUTTON_DOWN || buttonChangeType == POINTER_CHANGE_FOURTHBUTTON_UP)
    {
        button = PointerButton::ButtonX1;
    }
    else if (buttonChangeType == POINTER_CHANGE_FIFTHBUTTON_DOWN || buttonChangeType == POINTER_CHANGE_FIFTHBUTTON_UP)
    {
        button = PointerButton::ButtonX2;
    }
    return button;
}

auto PlatformPointerEventFunctionWin::ConvertPointerFlagsToPointerButtonFlags(UINT32 const pointerFlags) -> PointerButtonFlags
{
    auto buttons = PointerButtonFlags::None;
    if (pointerFlags & POINTER_FLAG_FIRSTBUTTON)
    {
        buttons |= PointerButtonFlags::Button1;
    }
    if (pointerFlags & POINTER_FLAG_SECONDBUTTON)
    {
        buttons |= PointerButtonFlags::Button2;
    }
    if (pointerFlags & POINTER_FLAG_THIRDBUTTON)
    {
        buttons |= PointerButtonFlags::Button3;
    }
    if (pointerFlags & POINTER_FLAG_FOURTHBUTTON)
    {
        buttons |= PointerButtonFlags::ButtonX1;
    }
    if (pointerFlags & POINTER_FLAG_FIFTHBUTTON)
    {
        buttons |= PointerButtonFlags::ButtonX2;
    }
    return buttons;
}

auto PlatformPointerEventFunctionWin::SetPointerEventParamsForMouse(PlatformPointerEvent& parameter, HWND const hWnd, UINT const msg, WPARAM const wParam, LPARAM const lParam) -> void
{
    parameter.SetPrimaryPointer(true);
    parameter.SetPointerType(PointerType::Mouse);
    parameter.SetPointerId(1U); // TODO: Unique ID for mouse pointer.
    auto frequency = LARGE_INTEGER();
    ::QueryPerformanceFrequency(&frequency);
    auto tickCount = LARGE_INTEGER();
    ::QueryPerformanceCounter(&tickCount);
    auto const seconds = tickCount.QuadPart / frequency.QuadPart;
    auto const remainder = tickCount.QuadPart % frequency.QuadPart;
    auto const nanoseconds = SInt64(seconds * 1'000'000'000 + (remainder * 1'000'000'000) / frequency.QuadPart);
    parameter.SetTimestamp(MonotonicTime::MakeFromNanoseconds(nanoseconds));

    if (WM_MOUSEFIRST <= msg && msg <= WM_MOUSELAST)
    {
        auto clientX = GET_X_LPARAM(lParam);
        auto clientY = GET_Y_LPARAM(lParam);
        if (msg == WM_MOUSEWHEEL || msg == WM_MOUSEHWHEEL)
        {
            auto clientPoint = POINT {clientX, clientY};
            if (::ScreenToClient(hWnd, &clientPoint))
            {
                clientX = clientPoint.x;
                clientY = clientPoint.y;
            }
        }
        auto clientRect = RECT();
        ::GetClientRect(hWnd, &clientRect);
        auto const displayScale = ::GetDpiForWindow(hWnd) / DisplayScale(USER_DEFAULT_SCREEN_DPI);
        auto const x = UnitFunction::ConvertVpToDp(clientX + clientRect.left, displayScale);
        auto const y = UnitFunction::ConvertVpToDp(clientY + clientRect.top, displayScale);
        parameter.SetPosition({x, y});
        auto const modifiers = ConvertMouseEventParamToModifierKeyFlags(wParam);
        auto const buttons = ConvertMouseEventParamToPointerButtonFlags(wParam);
        parameter.SetModifiers(modifiers);
        parameter.SetButtons(buttons);
    }
}

auto PlatformPointerEventFunctionWin::SetPointerMotionEventParamsForMouse(PlatformPointerEvent::Motion& parameter, UINT const msg, WPARAM const wParam, LPARAM const lParam) -> void
{
    (void)lParam;

    auto setParams = [&](auto& parameter) {
        if (LOWORD(wParam) & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON | MK_XBUTTON1 | MK_XBUTTON2))
        {
            parameter.SetPressure(0.5);
        }

        if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP)
        {
            parameter.SetButton(PointerButton::Button1);
        }
        else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP)
        {
            parameter.SetButton(PointerButton::Button2);
        }
        else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP)
        {
            parameter.SetButton(PointerButton::Button3);
        }
        else if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONUP)
        {
            const auto xButton = HIWORD(wParam);
            if (xButton == XBUTTON1)
            {
                parameter.SetButton(PointerButton::ButtonX1);
            }
            else if (xButton == XBUTTON2)
            {
                parameter.SetButton(PointerButton::ButtonX2);
            }
        }
    };

    if (parameter.Is<PlatformPointerEvent::Motion::Down>())
    {
        auto downParameter = parameter.As<PlatformPointerEvent::Motion::Down>();
        setParams(downParameter);
        parameter = downParameter;
    }
    else if (parameter.Is<PlatformPointerEvent::Motion::Up>())
    {
        auto upParameter = parameter.As<PlatformPointerEvent::Motion::Up>();
        setParams(upParameter);
        parameter = upParameter;
    }
    else if (parameter.Is<PlatformPointerEvent::Motion::Move>())
    {
        auto moveParameter = parameter.As<PlatformPointerEvent::Motion::Move>();
        setParams(moveParameter);
        parameter = moveParameter;
    }
}

auto PlatformPointerEventFunctionWin::SetPointerScrollEventParamsForMouse(PlatformPointerEvent::Action::Scroll& parameter, UINT const msg, WPARAM const wParam, LPARAM const lParam) -> void
{
    (void)lParam;

    auto const delta = std::bit_cast<SHORT>(HIWORD(wParam));
    if (msg == WM_MOUSEWHEEL)
    {
        parameter.SetDeltaX(0);
        parameter.SetDeltaY(delta / Dp(WHEEL_DELTA));
        parameter.SetPrecision(PointerScrollPrecision::Coarse);
    }
    else if (msg == WM_MOUSEHWHEEL)
    {
        parameter.SetDeltaX(delta / Dp(WHEEL_DELTA));
        parameter.SetDeltaY(0);
        parameter.SetPrecision(PointerScrollPrecision::Coarse);
    }
}

auto PlatformPointerEventFunctionWin::SetPointerEventParamsForPointer(PlatformPointerEvent& parameter, UINT32 const pointerId, HWND const hwnd) -> void
{
    auto info = POINTER_INFO();
    if (!::GetPointerInfo(pointerId, &info))
    {
        FW_DEBUG_LOG_ERROR("PlatformWindowWin: GetPointerInfo failed");
        return;
    }

    parameter.SetPointerId(PointerId(info.pointerId));
    parameter.SetPrimaryPointer((info.pointerFlags & POINTER_FLAG_PRIMARY) != POINTER_FLAG_NONE);

    if (info.pointerType == PT_MOUSE)
    {
        parameter.SetPointerType(PointerType::Mouse);
    }
    else if (info.pointerType == PT_PEN)
    {
        parameter.SetPointerType(PointerType::Pen);
    }
    else if (info.pointerType == PT_TOUCH)
    {
        parameter.SetPointerType(PointerType::Touch);
    }
    else if (info.pointerType == PT_TOUCHPAD)
    {
        parameter.SetPointerType(PointerType::TouchPad);
    }

    {
        auto frequency = LARGE_INTEGER();
        ::QueryPerformanceFrequency(&frequency);

        auto timestamp = info.PerformanceCount;
        if (timestamp == 0)
        {
            auto tickCount = LARGE_INTEGER();
            ::QueryPerformanceCounter(&tickCount);
            timestamp = tickCount.QuadPart;
        }
        auto const seconds = timestamp / frequency.QuadPart;
        auto const remainder = timestamp % frequency.QuadPart;
        auto const nanoseconds = SInt64(seconds * 1'000'000'000 + (remainder * 1'000'000'000) / frequency.QuadPart);
        parameter.SetTimestamp(MonotonicTime::MakeFromNanoseconds(nanoseconds));
    }
    {
        auto rect = RECT();
        if (::GetWindowRect(hwnd, &rect))
        {
            const auto displayScale = ::GetDpiForWindow(hwnd) / DisplayScale(USER_DEFAULT_SCREEN_DPI);
            const auto pos = info.ptPixelLocation;
            parameter.SetPosition(UnitFunction::ConvertVpToDp(Point<Vp>(pos.x - rect.left, pos.y - rect.top), displayScale));
        }
    }
}
auto PlatformPointerEventFunctionWin::SetPointerMotionEventParamsForPointer(PlatformPointerEvent::Motion& parameter, UINT32 const pointerId) -> void
{
    auto setParams = [&](auto& parameter) {
        auto inputType = POINTER_INPUT_TYPE();
        if (!::GetPointerType(pointerId, &inputType))
        {
            FW_DEBUG_LOG_ERROR("PlatformWindowWin: GetPointerType failed");
            return;
        }

        {
            auto info = POINTER_INFO();
            if (!::GetPointerInfo(pointerId, &info))
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: GetPointerInfo failed");
                return;
            }

            if (info.pointerFlags & POINTER_FLAG_INCONTACT)
            {
                parameter.SetPressure(0.5);
            }

            auto const button = ConvertPointerButtonChangeTypeToPointerButton(info.ButtonChangeType);
            auto const buttons = ConvertPointerFlagsToPointerButtonFlags(info.pointerFlags);
            parameter.SetButton(button);
            parameter.SetButtons(buttons);
        }

        if (inputType == PT_PEN)
        {
            auto info = POINTER_PEN_INFO();
            if (::GetPointerPenInfo(pointerId, &info))
            {
                if (info.penMask & PEN_MASK_PRESSURE)
                {
                    // 0 to 1024.
                    parameter.SetPressure(info.pressure / 1024.0);
                }

                if (info.penMask & PEN_MASK_ROTATION)
                {
                    parameter.SetTwist(info.rotation);
                }

                if ((info.penMask & PEN_MASK_TILT_X) && (info.penMask & PEN_MASK_TILT_Y))
                {
                    const auto [azimuth, altitude] = PlatformPointerEvent::ConvertTiltToSpherical(info.tiltX, info.tiltY);
                    parameter.SetTiltX(info.tiltX);
                    parameter.SetTiltY(info.tiltY);
                    parameter.SetAzimuth(azimuth);
                    parameter.SetAltitude(altitude);
                }
            }
            else
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: GetPointerPenInfo failed");
            }
        }
        else if (inputType == PT_TOUCH)
        {
            auto info = POINTER_TOUCH_INFO();
            if (::GetPointerTouchInfo(pointerId, &info))
            {
                if (info.touchMask & TOUCH_MASK_PRESSURE)
                {
                    // 0 to 1024 where 512 represents normal touch pressure.
                    parameter.SetPressure(info.pressure / 1024.0);
                }

                if (info.touchMask & TOUCH_MASK_ORIENTATION)
                {
                    parameter.SetAzimuth(info.orientation);
                }
            }
            else
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: GetPointerTouchInfo failed");
            }
        }
    };

    if (parameter.Is<PlatformPointerEvent::Motion::Down>())
    {
        setParams(parameter.As<PlatformPointerEvent::Motion::Down>());
    }
    else if (parameter.Is<PlatformPointerEvent::Motion::Up>())
    {
        setParams(parameter.As<PlatformPointerEvent::Motion::Up>());
    }
    else if (parameter.Is<PlatformPointerEvent::Motion::Move>())
    {
        setParams(parameter.As<PlatformPointerEvent::Motion::Move>());
    }
}
}
