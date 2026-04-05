// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformWindowMac.hpp"
#include "Futurewalker.Application.Mac.PlatformRootViewLayerMac.hpp"
#include "Futurewalker.Application.Mac.PlatformInputMethodMac.hpp"
#include "Futurewalker.Application.Mac.PlatformInputEditableMac.hpp"
#include "Futurewalker.Application.Mac.PlatformKeyboardLayoutMac.hpp"
#include "Futurewalker.Application.PlatformFrameEvent.hpp"
#include "Futurewalker.Application.PlatformWindowEvent.hpp"
#include "Futurewalker.Application.PlatformPointerEvent.hpp"
#include "Futurewalker.Application.PlatformKeyEvent.hpp"
#include "Futurewalker.Application.Key.hpp"

#include "Futurewalker.Graphics.GlyphRun.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Mac.PlatformStringFunctionMac.hpp"

#include <bit>
#include <Carbon/Carbon.h>

using namespace FW_NS;

static auto NSTimeIntervalToMonotonicTime(NSTimeInterval const interval) -> MonotonicTime
{
    return MonotonicTime::MakeFromNanoseconds(static_cast<int64_t>(interval * 1'000'000'000));
}

static auto SetPointerMotionEventParameter(NSEvent* event, auto& motionEvent, auto const button) -> void
{
    @autoreleasepool
    {
        auto const pressure = event.pressure;
        auto tangentialPressure = double(0);
        auto tiltX = Degree(0.0);
        auto tiltY = Degree(0.0);
        auto twist = Degree(0.0);
        auto altitude = Degree(90.0);
        auto azimuth = Degree(0.0);

        @try
        {
            auto const subtype = event.subtype;
            if (subtype == NSEventSubtypeTabletProximity || subtype == NSEventSubtypeTabletPoint)
            {
                tangentialPressure = event.tangentialPressure;
                tiltX = event.tilt.x * 90.0;
                tiltY = event.tilt.y * 90.0;
                twist = event.rotation;
                std::tie(altitude, azimuth) = PlatformPointerEvent::ConvertTiltToSpherical(tiltX, tiltY);
            }
        }
        @catch (NSException* e)
        {
            NSLog(@"%@", e);
            FW_DEBUG_ASSERT(false);
        }

        motionEvent->SetTiltX(tiltX);
        motionEvent->SetTiltY(tiltY);
        motionEvent->SetAzimuth(azimuth);
        motionEvent->SetAltitude(altitude);
        motionEvent->SetTwist(twist);
        motionEvent->SetPressure(pressure);
        motionEvent->SetTangentialPressure(tangentialPressure);
        motionEvent->SetButton(button);
    }
}

@interface PlatformWindowContentView : NSView<NSTextInputClient>
@property(assign, nonatomic) void* data;
@property(assign, nonatomic) void (*callbackOnPointer)(void* data, Event<>& event);
@property(assign, nonatomic) void (*callbackOnKey)(void* data, Event<>& event);
@property(strong, nonatomic) NSMutableDictionary<id<NSCopying, NSObject>, NSNumber*>* identifiers;
@property(strong, nonatomic) NSTrackingArea* trackingArea;
@property(assign, nonatomic) Shared<PlatformInputMethodMac> inputMethod;
- (instancetype)initWithFrame:(NSRect)frameRect;
- (BOOL)acceptsFirstResponder;
- (void)updateTrackingAreas;
- (void)cursorUpdate:(NSEvent *)event;
- (void)mouseDown:(NSEvent*)event;
- (void)mouseDragged:(NSEvent*)event;
- (void)mouseMoved:(NSEvent*)event;
- (void)mouseUp:(NSEvent*)event;
- (void)mouseEntered:(NSEvent*)event;
- (void)mouseExited:(NSEvent*)event;
- (void)rightMouseDown:(NSEvent *)event;
- (void)rightMouseDragged:(NSEvent *)event;
- (void)rightMouseUp:(NSEvent *)event;
- (void)otherMouseDown:(NSEvent *)event;
- (void)otherMouseDragged:(NSEvent *)event;
- (void)otherMouseUp:(NSEvent *)event;
- (void)scrollWheel:(NSEvent*)event;
- (void)tabletProximity:(NSEvent *)event;
- (void)tabletPoint:(NSEvent *)event;
- (void)touchesBeganWithEvent:(NSEvent*)event;
- (void)touchesMovedWithEvent:(NSEvent*)event;
- (void)touchesEndedWithEvent:(NSEvent*)event;
- (void)touchesCancelledWithEvent:(NSEvent*)event;
- (void)magnifyWithEvent:(NSEvent *)event;
- (void)rotateWithEvent:(NSEvent *)event;
- (void)swipeWithEvent:(NSEvent *)event;
- (void)keyDown:(NSEvent*)event;
- (void)keyUp:(NSEvent*)event;
- (void)flagsChanged:(NSEvent*)event;

- (void)insertText:(id)string replacementRange:(NSRange)replacementRange;
- (void)doCommandBySelector:(SEL)selector;
- (void)setMarkedText:(id)string selectedRange:(NSRange)selectedRange replacementRange:(NSRange)replacementRange;
- (void)unmarkText;
- (NSRange)selectedRange;
- (NSRange)markedRange;
- (BOOL)hasMarkedText;
- (nullable NSAttributedString *)attributedSubstringForProposedRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange;
- (NSArray<NSAttributedStringKey> *)validAttributesForMarkedText;
- (NSRect)firstRectForCharacterRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange;
- (NSUInteger)characterIndexForPoint:(NSPoint)point;

@end

@implementation PlatformWindowContentView

- (instancetype)initWithFrame:(NSRect)frameRect
{
    if (self = [super initWithFrame:frameRect])
    {
        // Direct touches means that the touch is directly on the screen, while indirect means that the touch is on a trackpad or similar device.
        [self setAllowedTouchTypes:NSTouchTypeMaskDirect];
        [self setIdentifiers:[NSMutableDictionary new]];
        [self updateTrackingAreas];

        self.inputMethod = PlatformInputMethodMac::Make(self);
    }
    return self;
}

- (NSTextInputContext*)inputContext
{
    if (_inputMethod && _inputMethod->GetEditable())
    {
        return [super inputContext];
    }
    return nil;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)updateTrackingAreas
{
    if (_trackingArea)
    {
        [self removeTrackingArea:_trackingArea];
        _trackingArea = nil;
    }
    auto const trackingOptions = (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveAlways);
    auto const trackingRect = self.bounds;
    _trackingArea = [[NSTrackingArea alloc] initWithRect:trackingRect options:trackingOptions owner:self userInfo:nil];
    [self addTrackingArea:_trackingArea];
}

- (void)cursorUpdate:(NSEvent *)event
{
    [super cursorUpdate:event];
}

- (unsigned long long)nextIdentifier
{
    static auto identifier = 0ull;
    return ++identifier;
}

- (void)sendPointerEvent:(NSEvent*)event forMotion:(Event<PlatformPointerEvent::Motion>&)motionEvent byTouch:(NSTouch*)touch
{
    @try
    {
        auto const pointerId = PointerId([[_identifiers objectForKey:touch.identity] unsignedLongLongValue]);
        auto const pointerType = (touch.type & NSTouchTypeDirect) ? PointerType::Touch : PointerType::TouchPad; // ??
        auto const locationInView = [touch locationInView:self];
        auto const modifiers = [self modifierFlagsToModifierKeyFlags:event.modifierFlags];

        motionEvent->SetPointerId(pointerId);
        motionEvent->SetPointerType(pointerType);
        motionEvent->SetTimestamp(NSTimeIntervalToMonotonicTime(event.timestamp));
        motionEvent->SetPrimaryPointer(false); // TODO: Implement primary pointer logic
        motionEvent->SetPosition({locationInView.x, self.bounds.size.height - locationInView.y});
        motionEvent->SetButtons(PointerButtonFlags::Button1);
        motionEvent->SetModifiers(modifiers);

        auto sendingEvent = Event<>(motionEvent);
        _callbackOnPointer(_data, sendingEvent);
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
}

- (PointerType)pointingDeviceTypeToPointerType:(NSPointingDeviceType)pointingDeviceType
{
    switch (pointingDeviceType)
    {
        case NSPointingDeviceTypeCursor:
            return PointerType::Mouse;
        case NSPointingDeviceTypeEraser:
            return PointerType::Pen;
        case NSPointingDeviceTypePen:
            return PointerType::Pen;
        case NSPointingDeviceTypeUnknown:
            return PointerType::Other;
    }
    return PointerType::Other;
}

- (ModifierKeyFlags)modifierFlagsToModifierKeyFlags:(NSEventModifierFlags)modifierFlags
{
    auto flags = ModifierKeyFlags::None;
    if (modifierFlags & NSEventModifierFlagShift)
    {
        flags |= ModifierKeyFlags::Shift;
    }
    if (modifierFlags & NSEventModifierFlagControl)
    {
        flags |= ModifierKeyFlags::Control;
    }
    if (modifierFlags & NSEventModifierFlagOption)
    {
        flags |= ModifierKeyFlags::Alt;
    }
    if (modifierFlags & NSEventModifierFlagCommand)
    {
        flags |= ModifierKeyFlags::Meta;
    }
    return flags;
}

- (PointerButtonFlags)pressedMouseButtonsToButtonState
{
    PointerButtonFlags buttonState = PointerButtonFlags::None;
    if (NSEvent.pressedMouseButtons & (1 << 0))
    {
        buttonState |= PointerButtonFlags::Button1;
    }
    if (NSEvent.pressedMouseButtons & (1 << 1))
    {
        buttonState |= PointerButtonFlags::Button2;
    }
    if (NSEvent.pressedMouseButtons & (1 << 2))
    {
        buttonState |= PointerButtonFlags::Button3;
    }
    if (NSEvent.pressedMouseButtons & (1 << 3))
    {
        buttonState |= PointerButtonFlags::ButtonX1;
    }
    if (NSEvent.pressedMouseButtons & (1 << 4))
    {
        buttonState |= PointerButtonFlags::ButtonX2;
    }
    return buttonState;
}

- (PointerId)mousePointerIdentifier
{
    static auto const mousePointerIdentifier = [self nextIdentifier];
    return PointerId(mousePointerIdentifier);
}

- (void)sendPointerEvent:(NSEvent*)event forMotion:(Event<PlatformPointerEvent::Motion>&)motionEvent
{
    @try 
    {
        auto const locationInView = [self convertPoint:event.locationInWindow fromView:nil];
        auto const buttons = [self pressedMouseButtonsToButtonState];
        auto const modifiers = [self modifierFlagsToModifierKeyFlags:event.modifierFlags];

        auto subtype = NSEventSubtypeMouseEvent;
        if (!motionEvent.Is<PlatformPointerEvent::Motion::Enter>() && !motionEvent.Is<PlatformPointerEvent::Motion::Leave>())
        {
            subtype = event.subtype;
        }

        auto pointerType = PointerType::Mouse;
        auto pointerIdentifier = PointerId([self mousePointerIdentifier]);
        if (subtype == NSEventSubtypeTabletProximity || subtype == NSEventSubtypeTabletPoint)
        {
            pointerType = [self pointingDeviceTypeToPointerType:event.pointingDeviceType];
            pointerIdentifier = event.pointingDeviceID;
        }

        motionEvent->SetPointerId(pointerIdentifier);
        motionEvent->SetPointerType(pointerType);
        motionEvent->SetPrimaryPointer(true);
        motionEvent->SetPosition({locationInView.x,  self.bounds.size.height- locationInView.y});
        motionEvent->SetButtons(buttons);
        motionEvent->SetModifiers(modifiers);
        motionEvent->SetTimestamp(NSTimeIntervalToMonotonicTime(event.timestamp));

        auto sendingEvent = Event<>(motionEvent);
        _callbackOnPointer(_data, sendingEvent);
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
}

- (void)mouseDown:(NSEvent*)event
{
    @try
    {
        try
        {
            auto downEvent = Event<PlatformPointerEvent::Motion::Down>();
            SetPointerMotionEventParameter(event, downEvent, PointerButton::Button1);
            auto motionEvent = Event<PlatformPointerEvent::Motion>(downEvent);
            [self sendPointerEvent:event forMotion:motionEvent];
        }
        catch (...)
        {
            NSLog(@"Exception in mouseDown event handling");
            FW_DEBUG_ASSERT(false);
        }
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
}

- (void)mouseDragged:(NSEvent*)event
{
    @try
    {
        auto moveEvent = Event<PlatformPointerEvent::Motion::Move>();
        SetPointerMotionEventParameter(event, moveEvent, PointerButton::Button1);
        auto motionEvent = Event<PlatformPointerEvent::Motion>(moveEvent);
        [self sendPointerEvent:event forMotion:motionEvent];
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
}

- (void)mouseMoved:(NSEvent*)event
{
    @try
    {
        auto moveEvent = Event<PlatformPointerEvent::Motion::Move>();
        SetPointerMotionEventParameter(event, moveEvent, PointerButton::Unknown);
        auto motionEvent = Event<PlatformPointerEvent::Motion>(moveEvent);
        [self sendPointerEvent:event forMotion:motionEvent];
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
}

- (void)mouseUp:(NSEvent*)event
{
    @try
    {
        auto upEvent = Event<PlatformPointerEvent::Motion::Up>();
        SetPointerMotionEventParameter(event, upEvent, PointerButton::Button1);
        auto motionEvent = Event<PlatformPointerEvent::Motion>(upEvent);
        [self sendPointerEvent:event forMotion:motionEvent];
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
}

- (void)mouseEntered:(NSEvent*)event
{
    @try
    {
        auto motionEvent = Event<PlatformPointerEvent::Motion>(Event<PlatformPointerEvent::Motion::Enter>());
        [self sendPointerEvent:event forMotion:motionEvent];
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
}

- (void)mouseExited:(NSEvent*)event
{
    @try
    {
        auto motionEvent = Event<PlatformPointerEvent::Motion>(Event<PlatformPointerEvent::Motion::Leave>());
        [self sendPointerEvent:event forMotion:motionEvent];
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
}

- (void)rightMouseDown:(NSEvent*)event
{
    auto downEvent = Event<PlatformPointerEvent::Motion::Down>();
    SetPointerMotionEventParameter(event, downEvent, PointerButton::Button2);
    auto motionEvent = Event<PlatformPointerEvent::Motion>(downEvent);
    [self sendPointerEvent:event forMotion:motionEvent];
}

- (void)rightMouseDragged:(NSEvent*)event
{
    auto moveEvent = Event<PlatformPointerEvent::Motion::Move>();
    SetPointerMotionEventParameter(event, moveEvent, PointerButton::Button2);
    auto motionEvent = Event<PlatformPointerEvent::Motion>(moveEvent);
    [self sendPointerEvent:event forMotion:motionEvent];
}

- (void)rightMouseUp:(NSEvent*)event
{
    auto upEvent = Event<PlatformPointerEvent::Motion::Up>();
    SetPointerMotionEventParameter(event, upEvent, PointerButton::Button2);
    auto motionEvent = Event<PlatformPointerEvent::Motion>(upEvent);
    [self sendPointerEvent:event forMotion:motionEvent];
}

- (PointerButton)mouseOtherButtonFromEvent:(NSEvent*)event
{
    @try
    {
        if (event.buttonNumber == 2)
        {
            return PointerButton::Button3;
        }
        else if (event.buttonNumber == 3)
        {
            return PointerButton::ButtonX1;
        }
        else if (event.buttonNumber == 4)
        {
            return PointerButton::ButtonX2;
        }
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
    return PointerButton::Unknown;
}

- (void)otherMouseDown:(NSEvent*)event
{
    auto downEvent = Event<PlatformPointerEvent::Motion::Down>();
    SetPointerMotionEventParameter(event, downEvent, [self mouseOtherButtonFromEvent:event]);
    auto motionEvent = Event<PlatformPointerEvent::Motion>(downEvent);
    [self sendPointerEvent:event forMotion:motionEvent];
}

- (void)otherMouseDragged:(NSEvent*)event
{
    auto moveEvent = Event<PlatformPointerEvent::Motion::Move>();
    SetPointerMotionEventParameter(event, moveEvent, [self mouseOtherButtonFromEvent:event]);
    auto motionEvent = Event<PlatformPointerEvent::Motion>(moveEvent);
    [self sendPointerEvent:event forMotion:motionEvent];
}

- (void)otherMouseUp:(NSEvent*)event
{
    auto moveEvent = Event<PlatformPointerEvent::Motion::Up>();
    SetPointerMotionEventParameter(event, moveEvent, [self mouseOtherButtonFromEvent:event]);
    auto motionEvent = Event<PlatformPointerEvent::Motion>(moveEvent);
    [self sendPointerEvent:event forMotion:motionEvent];
}

- (void)scrollWheel:(NSEvent*)event
{
    auto const phase = event.phase;
    auto const momentumPhase = event.momentumPhase;
    auto const locationInView = [self convertPoint:event.locationInWindow fromView:nil];
    auto const buttons = [self pressedMouseButtonsToButtonState];
    auto const modifiers = [self modifierFlagsToModifierKeyFlags:event.modifierFlags];

    if (phase == NSEventPhaseNone && momentumPhase == NSEventPhaseNone)
    {
        @try
        {
            // Mouse scroll wheel event.
            auto wheelEventParameter = Event<PlatformPointerEvent::Action::Scroll>();
            wheelEventParameter->SetPointerId([self mousePointerIdentifier]);
            wheelEventParameter->SetPointerType(PointerType::Mouse);
            wheelEventParameter->SetPrimaryPointer(true);
            wheelEventParameter->SetTimestamp(NSTimeIntervalToMonotonicTime(event.timestamp));
            wheelEventParameter->SetPosition({locationInView.x, self.bounds.size.height - locationInView.y});
            wheelEventParameter->SetButtons(buttons);
            wheelEventParameter->SetModifiers(modifiers);
            wheelEventParameter->SetDeltaX(event.scrollingDeltaX);
            wheelEventParameter->SetDeltaY(event.scrollingDeltaY);
            wheelEventParameter->SetPrecision(event.hasPreciseScrollingDeltas ? PointerScrollPrecision::Precise : PointerScrollPrecision::Coarse);

            auto wheelEvent = Event<>(wheelEventParameter);
            _callbackOnPointer(_data, wheelEvent);
        }
        @catch (NSException* e)
        {
            NSLog(@"%@", e);
            FW_DEBUG_ASSERT(false);
        }
    }
    else if (
      phase == NSEventPhaseBegan || phase == NSEventPhaseChanged || phase == NSEventPhaseEnded || phase == NSEventPhaseCancelled || (phase == NSEventPhaseNone && momentumPhase != NSEventPhaseNone))
    {
        auto actionPhase = PointerActionPhase::None;
        if (phase == NSEventPhaseBegan)
        {
            actionPhase = PointerActionPhase::Begin;
        }
        else if (phase == NSEventPhaseChanged)
        {
            actionPhase = PointerActionPhase::Update;
        }
        else if (phase == NSEventPhaseEnded)
        {
            if (momentumPhase == NSEventPhaseNone || momentumPhase == NSEventPhaseEnded)
            {
                actionPhase = PointerActionPhase::End;
            }
            else
            {
                actionPhase = PointerActionPhase::Update;
            }
        }
        else if (phase == NSEventPhaseCancelled)
        {
            actionPhase = PointerActionPhase::Cancel;
        }
        else if (phase == NSEventPhaseNone)
        {
            if (momentumPhase == NSEventPhaseChanged)
            {
                actionPhase = PointerActionPhase::Update;
            }
            else if (momentumPhase == NSEventPhaseEnded)
            {
                actionPhase = PointerActionPhase::End;
            }
            else if (momentumPhase == NSEventPhaseCancelled)
            {
                actionPhase = PointerActionPhase::Cancel;
            }
        }

        @try
        {
            auto scrollEventParameter = Event<PlatformPointerEvent::Action::Swipe>();
            scrollEventParameter->SetPointerId([self mousePointerIdentifier]);
            scrollEventParameter->SetPointerType(PointerType::TouchPad);
            scrollEventParameter->SetPrimaryPointer(true);
            scrollEventParameter->SetTimestamp(NSTimeIntervalToMonotonicTime(event.timestamp));
            scrollEventParameter->SetPosition({locationInView.x, self.bounds.size.height - locationInView.y});
            scrollEventParameter->SetButtons(buttons);
            scrollEventParameter->SetModifiers(modifiers);
            scrollEventParameter->SetPhase(actionPhase);
            scrollEventParameter->SetDeltaX(event.scrollingDeltaX);
            scrollEventParameter->SetDeltaY(event.scrollingDeltaY);
            scrollEventParameter->SetInertial(momentumPhase != NSEventPhaseNone);

            auto scrollEvent = Event<>(scrollEventParameter);
            _callbackOnPointer(_data, scrollEvent);
        }
        @catch (NSException* e)
        {
            NSLog(@"%@", e);
            FW_DEBUG_ASSERT(false);
        }
    }
}

- (void)sendPointerEventForTablet:(NSEvent*)event
{
    // TODO:
}

- (void)tabletProximity:(NSEvent*)event
{
    [self sendPointerEventForTablet:event];
}

- (void)tabletPoint:(NSEvent*)event
{
    [self sendPointerEventForTablet:event];
}

- (void)touchesBeganWithEvent:(NSEvent*)event
{
    auto touches = [event touchesMatchingPhase:NSTouchPhaseBegan inView:self];

    for (NSTouch* touch in touches)
    {
        if ([_identifiers objectForKey:touch.identity] == nil)
        {
            [_identifiers setObject:[NSNumber numberWithUnsignedLongLong:[self nextIdentifier]] forKey:touch.identity];
        }
        auto downEvent = Event<PlatformPointerEvent::Motion::Down>();
        downEvent->SetButton(PointerButton::Button1);
        downEvent->SetAltitude(90);
        downEvent->SetPressure(1.0);
        auto motionEvent = Event<PlatformPointerEvent::Motion>(downEvent);
        [self sendPointerEvent:event forMotion:motionEvent byTouch:touch];
    }
}

- (void)touchesMovedWithEvent:(NSEvent*)event
{
    auto touches = [event touchesMatchingPhase:NSTouchPhaseMoved inView:self];

    for (NSTouch* touch in touches)
    {
        if ([_identifiers objectForKey:touch.identity])
        {
            auto moveEvent = Event<PlatformPointerEvent::Motion::Move>();
            moveEvent->SetAltitude(90);
            moveEvent->SetPressure(1);
            moveEvent->SetButton(PointerButton::Button1);
            auto motionEvent = Event<PlatformPointerEvent::Motion>(moveEvent);
            [self sendPointerEvent:event forMotion:motionEvent byTouch:touch];
        }
    }
}

- (void)touchesEndedWithEvent:(NSEvent*)event
{
    auto touches = [event touchesMatchingPhase:NSTouchPhaseEnded inView:self];

    for (NSTouch* touch in touches)
    {
        if ([_identifiers objectForKey:touch.identity])
        {
            [_identifiers removeObjectForKey:touch.identity];
            auto upEvent = Event<PlatformPointerEvent::Motion::Up>();
            upEvent->SetAltitude(90);
            upEvent->SetButton(PointerButton::Button1);
            auto motionEvent = Event<PlatformPointerEvent::Motion>(upEvent);
            [self sendPointerEvent:event forMotion:motionEvent byTouch:touch];
        }
    }
}

- (void)touchesCancelledWithEvent:(NSEvent*)event
{
    auto touches = [event touchesMatchingPhase:NSTouchPhaseCancelled inView:self];

    for (NSTouch* touch in touches)
    {
        if ([_identifiers objectForKey:touch.identity])
        {
            [_identifiers removeObjectForKey:touch.identity];
            auto motionEvent = Event<PlatformPointerEvent::Motion>(Event<PlatformPointerEvent::Motion::Cancel>());
            [self sendPointerEvent:event forMotion:motionEvent byTouch:touch];
        }
    }
}

- (PointerActionPhase)actionPhaseFromEvent:(NSEvent*)event
{
    switch (event.phase)
    {
        case NSEventPhaseBegan:
            return PointerActionPhase::Begin;
        case NSEventPhaseChanged:
            return PointerActionPhase::Update;
        case NSEventPhaseEnded:
            return PointerActionPhase::End;
        case NSEventPhaseCancelled:
            return PointerActionPhase::Cancel;
    }
    return PointerActionPhase::None;
}

- (void)sendPointerEvent:(NSEvent*)event forAction:(Event<PlatformPointerEvent::Action>&)actionEvent
{
    @try
    {
        auto const locationInView = [self convertPoint:event.locationInWindow fromView:nil];

        actionEvent->SetTimestamp(NSTimeIntervalToMonotonicTime(event.timestamp));
        actionEvent->SetPointerType(PointerType::TouchPad);
        actionEvent->SetPointerId([self mousePointerIdentifier]);
        actionEvent->SetPrimaryPointer(true);
        actionEvent->SetPosition({locationInView.x, self.bounds.size.height - locationInView.y});

        auto sendingEvent = Event<>(actionEvent);
        _callbackOnPointer(_data, sendingEvent);
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
}

- (void)magnifyWithEvent:(NSEvent*)event
{
    auto magnifyEventParameter = Event<PlatformPointerEvent::Action::Magnify>();
    magnifyEventParameter->SetScale(event.magnification);
    magnifyEventParameter->SetPhase([self actionPhaseFromEvent:event]);
    auto actionEvent = Event<PlatformPointerEvent::Action>(magnifyEventParameter);
    [self sendPointerEvent:event forAction:actionEvent];
}

- (void)rotateWithEvent:(NSEvent*)event
{
    auto actionEventParameter = Event<PlatformPointerEvent::Action::Rotate>();
    actionEventParameter->SetRotation(event.rotation);
    actionEventParameter->SetPhase([self actionPhaseFromEvent:event]);
    auto actionEvent = Event<PlatformPointerEvent::Action>(actionEventParameter);
    [self sendPointerEvent:event forAction:actionEvent];
}

- (void)swipeWithEvent:(NSEvent*)event
{
    auto actionEventParameter = Event<PlatformPointerEvent::Action::Swipe>();
    actionEventParameter->SetDeltaX(-event.deltaX);
    actionEventParameter->SetDeltaY(-event.deltaY);
    actionEventParameter->SetPhase([self actionPhaseFromEvent:event]);
    auto actionEvent = Event<PlatformPointerEvent::Action>(actionEventParameter);
    [self sendPointerEvent:event forAction:actionEvent];
}

- (void)keyDown:(NSEvent*)event
{
    @try
    {
        [[self inputContext] handleEvent:event];

        if (!self.hasMarkedText)
        {
            auto keyEventParameter = Event<PlatformKeyEvent::Down>();
            keyEventParameter->SetKey(PlatformKeyboardLayoutMac::MapControlCharsToNamedKey(event.characters));
            keyEventParameter->SetUnmodifiedKey(PlatformKeyboardLayoutMac::MapControlCharsToNamedKey([event charactersByApplyingModifiers:0]));
            keyEventParameter->SetText(PlatformStringFunctionMac::ConvertNSStringToString(event.characters));
            keyEventParameter->SetModifiers([self modifierFlagsToModifierKeyFlags:event.modifierFlags]);
            keyEventParameter->SetRepeat(event.isARepeat);
            keyEventParameter->SetTimestamp(NSTimeIntervalToMonotonicTime(event.timestamp));
            auto keyEvent = Event<>(keyEventParameter);
            _callbackOnKey(_data, keyEvent);
        }
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
}

- (void)keyUp:(NSEvent*)event
{
    @try
    {
        auto keyEventParameter = Event<PlatformKeyEvent::Up>();
        keyEventParameter->SetTimestamp(NSTimeIntervalToMonotonicTime(event.timestamp));
        keyEventParameter->SetKey(PlatformKeyboardLayoutMac::MapControlCharsToNamedKey(event.characters));
        keyEventParameter->SetUnmodifiedKey(PlatformKeyboardLayoutMac::MapControlCharsToNamedKey([event charactersByApplyingModifiers:0]));

        auto keyEvent = Event<>(keyEventParameter);
        _callbackOnKey(_data, keyEvent);
    }
    @catch (NSException* e)
    {
        NSLog(@"%@", e);
        FW_DEBUG_ASSERT(false);
    }
}

- (void)flagsChanged:(NSEvent*)event
{
    auto const modifierFlags = [event modifierFlags];
    auto const keyCode = [event keyCode];

    auto keyDown = False;
    auto key = String();
    if (keyCode == kVK_Command || keyCode == kVK_RightCommand)
    {
        key = Key::Meta;
        keyDown = (modifierFlags & NSEventModifierFlagCommand) != 0;
    }
    else if (keyCode == kVK_Control || keyCode == kVK_RightControl)
    {
        key = Key::Control;
        keyDown = (modifierFlags & NSEventModifierFlagControl) != 0;
    }
    else if (keyCode == kVK_Option || keyCode == kVK_RightOption)
    {
        key = Key::Alt;
        keyDown = (modifierFlags & NSEventModifierFlagOption) != 0;
    }
    else if (keyCode == kVK_Shift || keyCode == kVK_RightShift)
    {
        key = Key::Shift;
        keyDown = (modifierFlags & NSEventModifierFlagShift) != 0;
    }
    else
    {
        return;
    }

    if (keyDown)
    {
        auto keyEventParameter = Event<PlatformKeyEvent::Down>();
        keyEventParameter->SetKey(key);
        keyEventParameter->SetUnmodifiedKey(key);
        keyEventParameter->SetModifiers([self modifierFlagsToModifierKeyFlags:modifierFlags]);
        keyEventParameter->SetTimestamp(NSTimeIntervalToMonotonicTime(event.timestamp));
        auto keyEvent = Event<>(keyEventParameter);
        _callbackOnKey(_data, keyEvent);
    }
    else
    {
        auto keyEventParameter = Event<PlatformKeyEvent::Up>();
        keyEventParameter->SetTimestamp(NSTimeIntervalToMonotonicTime(event.timestamp));
        keyEventParameter->SetKey(key);
        keyEventParameter->SetUnmodifiedKey(key);
        auto keyEvent = Event<>(keyEventParameter);
        _callbackOnKey(_data, keyEvent);
    }
}

- (void)insertText:(id)string replacementRange:(NSRange)replacementRange
{
    if (_inputMethod)
    {
        [_inputMethod->GetTextInputClient() insertText:string replacementRange:replacementRange];
    }
}

- (void)doCommandBySelector:(SEL)selector
{
    if (_inputMethod)
    {
        [_inputMethod->GetTextInputClient() doCommandBySelector:selector];
    }
}

- (void)setMarkedText:(id)string selectedRange:(NSRange)selectedRange replacementRange:(NSRange)replacementRange
{
    if (_inputMethod)
    {
        [_inputMethod->GetTextInputClient() setMarkedText:string selectedRange:selectedRange replacementRange:replacementRange];
    }
}

- (void)unmarkText
{
    if (_inputMethod)
    {
        [_inputMethod->GetTextInputClient() unmarkText];
    }
}

- (NSRange)selectedRange
{
    if (_inputMethod)
    {
        return [_inputMethod->GetTextInputClient() selectedRange];
    }
    return NSMakeRange(NSNotFound, 0);
}

- (NSRange)markedRange
{
    if (_inputMethod)
    {
        return [_inputMethod->GetTextInputClient() markedRange];
    }
    return NSMakeRange(NSNotFound, 0);
}

- (BOOL)hasMarkedText
{
    if (_inputMethod)
    {
        return [_inputMethod->GetTextInputClient() hasMarkedText];
    }
    return NO;
}

- (nullable NSAttributedString*)attributedSubstringForProposedRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange
{
    if (_inputMethod)
    {
        return [_inputMethod->GetTextInputClient() attributedSubstringForProposedRange:range actualRange:actualRange];
    }
    return nil;
}

- (NSArray<NSAttributedStringKey>*)validAttributesForMarkedText
{
    if (_inputMethod)
    {
        return [_inputMethod->GetTextInputClient() validAttributesForMarkedText];
    }
    return @[];
}

- (NSRect)firstRectForCharacterRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange
{
    if (_inputMethod)
    {
        return [_inputMethod->GetTextInputClient() firstRectForCharacterRange:range actualRange:actualRange];
    }
    return NSZeroRect;
}

- (NSUInteger)characterIndexForPoint:(NSPoint)point
{
    if (_inputMethod)
    {
        return [_inputMethod->GetTextInputClient() characterIndexForPoint:point];
    }
    return NSNotFound;
}

@end

@interface PlatformWindowDelegate : NSObject <NSWindowDelegate>
@property(assign, nonatomic) void* data;
@property(assign, nonatomic) void (*callbackOnResize)(void* data);
@property(assign, nonatomic) void (*callbackOnMove)(void* data);
@property(assign, nonatomic) void (*callbackOnScreenChange)(void* data);
@property(assign, nonatomic) void (*callbackOnScreenProfileChange)(void* data);
@property(assign, nonatomic) void (*callbackOnBackingPropertyChange)(void* data);
@property(assign, nonatomic) void (*callbackOnBecomeKey)(void* data);
@property(assign, nonatomic) void (*callbackOnResignKey)(void* data);
@property(assign, nonatomic) BOOL (*callbackOnShouldClose)(void* data);
@property(assign, nonatomic) void (*callbackOnFrameUpdate)(void* data, CADisplayLink* displayLink);
@property(assign, nonatomic) BOOL (*callbackOnWillClose)(void* data);
@property(strong, nonatomic) NSWindow* window;
@property(strong, nonatomic) CADisplayLink* displayLink;
@property(assign, nonatomic) NSRect restoredFrame;
@property(assign, nonatomic) NSRect zoomedRect;
- (instancetype)initWithOptions:(PlatformWindowOptions const&)options;
- (void)setContentView:(NSView*)contentView;
- (void)windowDidResize:(NSNotification*)notification;
- (void)windowDidMove:(NSNotification*)notification;
- (void)windowDidChangeScreen:(NSNotification*)notification;
- (void)windowDidChangeScreenProfile:(NSNotification*)notification;
- (void)windowDidChangeBackingProperties:(NSNotification*)notification;
- (void)windowDidBecomeKey:(NSNotification*)notification;
- (void)windowDidResignKey:(NSNotification*)notification;
- (BOOL)windowShouldClose:(NSWindow*)sender;
- (BOOL)windowShouldZoom:(NSWindow *)window toFrame:(NSRect)newFrame;
- (void)windowWillClose:(NSNotification*)notification;
- (void)windowWillMiniaturize:(NSNotification *)notification;
- (NSRect)windowWillUseStandardFrame:(NSWindow *)window defaultFrame:(NSRect)newFrame;
- (void)requestFrame;
- (void)displayLinkDidUpdate:(CADisplayLink*)sender;
@end

@implementation PlatformWindowDelegate

- (NSWindowStyleMask)getStyleMaskFrom:(PlatformWindowOptions const&)options
{
    auto style = NSWindowStyleMask(0);

    style |= NSWindowStyleMaskFullSizeContentView;

    if (options.closable)
    {
        style |= NSWindowStyleMaskClosable;
    }

    if (options.resizable)
    {
        style |= NSWindowStyleMaskResizable;
    }

    if (options.minimizable)
    {
        style |= NSWindowStyleMaskMiniaturizable;
    }

    if (!options.hasFrame)
    {
        style |= NSWindowStyleMaskBorderless;
    }

    if (options.hasTitleBar)
    {
        style |= NSWindowStyleMaskTitled;
    }
    return style;
}

- (instancetype)initWithOptions:(PlatformWindowOptions const&)options
{
    self = [super init];

    if (self)
    {
        auto const style = [self getStyleMaskFrom:options];
        _window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 500, 500) styleMask:style backing:NSBackingStoreBuffered defer:YES];
        [_window setReleasedWhenClosed:NO];
        [_window setTitlebarAppearsTransparent:YES];
        [_window setLevel:NSNormalWindowLevel];

        if (options.backgroundStyle == WindowBackgroundStyle::System)
        {
            [_window setBackgroundColor:nil];
        }
        else if (options.backgroundStyle == WindowBackgroundStyle::Transparent)
        {
            [_window setBackgroundColor:[NSColor colorWithRed:0 green:0 blue:0 alpha:0]];
        }

        _displayLink = [_window displayLinkWithTarget:self selector:@selector(displayLinkDidUpdate:)];
        [_displayLink setPaused:YES];
        [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];

        [_window setDelegate:self];
    }
    return self;
}

- (void)setContentView:(NSView*)contentView
{
    [contentView setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
    [_window setContentView:contentView];
}

- (void)windowDidResize:(NSNotification*)notification
{
    _callbackOnResize(_data);
}

- (void)windowDidMove:(NSNotification*)notification
{
    _callbackOnMove(_data);
}

- (void)windowDidChangeScreen:(NSNotification*)notification
{
    _callbackOnScreenChange(_data);
}

- (void)windowDidChangeScreenProfile:(NSNotification*)notification
{
    _callbackOnScreenProfileChange(_data);
}

- (void)windowDidChangeBackingProperties:(NSNotification*)notification
{
    _callbackOnBackingPropertyChange(_data);
}

- (void)windowDidBecomeKey:(NSNotification*)notification
{
    _callbackOnBecomeKey(_data);
}

- (void)windowDidResignKey:(NSNotification*)notification
{
    _callbackOnResignKey(_data);
}

- (BOOL)windowShouldClose:(NSWindow*)sender
{
    return _callbackOnShouldClose(_data);
}

- (BOOL)windowShouldZoom:(NSWindow*)window toFrame:(NSRect)newFrame
{
    if (NSEqualRects(newFrame, _zoomedRect))
    {
        _restoredFrame = [self.window frame];
    }
    return YES;
}

- (void)windowWillClose:(NSNotification*)notification
{
    _displayLink = nil;
    _window = nil;
    _callbackOnWillClose(_data);
}

- (void)windowWillMiniaturize:(NSNotification*)notification
{
    _restoredFrame = [self.window frame];
}

- (NSRect)windowWillUseStandardFrame:(NSWindow*)window defaultFrame:(NSRect)newFrame
{
    _zoomedRect = newFrame;
    return newFrame;
}

- (void)requestFrame
{
    [_displayLink setPaused:NO];
}

- (void)displayLinkDidUpdate:(CADisplayLink*)sender
{
    [sender setPaused:YES];
    _callbackOnFrameUpdate(_data, sender);
}

@end

namespace FW_DETAIL_NS
{
auto PlatformWindowMac::Make(
  Shared<PlatformWindowContextMac> const& context,
  Shared<PlatformViewLayerVisualContextMac> const& visualContext,
  PlatformWindowOptions const& options,
  Delegate const& delegate) -> Shared<PlatformWindowMac>
{
    return PlatformWindow::MakeDerived<PlatformWindowMac>(context, visualContext, options, delegate);
}

PlatformWindowMac::PlatformWindowMac(
  PassKey<PlatformWindow> key,
  Shared<PlatformWindowContextMac> const& context,
  Shared<PlatformViewLayerVisualContextMac> const& visualContext,
  PlatformWindowOptions const& options,
  Delegate const& delegate)
  : PlatformWindow(key, delegate)
  , _context {context}
  , _visualContext {visualContext}
  , _options {options}
  , _sizeConstraints {BoxConstraints::MakeUnconstrained()}
{
}

auto PlatformWindowMac::IsVisible() -> Bool
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            return _windowDelegate.window.isVisible;
        }
        return false;
    }
}

auto PlatformWindowMac::SetVisible(Bool const visible) -> void
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            if (visible != Bool(_windowDelegate.window.isVisible))
            {
                if (visible)
                {
                    [_windowDelegate.window makeKeyAndOrderFront:nil];
                }
                else
                {
                    [_windowDelegate.window orderOut:nil];
                }
            }
        }
    }
}

auto PlatformWindowMac::IsActive() -> Bool
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            return [_windowDelegate.window isKeyWindow];
        }
        return false;
    }
}

auto PlatformWindowMac::SetActive() -> void
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            [_windowDelegate.window makeKeyAndOrderFront:nil];
        }
    }
}

auto PlatformWindowMac::IsFocused() -> Bool
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            return [_windowDelegate.window isKeyWindow];
        }
        return false;
    }
}

