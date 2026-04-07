// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformInputMethodMac.hpp"
#include "Futurewalker.Application.Mac.PlatformInputEditableMac.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Graphics.GlyphRun.hpp"

#include <bit>

using namespace FW_NS;

@interface PlatformInputMethodContext : NSObject <NSTextInputClient>
@property(assign, nonatomic) PlatformInputMethodMac* inputMethod;
@property(weak, nonatomic) NSView* view;
@end

@implementation PlatformInputMethodContext
- (instancetype)initWithInputMethod:(PlatformInputMethodMac*)inputMethod
{
    self = [super init];
    if (self)
    {
        self.inputMethod = inputMethod;
    }
    return self;
}

- (void)insertText:(id)string replacementRange:(NSRange)replacementRange
{
    auto editable = self.inputMethod->GetEditable();
    if (!editable)
    {
        return;
    }

    auto insertString = (NSString*)nil;
    if ([string isKindOfClass:[NSString class]])
    {
        insertString = (NSString*)string;
    }
    else if ([string isKindOfClass:[NSAttributedString class]])
    {
        insertString = [(NSAttributedString*)string string];
    }

    if (insertString)
    {
        // Get UTF16 bytes without BOM.
        static_assert(std::endian::native == std::endian::little);
        auto const nativeEncoding = NSUTF16LittleEndianStringEncoding;
        auto data = [insertString dataUsingEncoding:nativeEncoding allowLossyConversion:NO];
        if (replacementRange.location != NSNotFound)
        {
            editable->SetU16SelectionRange(Range<CodeUnit>(replacementRange.location, replacementRange.location + replacementRange.length), editable->GetSelectionDirection(), true);
        }
        else if (editable->GetU16ComposingRange().GetLength() != 0)
        {
            auto const composingRange = Range<CodeUnit>::Normalize(editable->GetU16ComposingRange());
            editable->SetU16SelectionRange(composingRange, editable->GetSelectionDirection(), true);
        }
        auto const u16Data = reinterpret_cast<char16_t const*>(data.bytes);
        auto const u16Length = static_cast<NSInteger>(data.length / sizeof(char16_t));
        editable->InsertU16Text({u16Data, u16Length}, 1, true, false);
        editable->SetU16ComposingRange(editable->GetU16SelectionRange());
    }
}

- (void)setMarkedText:(id)string selectedRange:(NSRange)selectedRange replacementRange:(NSRange)replacementRange
{
    NSLog(@"setMarkedText: %@, selectedRange: %@, replacementRange: %@", string, NSStringFromRange(selectedRange), NSStringFromRange(replacementRange));

    auto editable = self.inputMethod->GetEditable();
    if (!editable)
    {
        return;
    }

    auto markedString = (NSString*)nil;
    if ([string isKindOfClass:[NSString class]])
    {
        markedString = (NSString*)string;
    }
    else if ([string isKindOfClass:[NSAttributedString class]])
    {
        markedString = [(NSAttributedString*)string string];
    }

    if (markedString)
    {
        // Get UTF-16 bytes without BOM.
        static_assert(std::endian::native == std::endian::little);
        auto const nativeEncoding = NSUTF16LittleEndianStringEncoding;
        auto const data = [markedString dataUsingEncoding:nativeEncoding allowLossyConversion:NO];
        auto const stringView = U16StringView(reinterpret_cast<char16_t const*>(data.bytes), static_cast<NSInteger>(data.length / sizeof(char16_t)));

        // NOTE: If there is no marked text, the current selection is replaced. If there is no selection, the string is inserted at the insertion point.
        // https://developer.apple.com/documentation/appkit/nstextinputclient/setmarkedtext(_:selectedrange:replacementrange:)?language=objc
        auto const composingRange = Range<CodeUnit>::Normalize(editable->GetU16ComposingRange());
        auto const selectionRange = Range<CodeUnit>::Normalize(editable->GetU16SelectionRange());
        if (composingRange.GetLength() != 0)
        {
            if (replacementRange.location != NSNotFound)
            {
                // replacementRange is relative to the composing range.
                auto const insertBegin = composingRange.GetBegin() + CodeUnit(replacementRange.location);
                auto const insertEnd = insertBegin + CodeUnit(replacementRange.length);
                editable->SetU16SelectionRange(Range<CodeUnit>(insertBegin, insertEnd), editable->GetSelectionDirection(), true);
            }
            else
            {
                // when replacementRange is not specified, replace whole composing range.
                editable->SetU16SelectionRange(composingRange, editable->GetSelectionDirection(), true);
            }
        }
        else
        {
            // otherwise, insert at current selection.
        }

        editable->InsertU16Text(stringView, 1, true, false);

        if (composingRange.GetLength() != 0)
        {
            auto const addedLength = CodeUnit(stringView.GetSize());
            if (replacementRange.location != NSNotFound)
            {
                // composing range has been partially replaced.
                auto const removedLength = CodeUnit(replacementRange.length);
                auto const composingBegin = composingRange.GetBegin();
                auto const composingEnd = composingRange.GetEnd() - removedLength + addedLength;
                editable->SetU16ComposingRange({composingBegin, composingEnd});
            }
            else
            {
                // whole composing range has been replaced.
                auto const composingBegin = composingRange.GetBegin();
                auto const composingEnd = composingBegin + addedLength;
                editable->SetU16ComposingRange({composingBegin, composingEnd});
            }
        }
        else
        {
            // composing text has been inserted at selection.
            auto const composingBegin = selectionRange.GetBegin();
            auto const composingEnd = composingBegin + CodeUnit(stringView.GetSize());
            editable->SetU16ComposingRange({composingBegin, composingEnd});
        }

        if (selectedRange.location != NSNotFound)
        {
            // selectedRange is relative to the start of the inserted text.
            auto const caretPosition = editable->GetU16SelectionRange().GetBegin();
            auto const selecteBegin = caretPosition + CodeUnit(selectedRange.location);
            auto const selecteEnd = selecteBegin + CodeUnit(selectedRange.length);
            editable->SetU16SelectionRange(Range<CodeUnit>(selecteBegin, selecteEnd), editable->GetSelectionDirection(), true);
        }
    }
}

