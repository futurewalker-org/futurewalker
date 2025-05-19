// SPDX-License-Identifier: MIT

#include "Futurewalker.Graphics.SkiaTextShaper.hpp"
#include "Futurewalker.Graphics.SkiaShapedText.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include "Futurewalker.Graphics.SkiaTypeface.hpp"
#include "Futurewalker.Graphics.TextLayoutInfo.hpp"
#include "Futurewalker.Graphics.PlatformSkiaFontManager.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include <include/core/SkFontMetrics.h>
#include <include/core/SkTextBlob.h>
#include <modules/skshaper/include/SkShaper.h>
#include <include/ports/SkTypeface_win.h>

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
    SkScalar _minAdvance = 0;
    SkScalar _maxAdvance = 0;
    SkPoint _currentPosition = {};
    SkPoint _offset = {};
    SkTextBlobBuilder _builder;
    uint32_t* _clusters = nullptr;
    SkPoint* _points = nullptr;
    int _clusterOffset = 0;
    int _glyphCount = 0;
    std::vector<Graphics::TextLayoutInfo::GlyphInfo> _glyphInfo;

public:
    explicit SkiaRunHandler(String const& text)
      : _text {text}
    {
    }

    /** Called when beginning a line. */
    auto beginLine() -> void override
    {
        _currentPosition = _offset;
        _maxRunAscent = 0;
        _maxRunDescent = 0;
        _maxRunLeading = 0;
    }

    /** Called once for each run in a line. Can compute baselines and offsets. */
    auto runInfo(const RunInfo& info) -> void override
    {
        auto metrics = SkFontMetrics();
        info.fFont.getMetrics(&metrics);
        _maxRunAscent = std::min(_maxRunAscent, metrics.fAscent);
        _maxRunDescent = std::max(_maxRunDescent, metrics.fDescent);
        _maxRunLeading = std::max(_maxRunLeading, metrics.fLeading);
    }

    /** Called after all runInfo calls for a line. */
    auto commitRunInfo() -> void override
    {
        _currentPosition.fY -= _maxRunAscent;
    }

    /** Called for each run in a line after commitRunInfo. The buffer will be filled out. */
    auto runBuffer(const RunInfo& info) -> Buffer override
    {
        auto const glyphCount = SkTFitsIn<int>(info.glyphCount) ? static_cast<int>(info.glyphCount) : INT_MAX;
        auto const utf8RangeSize = SkTFitsIn<int>(info.utf8Range.size()) ? static_cast<int>(info.utf8Range.size()) : INT_MAX;

        auto const utf8Data = _text.GetView().GetData();

        auto const& runBuffer = _builder.allocRunTextPos(info.fFont, glyphCount, utf8RangeSize);
        if (runBuffer.utf8text && utf8Data)
        {
            std::memcpy(runBuffer.utf8text, static_cast<char8_t const*>(utf8Data) + info.utf8Range.begin(), utf8RangeSize);
        }
        _clusters = runBuffer.clusters;
        _points = runBuffer.points();
        _glyphCount = glyphCount;
        _clusterOffset = static_cast<int>(info.utf8Range.begin());

        return {runBuffer.glyphs, runBuffer.points(), nullptr, runBuffer.clusters, _currentPosition};
    }

    /** Called after each runBuffer is filled out. */
    auto commitRunBuffer(const RunInfo& info) -> void override
    {
        SkASSERT(0 <= _clusterOffset);
        for (int i = 0; i < _glyphCount; ++i)
        {
            SkASSERT(_clusters[i] >= (unsigned)_clusterOffset);
            _clusters[i] -= _clusterOffset;
            _glyphInfo.push_back({.codePoint = _clusters[i], .position = Point<Dp>(_points[i].fX, _points[i].fY)});
        }
        _currentPosition += info.fAdvance;
    }

    /** Called when ending a line. */
    auto commitLine() -> void override
    {
        _offset += {0, _maxRunDescent + _maxRunLeading - _maxRunAscent};
        _minAdvance = std::min(_minAdvance, _currentPosition.fX);
        _maxAdvance = std::max(_maxAdvance, _currentPosition.fX);
    }

    auto Finalize() &&
    {
        auto layoutInfo = Shared<Graphics::TextLayoutInfo>::Make();
        layoutInfo->SetSize(Size<Dp>(_maxAdvance - _minAdvance, _offset.fY));
        layoutInfo->SetGlyphInfo(std::move(_glyphInfo));
        return std::make_tuple(_builder.make(), layoutInfo);
    }
};
}

///
/// @brief Shape text.
///
auto SkiaTextShaper::Shape(String const& text, Shared<Typeface> const& typeface, FontSize const size, Dp const maxWidth) -> Shared<ShapedText>
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

    auto const [textBlob, layoutInfo] = std::move(runHandler).Finalize();
    return Shared<SkiaShapedText>::Make(textBlob/*, layoutInfo*/);
}
}
