// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaTextShaper.hpp"
#include "Futurewalker.Graphics.SkiaGlyphRun.hpp"
#include "Futurewalker.Graphics.ShapedText.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include "Futurewalker.Graphics.SkiaTypeface.hpp"
#include "Futurewalker.Graphics.PlatformSkiaFontManager.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include <include/core/SkFontMetrics.h>
#include <include/core/SkTextBlob.h>
#include <modules/skshaper/include/SkShaper.h>
#include <include/ports/SkTypeface_win.h>

#include <unicode/utf8.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace
{
auto GetSkTypeface(Shared<Typeface> const& typeface) -> sk_sp<SkTypeface>
{
    if (auto const skia = typeface.Maybe<SkiaTypeface>())
    {
        return skia->GetTypeface();
    }
    return {};
}

auto GetSkFontMgr() -> sk_sp<SkFontMgr>
{
    if (auto const skia = Locator::GetInstance<PlatformSkiaFontManager>())
    {
        return skia->GetSkFontMgr();
    }
    return {};
}

class SkiaRunHandler final : public SkShaper::RunHandler
{
    String _text;
    SkScalar _maxRunAscent = 0;
    SkScalar _maxRunDescent = 0;
    SkScalar _maxRunLeading = 0;
    SkScalar _advance = 0;
    SkiaGlyphRun::Buffer _buffer;
    std::vector<Shared<GlyphRun>> _runs;
    std::vector<ShapedText::Line> _lines;

public:
    explicit SkiaRunHandler(String const& text)
      : _text {text}
    {
    }

    // Called when beginning a line.
    auto beginLine() -> void override
    {
        _maxRunAscent = 0;
        _maxRunDescent = 0;
        _maxRunLeading = 0;
        _advance = 0;
    }

    // Called once for each run in a line. Can compute baselines and offsets.
    auto runInfo(const RunInfo& info) -> void override
    {
        auto metrics = SkFontMetrics();
        info.fFont.getMetrics(&metrics);
        _maxRunAscent = std::max(_maxRunAscent, -metrics.fAscent);
        _maxRunDescent = std::max(_maxRunDescent, metrics.fDescent);
        _maxRunLeading = std::max(_maxRunLeading, metrics.fLeading);
        _advance += info.fAdvance.fX;
    }

    // Called after all runInfo calls for a line.
    auto commitRunInfo() -> void override
    {
    }

    // Called for each run in a line after commitRunInfo. The buffer will be filled out.
    auto runBuffer(const RunInfo& info) -> Buffer override
    {
        auto const run = Shared<SkiaGlyphRun>::Make();
        _buffer = run->AllocBuffer(SInt64(info.glyphCount));
        _runs.push_back(run);

        return {_buffer.glyphs, _buffer.positions, nullptr, _buffer.clusters, SkPoint()};
    }

    // Called after each runBuffer is filled out.
    auto commitRunBuffer(const RunInfo& info) -> void override
    {
        auto run = _runs.back().Assume<SkiaGlyphRun>();
        run->SetFont(info.fFont);
        run->SetAdvance(info.fAdvance.fX);
        run->SetText(_text.GetSubstring(SInt64(info.utf8Range.begin()), SInt64(info.utf8Range.end())));

        auto const clusterOffset = static_cast<uint32_t>(info.utf8Range.fBegin);
        SkASSERT(0 <= clusterOffset);
        for (auto i = 0uz; i < info.glyphCount; ++i)
        {
            SkASSERT(_buffer.clusters[i] >= static_cast<uint32_t>(clusterOffset));
            _buffer.clusters[i] -= clusterOffset;
        }
    }

    // Called when ending a line.
    auto commitLine() -> void override
    {
        auto lineMetrics = FontMetrics();
        lineMetrics.SetAscent(_maxRunAscent);
        lineMetrics.SetDescent(_maxRunDescent);
        lineMetrics.SetLeading(_maxRunLeading);

        auto& line = _lines.emplace_back();
        line.SetRuns(std::exchange(_runs, {}));
        line.SetMetrics(lineMetrics);
        line.SetAdvance(_advance);
    }

    auto Finalize() &&
    {
        // TODO: CoW + eliminate copy.
        return ShapedText(_lines);
    }
};
}

///
/// @brief Shape text.
///
auto SkiaTextShaper::ShapeText(String const& text, Shared<Typeface> const& typeface, FontSize const size, Dp const maxWidth) -> ShapedText
{
    auto const width = static_cast<SkScalar>(maxWidth);
    auto const font = SkFont(GetSkTypeface(typeface), static_cast<SkScalar>(size));

    auto utf8 = String();
    if (!StringFunction::ValidateString(utf8, text))
    {
        FW_DEBUG_ASSERT(false);
    }

    auto const utf8Str = StringFunction::ConvertStringToStdString(utf8);
    auto const utf8Chars = utf8Str.c_str();
    auto const utf8Bytes = utf8Str.size();

    auto const fontMgr = GetSkFontMgr();

    auto const fontRunIterator = SkShaper::MakeFontMgrRunIterator(utf8Chars, utf8Bytes, font, fontMgr);
    auto const bidiRunIterator = SkShaper::MakeBiDiRunIterator(utf8Chars, utf8Bytes, 0xfe /*UBIDI_DEFAULT_LTR*/);
    auto const scriptRunIterator = SkShaper::MakeScriptRunIterator(utf8Chars, utf8Bytes, 'latn');
    auto const languageRunIterator = SkShaper::MakeStdLanguageRunIterator(utf8Chars, utf8Bytes);

    auto shaper = SkShaper::MakeShaperDrivenWrapper(fontMgr);
    auto runHandler = SkiaRunHandler(text);
    shaper->shape(utf8Chars, utf8Bytes, *fontRunIterator, *bidiRunIterator, *scriptRunIterator, *languageRunIterator, nullptr, 0, width, &runHandler);

    return std::move(runHandler).Finalize();
}

auto SkiaTextShaper::ShapeGlyph(char32_t const codePoint, Shared<Typeface> const& typeface, FontSize const size) -> ShapedText
{
    auto buffer = std::array<char8_t, U8_MAX_LENGTH>();
    auto offset = 0;
    U8_APPEND_UNSAFE(buffer.data(), offset, codePoint);
    return ShapeText(String(buffer.data(), offset), typeface, size, Dp::Infinity());
}
}