auto PlatformWindowMac::SetFocus() -> void
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            return [_windowDelegate.window makeKeyWindow];
        }
    }
}

auto PlatformWindowMac::GetFrameRect() -> Rect<Vp>
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            return NativeToVpRect(_windowDelegate.window.frame);
        }
        return {};
    }
}

auto PlatformWindowMac::SetFrameRect(Rect<Vp> const& rect) -> void
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            [_windowDelegate.window setFrame:VpToNativeRect(rect) display:NO];
        }
    }
}

auto PlatformWindowMac::CapturePointer(PointerId const id) -> void
{
    (void)id;
}

auto PlatformWindowMac::ReleasePointer(PointerId const id) -> void
{
    (void)id;
}

auto PlatformWindowMac::GetRestoredFrameRect() -> Rect<Vp>
{
    @autoreleasepool
    {
        auto const window = _windowDelegate.window;
        if (window.isZoomed)
        {
            return NativeToVpRect([_windowDelegate restoredFrame]);
        }
        else
        {
            return GetFrameRect();
        }
    }
}

auto PlatformWindowMac::SetRestoredFrameRect(Rect<Vp> const& rect) -> void
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            if (_windowDelegate.window.isZoomed)
            {
                // TODO: Currently not supported.
            }
            else
            {
                SetFrameRect(rect);
            }
        }
    }
}

