// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Text.LineBreakIteratorType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Optional.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Iterator for line breaks.
///
class LineBreakIterator : NonCopyable
{
public:
    enum class BreakCategory
    {
        Soft, ///< Non-mandatory break opportunity, e.g., space character.
        Hard, ///< Mandatory break opportunity, e.g., newline character.
    };

    explicit LineBreakIterator(std::string_view const bcp47LanguageTag);
    ~LineBreakIterator() noexcept;

    auto SetText(String const& text) -> void;

    auto GetNext() -> Optional<String::IndexType>;
    auto GetPrev() -> Optional<String::IndexType>;
    auto GetCurrent() const -> String::IndexType;
    auto GetCurrentBreakCategory() const -> BreakCategory;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};
}
}
