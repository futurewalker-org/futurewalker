// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformKeyboardLayoutMac.hpp"
#include "Futurewalker.Application.Key.hpp"

#include "Futurewalker.Core.Mac.PlatformStringFunctionMac.hpp"

#include <Carbon/Carbon.h>

namespace FW_DETAIL_NS
{
auto PlatformKeyboardLayoutMac::MapControlCharsToNamedKey(NSString* chars) -> String
{
    @autoreleasepool
    {
        if (chars.length == 1)
        {
            auto const character = [chars characterAtIndex:0];
            switch (character)
            {
                // ASCII control characters.
                case '\x08':
                    return Key::Backspace;
                case '\x09':
                    return Key::Tab;
                case '\x0D':
                    return Key::Enter;
                case '\x1B':
                    return Key::Escape;
                case '\x7F':
                    return Key::Delete;
                // PUA range reserved by OpenStep for function keys.
                case NSUpArrowFunctionKey:
                    return Key::ArrowUp;
                case NSDownArrowFunctionKey:
                    return Key::ArrowDown;
                case NSLeftArrowFunctionKey:
                    return Key::ArrowLeft;
                case NSRightArrowFunctionKey:
                    return Key::ArrowRight;
                case NSF1FunctionKey:
                    return Key::F1;
                case NSF2FunctionKey:
                    return Key::F2;
                case NSF3FunctionKey:
                    return Key::F3;
                case NSF4FunctionKey:
                    return Key::F4;
                case NSF5FunctionKey:
                    return Key::F5;
                case NSF6FunctionKey:
                    return Key::F6;
                case NSF7FunctionKey:
                    return Key::F7;
                case NSF8FunctionKey:
                    return Key::F8;
                case NSF9FunctionKey:
                    return Key::F9;
                case NSF10FunctionKey:
                    return Key::F10;
                case NSF11FunctionKey:
                    return Key::F11;
                case NSF12FunctionKey:
                    return Key::F12;
                case NSF13FunctionKey:
                    return Key::F13;
                case NSF14FunctionKey:
                    return Key::F14;
                case NSF15FunctionKey:
                    return Key::F15;
                case NSF16FunctionKey:
                    return Key::F16;
                case NSF17FunctionKey:
                    return Key::F17;
                case NSF18FunctionKey:
                    return Key::F18;
                case NSF19FunctionKey:
                    return Key::F19;
                case NSF20FunctionKey:
                    return Key::F20;
                case NSF21FunctionKey:
                    return Key::F21;
                case NSF22FunctionKey:
                    return Key::F22;
                case NSF23FunctionKey:
                    return Key::F23;
                case NSF24FunctionKey:
                    return Key::F24;
                case NSInsertFunctionKey:
                    return Key::Insert;
                case NSDeleteFunctionKey:
                    return Key::Delete;
                case NSDeleteCharFunctionKey:
                    return Key::Backspace;
                case NSHomeFunctionKey:
                    return Key::Home;
                case NSEndFunctionKey:
                    return Key::End;
                case NSPageUpFunctionKey:
                    return Key::PageUp;
                case NSPageDownFunctionKey:
                    return Key::PageDown;
                case NSPrintScreenFunctionKey:
                    return Key::PrintScreen;
                case NSScrollLockFunctionKey:
                    return Key::ScrollLock;
                case NSPauseFunctionKey:
                    return Key::Pause;
                case NSPrintFunctionKey:
                    return Key::Print;
                case NSClearLineFunctionKey:
                    return Key::Clear;
                case NSSelectFunctionKey:
                    return Key::Select;
                case NSExecuteFunctionKey:
                    return Key::Execute;
                case NSUndoFunctionKey:
                    return Key::Undo;
                case NSRedoFunctionKey:
                    return Key::Redo;
                case NSFindFunctionKey:
                    return Key::Find;
                case NSHelpFunctionKey:
                    return Key::Help;
            }
        }
        return PlatformStringFunctionMac::ConvertNSStringToString(chars);
    }
}
}