auto PlatformWindowMac::GetAreaBounds(WindowArea const windowArea) -> std::vector<Rect<Dp>>
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            if (windowArea == WindowArea::TitleBar)
            {
                auto getButtonFrame = [&](NSWindowButton buttonType) -> Rect<Vp> {
                    if (auto const button = [_windowDelegate.window standardWindowButton:buttonType])
                    {
                        auto const frame = [button convertRect:button.bounds toView:nil];
                        return Rect<Vp>(Point<Vp>(frame.origin.x, _windowDelegate.window.frame.size.height - (frame.origin.y + frame.size.height)), Size<Vp>(frame.size.width, frame.size.height));
                    }
                    return Rect<Vp>();
                };
                return {
                  UnitFunction::ConvertVpToDp(getButtonFrame(NSWindowCloseButton), GetDisplayScale()),
                  UnitFunction::ConvertVpToDp(getButtonFrame(NSWindowMiniaturizeButton), GetDisplayScale()),
                  UnitFunction::ConvertVpToDp(getButtonFrame(NSWindowZoomButton), GetDisplayScale()),
                };
            }
        }
    }
    return {};
}

auto PlatformWindowMac::GetAreaInsets(WindowArea const area) -> EdgeInsets
{
    @autoreleasepool
    {
        auto const window = _windowDelegate.window;
        if (window)
        {
            if (area == WindowArea::Frame)
            {
                auto const frame = window.frame;
                auto const contentRect = [NSWindow contentRectForFrameRect:frame styleMask:window.styleMask];
                return EdgeInsets(
                  contentRect.origin.x - frame.origin.x,
                  contentRect.origin.y + contentRect.size.height - frame.origin.y - frame.size.height,
                  contentRect.origin.x + contentRect.size.width - frame.origin.x - frame.size.width,
                  contentRect.origin.y - frame.origin.y);
            }
            else if (area == WindowArea::TitleBar)
            {
                auto titleBarHeight = Dp(0);
                if (window.styleMask & NSWindowStyleMaskTitled)
                {
                    titleBarHeight = Dp(window.frame.size.height - (window.contentLayoutRect.origin.y + window.contentLayoutRect.size.height));
                }
                return EdgeInsets(0, titleBarHeight, 0, 0);
            }
        }
        return {};
    }
}