- (void)unmarkText
{
    NSLog(@"unmarkText");

    auto editable = self.inputMethod->GetEditable();
    if (!editable)
    {
        return;
    }

    auto const composingRange = Range<CodeUnit>::Normalize(editable->GetU16ComposingRange());
    if (composingRange.GetLength() != 0)
    {
        auto const selectedRange = Range<CodeUnit>(composingRange.GetEnd(), composingRange.GetEnd());
        editable->SetU16ComposingRange(selectedRange);
        editable->SetU16SelectionRange(selectedRange, editable->GetSelectionDirection(), true);
    }
}

- (NSRange)selectedRange
{
    NSLog(@"selectedRange");
    auto editable = self.inputMethod->GetEditable();
    if (editable)
    {
        auto const selectionRange = Range<CodeUnit>::Normalize(editable->GetU16SelectionRange());
        auto const start = static_cast<NSUInteger>(selectionRange.GetBegin());
        auto const length = static_cast<NSUInteger>(selectionRange.GetLength());
        return NSMakeRange(start, length);
    }
    return NSMakeRange(NSNotFound, 0);
}

- (NSRange)markedRange
{
    NSLog(@"markedRange");
    auto editable = self.inputMethod->GetEditable();
    if (editable)
    {
        auto const composingRange = Range<CodeUnit>::Normalize(editable->GetU16ComposingRange());
        auto const start = static_cast<NSUInteger>(composingRange.GetBegin());
        auto const length = static_cast<NSUInteger>(composingRange.GetLength());
        return NSMakeRange(start, length);
    }
    return NSMakeRange(NSNotFound, 0);
}

- (BOOL)hasMarkedText
{
    NSLog(@"hasMarkedText");
    auto editable = self.inputMethod->GetEditable();
    if (editable)
    {
        if (editable->GetU16ComposingRange().GetLength() != 0)
        {
            return YES;
        }
    }
    return NO;
}

- (void)doCommandBySelector:(SEL)selector
{
    NSLog(@"doCommandBySelector: %@", NSStringFromSelector(selector));

    if (selector == @selector(deleteBackward:))
    {
        [self deleteBackward:self];
    }
    else if (selector == @selector(deleteForward:))
    {
        [self deleteForward:self];
    }
    else if (selector == @selector(insertNewline:))
    {
        [self insertNewline:self];
    }
}

- (void)deleteBackward:(id)sender
{
    NSLog(@"deleteBackward: %@", sender);
    if (auto editable = self.inputMethod->GetEditable())
    {
        if (editable->GetSelectionRange().GetLength() != 0)
        {
            editable->InsertText({}, 0);
        }
        else
        {
            editable->DeleteSurroundingText(1, 0);
        }
    }
}

- (void)deleteForward:(id)sender
{
    NSLog(@"deleteForward: %@", sender);
    if (auto editable = self.inputMethod->GetEditable())
    {
        if (editable->GetSelectionRange().GetLength() != 0)
        {
            editable->InsertText({}, 0);
        }
        else
        {
            editable->DeleteSurroundingText(0, 1);
        }
    }
}

- (void)insertNewline:(id)sender
{
    NSLog(@"insertNewline: %@", sender);
    if (auto editable = self.inputMethod->GetEditable())
    {
        editable->InsertLineBreak();
    }
}

- (nullable NSAttributedString*)attributedSubstringForProposedRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange
{
    return nil;
}

- (NSArray<NSAttributedStringKey>*)validAttributesForMarkedText
{
    return @[];
}

