// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.TextLayoutInfo.hpp"
#include "Futurewalker.Graphics.GlyphRun.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief Add glyhp run to layout.
///
/// @param[in] position Position of glyph run.
/// @param[in] glyphRun Glyph run.
///
auto TextLayoutInfo::AddGlyphRun(Point<Dp> const& position, Shared<GlyphRun> const& glyphRun) -> void
{
    if (glyphRun)
    {
        _glyphRuns.push_back(glyphRun);
        _glyphRunPositions.push_back(position);
        _glyphRunCount += 1;
        _glyphCount += glyphRun->GetGlyphCount();
        _textSize += glyphRun->GetText().GetCodePointCount();

        auto const metrics = glyphRun->GetMetrics();
        auto const width = glyphRun->GetAdvance();
        auto const height = metrics.ascent + metrics.descent + metrics.leading;
        _glyphRunBounds = Rect<Dp>::Union(_glyphRunBounds, {position, {width, height}});
    }
}

///
/// @brief Get number of glyph runs.
///
auto TextLayoutInfo::GetGlyphRunCount() const -> SInt64
{
    return _glyphRunCount;
}

///
/// @brief Get glyph run at specified index.
///
/// @param[in] glyphRunIndex Index of glyph run.
///
auto TextLayoutInfo::GetGlyphRun(SInt64 const glyphRunIndex) const -> Shared<GlyphRun>
{
    if (0 <= glyphRunIndex && glyphRunIndex < _glyphRunCount)
    {
        return _glyphRuns[static_cast<size_t>(glyphRunIndex)];
    }
    return nullptr;
}

///
/// @brief Get position of glyph run.
///
/// @param[in] glyphRunIndex Index of glyph run.
///
auto TextLayoutInfo::GetGlyphRunPosition(SInt64 const glyphRunIndex) const -> Optional<Point<Dp>>
{
    if (0 <= glyphRunIndex && glyphRunIndex < _glyphRunCount)
    {
        return _glyphRunPositions[static_cast<size_t>(glyphRunIndex)];
    }
    return {};
}

///
/// @brief Get character index of glyph run.
///
/// @param[in] glyphRunIndex Index of glyph run.
///
auto TextLayoutInfo::GetGlyphRunCharacterIndex(SInt64 const glyphRunIndex) const -> Optional<CodePoint>
{
    if (0 <= glyphRunIndex && glyphRunIndex < _glyphRunCount)
    {
        auto characterIndex = CodePoint(0);
        for (auto i = SInt64(0); i < glyphRunIndex; ++i)
        {
            characterIndex += _glyphRuns[static_cast<size_t>(i)]->GetText().GetCodePointCount();
        }
        return characterIndex;
    }
    return {};
}

///
/// @brief Get glyph run index from character index.
///
/// @param[in] characterIndex Character index.
///
auto TextLayoutInfo::GetGlyphRunIndex(CodePoint const characterIndex) const -> Optional<SInt64>
{
    if (characterIndex < 0)
    {
        return {};
    }

    auto localGlyphCount = GlyphIndex(0);
    auto localPosition = characterIndex;
    for (auto i = SInt64(0); i < GetGlyphRunCount(); ++i)
    {
        auto const glyphRun = GetGlyphRun(i);
        auto const runTextSize = glyphRun->GetText().GetCodePointCount();
        if (characterIndex < runTextSize)
        {
            return i;
        }
        localPosition -= runTextSize;
        localGlyphCount += glyphRun->GetGlyphCount();
        if (localPosition < 0)
        {
            break;
        }
    }
    return {};
}

///
/// @brief Get number of glyphs.
///
auto TextLayoutInfo::GetGlyphCount() const -> GlyphIndex
{
    return _glyphCount;
}

///
/// @brief Get position of glyph.
///
/// @param[in] glyphIndex Index of glyph.
///
auto TextLayoutInfo::GetGlyphPosition(GlyphIndex const glyphIndex) const -> Optional<Point<Dp>>
{
    if (auto [glyphRun, localGlyphIndex] = FindRunByGlyphIndex(glyphIndex); glyphRun)
    {
        return glyphRun->GetGlyphPosition(localGlyphIndex);
    }
    return {};
}

///
/// @brief Get character index of glyph.
///
/// @param[in] glyphIndex Index of glyph.
///
auto TextLayoutInfo::GetCharacterIndex(GlyphIndex const glyphIndex) const -> Optional<CodePoint>
{
    if (auto [glyphRun, localGlyphIndex] = FindRunByGlyphIndex(glyphIndex); glyphRun)
    {
        return glyphRun->GetCharacterIndex(localGlyphIndex);
    }
    return {};
}

///
/// @brief Get glyph index from character index.
///
/// @param[in] characterIndex Character index.
///
auto TextLayoutInfo::GetGlyphIndex(CodePoint const characterIndex) const -> GlyphIndex
{
    if (characterIndex < 0)
    {
        return 0;
    }

    if (characterIndex < _textSize)
    {
        auto localGlyphCount = GlyphIndex(0);
        auto localPosition = characterIndex;
        for (auto i = SInt64(0); i < GetGlyphRunCount(); ++i)
        {
            auto const glyphRun = GetGlyphRun(i);
            auto const runTextSize = glyphRun->GetText().GetCodePointCount();
            if (characterIndex < runTextSize)
            {
                auto const glyphIndex = glyphRun->GetGlyphIndex(localPosition);
                return glyphIndex.GetValueOr(0) + localGlyphCount;
            }
            localPosition -= runTextSize;
            localGlyphCount += glyphRun->GetGlyphCount();
            if (localPosition < 0)
            {
                return 0;
            }
        }
    }
    return GetGlyphCount() - 1;
}

///
/// @brief Get enclosing bounds of glyphs.
///
auto TextLayoutInfo::GetBoundsRect() const -> Rect<Dp>
{
    return _glyphRunBounds;
}

auto TextLayoutInfo::FindRunByGlyphIndex(GlyphIndex const glyphIndex) const -> std::pair<Shared<GlyphRun>, GlyphIndex>
{
    if (0 <= glyphIndex && glyphIndex < GetGlyphCount())
    {
        auto localIndex = glyphIndex;
        for (auto i = SInt64(0); i < GetGlyphRunCount(); ++i)
        {
            auto const glyphRun = GetGlyphRun(i);
            auto const runGlyphCount = glyphRun->GetGlyphCount();
            if (localIndex < runGlyphCount)
            {
                return {glyphRun, localIndex};
            }
            localIndex -= runGlyphCount;
            if (localIndex < 0)
            {
                return {nullptr, 0};
            }
        }
    }
    return {nullptr, 0};
}

auto TextLayoutInfo::FindRunByCharacterIndex(CodePoint const characterIndex) const -> std::pair<Shared<GlyphRun>, CodePoint>
{
    if (0 <= characterIndex && characterIndex < _textSize)
    {
        auto localIndex = characterIndex;
        for (auto i = SInt64(0); i < GetGlyphRunCount(); ++i)
        {
            auto const glyphRun = GetGlyphRun(i);
            auto const runTextSize = glyphRun->GetText().GetCodePointCount();
            if (localIndex < runTextSize)
            {
                return {glyphRun, localIndex};
            }
            localIndex -= runTextSize;
            if (localIndex < 0)
            {
                return {nullptr, 0};
            }
        }
    }
    return {nullptr, 0};
}
}