auto PlatformWindowMac::GetDisplayScale() -> DisplayScale
{
    return 1.0;
}

auto PlatformWindowMac::GetBackingScale() -> BackingScale
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            return _windowDelegate.window.backingScaleFactor;
        }
        return 1.0;
    }
}

auto PlatformWindowMac::GetSizeConstraints() -> BoxConstraints
{
    return _sizeConstraints;
}

auto PlatformWindowMac::SetSizeConstraints(BoxConstraints const& constraints) -> void
{
    @autoreleasepool
    {
        _sizeConstraints = constraints;

        if (_windowDelegate.window)
        {
            auto const minSize = constraints.GetMinSize();
            auto const maxSize = constraints.GetMaxSize();
            [_windowDelegate.window setMinSize:NSMakeSize(static_cast<CGFloat>(minSize.GetWidth()), static_cast<CGFloat>(minSize.GetHeight()))];
            [_windowDelegate.window setMaxSize:NSMakeSize(static_cast<CGFloat>(maxSize.GetWidth()), static_cast<CGFloat>(maxSize.GetHeight()))];
        }
    }
}

auto PlatformWindowMac::GetTitle() -> String
{
    return _title;
}

auto PlatformWindowMac::SetTitle(String const& title) -> void
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            auto const nsTitle = PlatformStringFunctionMac::ConvertStringToNSString(title);
            [_windowDelegate.window setTitle:nsTitle];
        }
    }
}