- (NSRect)firstRectForCharacterRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange
{
    auto editable = self.inputMethod->GetEditable();
    if (editable)
    {
        // Documentation says we can return rectangle for first line if range covers multiple lines.
        // FIXME: We currently don't have line informatino so just return rectangle of first glyph run here.
        auto const & layoutRect = editable->GetLayoutRect();
        auto const& layoutInfo = editable->GetLayoutInfo();
        auto const u8Range = editable->GetRangeFromU16Range(Range<CodeUnit>(range.location, range.location + range.length));

        auto const i = u8Range.GetBegin();
        if (auto const runIndex = layoutInfo.GetGlyphRunIndex(i))
        {
            if (auto const run = layoutInfo.GetGlyphRun(*runIndex))
            {
                auto const pos = layoutInfo.GetGlyphRunPosition(*runIndex).GetValueOrDefault();
                auto const metrics = run->GetMetrics();
                auto const width = run->GetAdvance();
                auto const height = metrics.ascent + metrics.descent;
                auto const rect = FW_NS::Rect<Dp>::Make(pos, FW_NS::Size<Dp>(width, height));
                auto const top = self.view.frame.size.height - (static_cast<CGFloat>(rect.y0 + rect.GetHeight()));
                auto const bottom = top + static_cast<CGFloat>(rect.GetHeight());
                auto const left = static_cast<CGFloat>(rect.x0);
                auto const right = static_cast<CGFloat>(rect.x1);
                if (actualRange)
                {
                    actualRange->location = static_cast<NSUInteger>(layoutInfo.GetGlyphRunCharacterIndex(*runIndex).GetValueOr(i));
                    actualRange->length = static_cast<NSUInteger>(run->GetText().GetCodePointCount());
                }
                return NSMakeRect(left, top, right - left, bottom - top);
            }
        }
        auto const windowFrame = self.view.window.frame;
        auto const x = windowFrame.origin.x + static_cast<CGFloat>(layoutRect.x0);
        auto const y = windowFrame.origin.y + windowFrame.size.height - static_cast<CGFloat>(layoutRect.y1);
        return NSMakeRect(x, y, 1, 1);
    }
    return {};
}

- (NSUInteger)characterIndexForPoint:(NSPoint)point
{
    auto const windowPoint = [self.view.window convertPointFromScreen:point];
    auto const viewPoint = [self.view convertPoint:windowPoint fromView:nil];

    auto editable = self.inputMethod->GetEditable();
    if (editable)
    {
        auto x = Dp(viewPoint.x);
        auto y = Dp(self.view.bounds.size.height - viewPoint.y);

        auto const layoutRect = editable->GetLayoutRect();
        x -= layoutRect.x0;
        y -= layoutRect.y0;

        auto const& layoutInfo = editable->GetLayoutInfo();

        for (auto i = Graphics::GlyphIndex(0); i < layoutInfo.GetGlyphCount(); ++i)
        {
            auto const pos = layoutInfo.GetGlyphPosition(i).GetValueOrDefault();
            if (x < pos.x)
            {
                if (i > 0)
                {
                    auto const characterIndex = layoutInfo.GetCharacterIndex(i - 1).GetValueOr(0);
                    auto const u16Range = editable->GetU16RangeFromRange({characterIndex, characterIndex});
                    return static_cast<NSUInteger>(u16Range.GetBegin());
                }
                else
                {
                    return 0;
                }
            }
        }
        return static_cast<NSUInteger>(editable->GetU16TextRange().GetEnd());
    }
    return 0;
}

@end

namespace FW_DETAIL_NS
{
auto PlatformInputMethodMac::Make(NSView* view) -> Shared<PlatformInputMethodMac>
{
    return Shared<PlatformInputMethodMac>::Make(PassKey<PlatformInputMethodMac>(), view);
}

PlatformInputMethodMac::PlatformInputMethodMac(PassKey<PlatformInputMethodMac>, NSView* view)
  : _view(view)
{
    @autoreleasepool
    {
        auto inputContext = [[PlatformInputMethodContext alloc] initWithInputMethod:this];
        inputContext.view = view;

        _textInputClient = inputContext;
        _inputContext = [view inputContext];
    }
}

auto PlatformInputMethodMac::GetEditable() -> Shared<PlatformInputEditableMac>
{
    return _editable;
}

auto PlatformInputMethodMac::SetEditable(Shared<PlatformInputEditable> const& editable) -> void
{
    auto editableMac = editable.TryAs<PlatformInputEditableMac>();
    if (_editable != editableMac)
    {
        if (_editable)
        {
            _editable->SetTextInputContext(nil);
        }

        _editable = editableMac;

        if (_editable)
        {
            _editable->SetTextInputContext(_inputContext);
        }
    }
}

auto PlatformInputMethodMac::GetTextInputClient() -> id<NSTextInputClient>
{
    return _textInputClient;
}
}
