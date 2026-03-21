// SPDX-License-Identifier: MPL-2.0
#include "Futurewalker.Core.StringFunction.hpp"
#include "Futurewalker.Core.UnicodeFunction.hpp"
#include "Futurewalker.Core.Exception.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>
#include <fmt/args.h>

template <class T, class Tag>
struct fmt::formatter<FW_NS::Integer<T, Tag>, char8_t> : fmt::formatter<T, char8_t>
{
    template <class FormatContext>
    auto format(FW_NS::Integer<T, Tag> const& a, FormatContext& ctx) const
    {
        return fmt::formatter<T, char8_t>::format(static_cast<T>(a), ctx);
    }
};

template <class T, class Tag>
struct fmt::formatter<FW_NS::Float<T, Tag>, char8_t> : fmt::formatter<T, char8_t>
{
    template <class FormatContext>
    auto format(FW_NS::Float<T, Tag> const& a, FormatContext& ctx) const
    {
        return fmt::formatter<T, char8_t>::format(static_cast<T>(a), ctx);
    }
};

template <class Tag>
struct fmt::formatter<FW_NS::Boolean<Tag>, char8_t> : fmt::formatter<bool, char8_t>
{
    template <class FormatContext>
    auto format(FW_NS::Boolean<Tag> const& a, FormatContext& ctx) const
    {
        return fmt::formatter<bool, char8_t>::format(static_cast<bool>(a), ctx);
    }
};

template <>
struct fmt::formatter<FW_NS::StringView, char8_t> : fmt::formatter<std::u8string_view, char8_t>
{
    template <class FormatContext>
    auto format(FW_NS::StringView const& a, FormatContext& ctx) const
    {
        auto const str = std::basic_string_view(static_cast<const char8_t*>(a.GetData()), static_cast<size_t>(a.GetSize()));
        return fmt::formatter<std::u8string_view, char8_t>::format(str, ctx);
    }
};

namespace FW_DETAIL_NS
{
///
/// @brief Join strings with separator.
///
/// @param[in] separator Separator string.
/// @param[in] parts Parts to join.
///
/// @return Joined string.
///
auto StringFunction::Join(String const separator, std::span<String const> const parts) -> String
{
    auto result = String();
    for (auto i = SInt64(0); i < std::ssize(parts); ++i)
    {
        if (i > 0)
        {
            result.Append(separator);
        }
        result.Append(parts[static_cast<size_t>(i)]);
    }
    return result;
}

///
/// @brief Trim end of text range that consists of break and space characters, and return substring of the trimmed range.
///
/// @param[in] text The text to trim.
/// @param[in] begin The index of the beginning of the text range to trim.
/// @param[in] end The index of the end of the text range to trim.
///
/// @return The substring of the trimmed text range.
///
auto StringFunction::StripTrailingBreakAndSpace(String const& text, String::IndexType const begin, String::IndexType const end) -> String
{
    auto idx = end;
    while (begin < idx)
    {
        auto prevIdx = text.GetPrev(idx);
        auto c = String::CharType();
        if (text.GetChar(prevIdx, c))
        {
            if (!UnicodeFunction::IsSpace(c) && !UnicodeFunction::IsBreak(c))
            {
                return text.GetSubString(begin, idx);
            }
        }
        idx = prevIdx;
    }
    return String();
}

///
/// @brief Format string with parameters.
///
/// @param[in] format Formatter string.
/// @param[in] params Array of format parameters.
///
/// @return result Result of formatting.
///
/// @throw Exception on format error.
///
auto StringFunction::FormatImpl(StringView const formatter, std::span<FormatParameter const> const& params) -> String
{
    using FormatContext = fmt::buffered_context<char8_t>;

    fmt::dynamic_format_arg_store<FormatContext> args;
    args.reserve(params.size(), 0);

    for (auto const& param : params)
    {
        std::visit([&](auto const& v) { args.push_back(v); }, param);
    }

    try
    {
        auto const data = static_cast<char8_t const*>(formatter.GetData());
        auto const size = static_cast<size_t>(formatter.GetSize());
        auto const str = fmt::vformat<char8_t>(std::basic_string_view(data, size), args);
        return String(StringView(str.data(), StringView::SizeType(str.length())));
    }
    catch (fmt::format_error&)
    {
        throw Exception(ErrorCode::InvalidArgument, "Invalid format arguments");
    }
}
}