auto PlatformWindowMac::Minimize() -> void
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            [_windowDelegate.window miniaturize:nil];
        }
    }
}

auto PlatformWindowMac::Maximize() -> void
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            [_windowDelegate.window setIsZoomed:YES];
        }
    }
}

auto PlatformWindowMac::Restore() -> void
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            [_windowDelegate.window setIsZoomed:NO];
        }
    }
}

auto PlatformWindowMac::IsClosed() -> Bool
{
    @autoreleasepool
    {
        return _windowDelegate.window == nil;
    }
}

auto PlatformWindowMac::Close() -> Async<Bool>
{
    @autoreleasepool
    {
        if (_windowDelegate.window)
        {
            [_windowDelegate.window performClose:nil];
        }
        co_return true;
    }
}

auto PlatformWindowMac::Destroy() -> void
{
    //TODO: 
}

auto PlatformWindowMac::Render() -> void
{
    if (_rootViewLayer)
    {
        _rootViewLayer->Render();
    }
}

auto PlatformWindowMac::RequestFrame() -> void
{
    @autoreleasepool
    {
        [_windowDelegate requestFrame];
    }
}

auto PlatformWindowMac::GetFrameTime() -> MonotonicTime
{
    return _currentFrameTime;
}

auto PlatformWindowMac::GetViewLayer() -> Shared<PlatformViewLayer>
{
    return _rootViewLayer;
}

