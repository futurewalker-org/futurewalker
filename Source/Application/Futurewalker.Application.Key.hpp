// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.String.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Logical key values.
///
class Key : NonConstructible
{
public:
    /* Special Keys */
    static String Unidentified;

    /* Modifier Keys */
    static String Alt;
    static String AltGraph;
    static String CapsLock;
    static String Control;
    static String Fn;
    static String FnLock;
    static String Meta;
    static String NumLock;
    static String ScrollLock;
    static String Shift;

    /* Navigation Keys */
    static String ArrowLeft;
    static String ArrowRight;
    static String ArrowUp;
    static String ArrowDown;
    static String End;
    static String Home;
    static String PageDown;
    static String PageUp;

    /* Whitespace Keys */
    static String Enter;
    static String Tab;

    /* Editing Keys */
    static String Backspace;
    static String Delete;
    static String Insert;

    /* UI Keys */
    static String ContextMenu;
    static String Escape;

    /* IME Composition Keys */
    static String Dead;
    static String Process;

    /* General-purpose Function Keys */
    static String F1;
    static String F2;
    static String F3;
    static String F4;
    static String F5;
    static String F6;
    static String F7;
    static String F8;
    static String F9;
    static String F10;
    static String F11;
    static String F12;

    /* ASCII Digit Strings */
    static String Digit0;
    static String Digit1;
    static String Digit2;
    static String Digit3;
    static String Digit4;
    static String Digit5;
    static String Digit6;
    static String Digit7;
    static String Digit8;
    static String Digit9;

    /* ASCII Alphabet Strings */
    static String A;
    static String B;
    static String C;
    static String D;
    static String E;
    static String F;
    static String G;
    static String H;
    static String I;
    static String J;
    static String K;
    static String L;
    static String M;
    static String N;
    static String O;
    static String P;
    static String Q;
    static String R;
    static String S;
    static String T;
    static String U;
    static String V;
    static String W;
    static String X;
    static String Y;
    static String Z;

    /* ASCII Symbol Strings */
    static String Grave;
    static String Minus;
    static String Plus;
    static String Equal;
    static String Backslash;
    static String Quote;
    static String Semicolon;
    static String Comma;
    static String Period;
    static String Slash;
    static String Space;
    static String LeftBracket;
    static String RightBracket;
};
}
}
