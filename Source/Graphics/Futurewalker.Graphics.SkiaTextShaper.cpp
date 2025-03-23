// SPDX-License-Identifier: MIT

#include "Futurewalker.Graphics.SkiaTextShaper.hpp"
#include "Futurewalker.Graphics.SkiaShapedText.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include <modules/skshaper/include/SkShaper.h>
#include <include/ports/SkTypeface_win.h>

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief Shape text.
///
/// @param text 
///
auto SkiaTextShaper::Shape(String const& text) -> Shared<ShapedText>
{
    static auto const fontMgr = SkFontMgr_New_DirectWrite();

    auto const width = SK_FloatInfinity;
    auto const font = SkFont(nullptr, 96);

    auto const str = StringFunction::ConvertStringToStdU8String(text);
    auto const utf8 = reinterpret_cast<const char*>(str.c_str());
    auto const utf8Bytes = str.size();

    auto const fontRunIterator = SkShaper::MakeFontMgrRunIterator(utf8, utf8Bytes, font, fontMgr);
    auto const bidiRunIterator = SkShaper::MakeBiDiRunIterator(utf8, utf8Bytes, 0xfe /*UBIDI_DEFAULT_LTR*/);
    auto const scriptRunIterator = SkShaper::MakeScriptRunIterator(utf8, utf8Bytes, 'latn');
    auto const languageRunIterator = SkShaper::MakeStdLanguageRunIterator(utf8, utf8Bytes);

    auto shaper = SkShaper::MakeShaperDrivenWrapper(fontMgr);
    auto runHandler = SkTextBlobBuilderRunHandler(utf8, SkPoint(0, 0));
    shaper->shape(utf8, utf8Bytes, *fontRunIterator, *bidiRunIterator, *scriptRunIterator, *languageRunIterator, nullptr, 0, width, &runHandler);

    // TODO: Implement custom RunHandler.
    auto const blob = runHandler.makeBlob();
    return Shared<SkiaShapedText>::Make(blob);
}
}