auto PlatformWindowMac::GetInputMethod() -> Shared<PlatformInputMethod>
{
    return [_windowContentView inputMethod];
}

auto PlatformWindowMac::SetBackgroundColor(RGBColor const& color) -> void
{
    if (_options.backgroundStyle == WindowBackgroundStyle::Solid)
    {
        [_windowDelegate.window setBackgroundColor:[NSColor colorWithRed:static_cast<CGFloat>(color.GetRed().GetValue())
                                                    green:static_cast<CGFloat>(color.GetGreen().GetValue())
                                                     blue:static_cast<CGFloat>(color.GetBlue().GetValue())
                                                    alpha:1.0]];
    }
}

auto PlatformWindowMac::GetNativeHandle() -> NSWindow*
{
    @autoreleasepool
    {
        return _windowDelegate.window;
    }
}

auto PlatformWindowMac::Initialize() -> void
{
    @autoreleasepool
    {
        _windowContentView = [[PlatformWindowContentView alloc] initWithFrame:NSMakeRect(0, 0, 1, 1)];
        _windowDelegate = [[PlatformWindowDelegate alloc] initWithOptions:_options];

        [_windowDelegate setContentView:_windowContentView];

        [_windowContentView setData:this];
        [_windowContentView setCallbackOnPointer:[](void* data, Event<>& event) {
            auto const _this = static_cast<PlatformWindowMac*>(data);
            _this->SendPointerEventDetached(event);
        }];
        [_windowContentView setCallbackOnKey:[](void* data, Event<>& event) {
            auto const _this = static_cast<PlatformWindowMac*>(data);
            _this->SendKeyEventDetached(event);
        }];

        [_windowDelegate setData:this];
        [_windowDelegate setCallbackOnResize:[](void* data) -> void {
            auto const _this = static_cast<PlatformWindowMac*>(data);
            {
                auto event = Event<>(Event<PlatformWindowEvent::SizeChanged>());
                _this->SendWindowEventDetached(event);
            }
            {
                auto event = Event<>(Event<PlatformWindowEvent::AreaChanged>());
                _this->SendWindowEventDetached(event);
            }
        }];
        [_windowDelegate setCallbackOnMove:[](void* data) -> void {
            auto const _this = static_cast<PlatformWindowMac*>(data);
            auto event = Event<>(Event<PlatformWindowEvent::PositionChanged>());
            _this->SendWindowEventDetached(event);
        }];
        [_windowDelegate setCallbackOnScreenChange:[](void*) -> void {}];
        [_windowDelegate setCallbackOnScreenProfileChange:[](void*) -> void {}];
        [_windowDelegate setCallbackOnBackingPropertyChange:[](void* data) -> void {
            auto const _this = static_cast<PlatformWindowMac*>(data);
            auto event = Event<>(Event<PlatformWindowEvent::BackingScaleChanged>());
            _this->SendWindowEventDetached(event);
        }];
        [_windowDelegate setCallbackOnBecomeKey:[](void* data) -> void {
            auto const _this = static_cast<PlatformWindowMac*>(data);
            auto event = Event<>(Event<PlatformWindowEvent::FocusedChanged>());
            _this->SendWindowEventDetached(event);
        }];
        [_windowDelegate setCallbackOnResignKey:[](void* data) -> void {
            auto const _this = static_cast<PlatformWindowMac*>(data);
            auto event = Event<>(Event<PlatformWindowEvent::FocusedChanged>());
            _this->SendWindowEventDetached(event);
        }];
        [_windowDelegate setCallbackOnShouldClose:[](void* data) -> BOOL {
            auto const _this = static_cast<PlatformWindowMac*>(data);
            auto const parameter = Event<PlatformWindowEvent::CloseRequested>();
            auto const event = Event<>(parameter);
            auto const cancelled = Shared<Optional<Bool>>::Make();
            AsyncFunction::SpawnFn([self = _this->GetSelf(), e = event, cancelled]() mutable -> Task<void> {
                try
                {
                    if (co_await self->SendWindowEvent(e))
                    {
                        if (e.Is<PlatformWindowEvent::CloseRequested>())
                        {
                            *cancelled = e.As<PlatformWindowEvent::CloseRequested>()->IsCancelled();
                            co_return;
                        }
                    }
                    *cancelled = false;
                }
                catch (...)
                {
                    FW_DEBUG_ASSERT(false);
                    *cancelled = false;
                }
            }).Detach();

            while (!*cancelled)
            {
                ::CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.125, YES);
            }
            return **cancelled ? NO : YES;
        }];
        [_windowDelegate setCallbackOnFrameUpdate:[](void* data, CADisplayLink* displayLink) -> void {
            @autoreleasepool
            {
                auto const _this = static_cast<PlatformWindowMac*>(data);
                auto const currentFrameTime = NSTimeIntervalToMonotonicTime(displayLink.targetTimestamp);
                _this->_currentFrameTime = currentFrameTime;
                auto tickEvent = Event<PlatformFrameEvent::Tick>();
                tickEvent->SetTargetTimestamp(currentFrameTime);
                auto event = Event<>(tickEvent);
                _this->SendFrameEventDetached(event);
            }
        }];
        [_windowDelegate setCallbackOnWillClose:[](void* data) -> BOOL {
            auto const _this = static_cast<PlatformWindowMac*>(data);
            auto event = Event<>(Event<PlatformWindowEvent::Closed>());
            _this->SendWindowEventDetached(event);
            return YES;
        }];

        _rootViewLayer = PlatformRootViewLayerMac::Make(_visualContext, _windowDelegate.window);
        _currentFrameTime = NSTimeIntervalToMonotonicTime(_windowDelegate.displayLink.targetTimestamp);
    }
}

auto PlatformWindowMac::NativeToVpRect(NSRect const& rect) -> Rect<Vp>
{
    auto const mainScreen = [NSScreen mainScreen];
    auto const mainScreenFrame = mainScreen.frame;
    auto const origin = rect.origin;
    auto const size = rect.size;
    return Rect<Vp>(Point<Vp>(origin.x, mainScreenFrame.size.height - (origin.y + size.height)), Size<Vp>(size.width, size.height));
}

auto PlatformWindowMac::VpToNativeRect(Rect<Vp> const& rect) -> NSRect
{
    auto const mainScreen = [NSScreen mainScreen];
    auto const mainScreenFrame = mainScreen.frame;
    auto const position = rect.GetPosition();
    auto const size = rect.GetSize();
    auto const x = static_cast<CGFloat>(position.GetX());
    auto const y = static_cast<CGFloat>(position.GetY());
    auto const w = static_cast<CGFloat>(size.GetWidth());
    auto const h = static_cast<CGFloat>(size.GetHeight());
    return NSMakeRect(x, mainScreenFrame.size.height - (y + h), w, h);
}
}
