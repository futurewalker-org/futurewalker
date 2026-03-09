// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Text.LineBreakIterator.hpp"

#include "Futurewalker.Core.Optional.hpp"

#include <unicode/brkiter.h>

#include <memory>
#include <cassert>

namespace FW_DETAIL_NS
{
class LineBreakIterator::Impl
{
public:
    explicit Impl(std::string_view const bcp47LanguageTag)
    {
        auto err = U_ZERO_ERROR;
        auto const locale = icu::Locale::forLanguageTag(bcp47LanguageTag, err);
        _breakIterator.reset(icu::BreakIterator::createLineInstance(locale, err));
    }

    auto SetText(String const& text) -> void
    {
        _text = text;

        if (_breakIterator)
        {
            auto const utf8Text = static_cast<char8_t const*>(_text.GetView().GetData());
            auto const utf8TextLength = static_cast<int>(_text.GetView().GetSize());
            auto err = U_ZERO_ERROR;
            auto ut = UText(UTEXT_INITIALIZER);
            utext_openUTF8(&ut, reinterpret_cast<char const*>(utf8Text), utf8TextLength, &err);
            _breakIterator->setText(&ut, err);
            utext_close(&ut);
        }
    }

    auto GetNext() -> Optional<String::IndexType>
    {
        if (_breakIterator)
        {
            auto const next = _breakIterator->next();
            if (next != icu::BreakIterator::DONE)
            {
                return CodeUnit(next);
            }
        }
        return {};
    }

    auto GetPrev() -> Optional<String::IndexType>
    {
        if (_breakIterator)
        {
            auto const prev = _breakIterator->previous();
            if (prev != icu::BreakIterator::DONE)
            {
                return CodeUnit(prev);
            }
        }
        return {};
    }

    auto GetCurrent() -> String::IndexType
    {
        if (_breakIterator)
        {
            return CodeUnit(_breakIterator->current());
        }
        return 0;
    }

    auto GetCurrentBreakCategory() -> BreakCategory
    {
        if (_breakIterator)
        {
            auto const ruleStatus = _breakIterator->getRuleStatus();
            if (ruleStatus < UBRK_LINE_SOFT_LIMIT)
            {
                return BreakCategory::Soft;
            }
            else if (ruleStatus < UBRK_LINE_HARD_LIMIT)
            {
                return BreakCategory::Hard;
            }
        }
        assert(false);
        return BreakCategory::Hard;
    }

private:
    String _text;
    std::unique_ptr<icu::BreakIterator> _breakIterator;
};

///
/// @brief Constructor.
///
/// @param bcp47LanguageTag BCP47 language tag of locale to use.
///
LineBreakIterator::LineBreakIterator(std::string_view const bcp47LanguageTag)
  : _impl {std::make_unique<Impl>(bcp47LanguageTag)}
{
}

///
/// @brief Destructor.
///
LineBreakIterator::~LineBreakIterator() noexcept = default;

///
/// @brief Set text to analyze.
///
/// @param text The text.
///
auto LineBreakIterator::SetText(String const& text) -> void
{
    _impl->SetText(text);
}

///
/// @brief Get next boundary index.
///
auto LineBreakIterator::GetNext() -> Optional<String::IndexType>
{
    return _impl->GetNext();
}

///
/// @brief Get previous boundary index.
///
auto LineBreakIterator::GetPrev() -> Optional<String::IndexType>
{
    return _impl->GetPrev();
}

///
/// @brief Get current boundary index.
///
auto LineBreakIterator::GetCurrent() const -> String::IndexType
{
    return _impl->GetCurrent();
}

///
/// @brief Get current break category.
///
auto LineBreakIterator::GetCurrentBreakCategory() const -> BreakCategory
{
    return _impl->GetCurrentBreakCategory();
}
}
