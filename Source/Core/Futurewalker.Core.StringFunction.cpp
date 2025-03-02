// SPDX-License-Identifier: MIT
#include "Futurewalker.Core.StringFunction.hpp"
#include "Futurewalker.Core.Exception.hpp"

#include <boost/locale.hpp>
#include <boost/functional/overloaded_function.hpp>

#include <fmt/format.h>
#include <fmt/xchar.h>
#include <fmt/args.h>

template <class T, class Tag>
struct fmt::formatter<FW_NS::Integer<T, Tag>, char8_t> : fmt::formatter<T, char8_t>
{
    template <class FormatContext>
    auto format(const FW_NS::Integer<T, Tag>& a, FormatContext& ctx) const
    {
        return fmt::formatter<T, char8_t>::format(static_cast<T>(a), ctx);
    }
};

template <class T, class Tag>
struct fmt::formatter<FW_NS::Float<T, Tag>, char8_t> : fmt::formatter<T, char8_t>
{
    template <class FormatContext>
    auto format(const FW_NS::Float<T, Tag>& a, FormatContext& ctx) const
    {
        return fmt::formatter<T, char8_t>::format(static_cast<T>(a), ctx);
    }
};

template <class Tag>
struct fmt::formatter<FW_NS::Boolean<Tag>, char8_t> : fmt::formatter<bool, char8_t>
{
    template <class FormatContext>
    auto format(const FW_NS::Boolean<Tag>& a, FormatContext& ctx) const
    {
        return fmt::formatter<bool, char8_t>::format(static_cast<bool>(a), ctx);
    }
};

template <>
struct fmt::formatter<FW_NS::StringView, char8_t> : fmt::formatter<std::u8string_view, char8_t>
{
    template <class FormatContext>
    auto format(const FW_NS::StringView& a, FormatContext& ctx) const
    {
        const auto str = std::basic_string_view(static_cast<const char8_t*>(a.GetData()), static_cast<size_t>(a.GetSize()));
        return fmt::formatter<std::u8string_view, char8_t>::format(str, ctx);
    }
};

namespace FW_DETAIL_NS
{
///
/// @brief Create String from UTF-8 C string with runtime checks.
///
/// Copies content of incoming C string into new String object.
/// Any invalid code points found in incoming string will be ignored (skipped).
///
/// @note To avoid runtime checks at the cost of safety, use Utf8ToStringUnchecked().
///
/// @param[in] sv A view of UTF-8 string.
///
/// @return Constructed String.
///
auto StringFunction::ConvertUtf8ToString(std::string_view sv) -> String
{
    const auto utf8Str = boost::locale::conv::utf_to_utf<char8_t>(sv.data(), sv.data() + sv.length(), boost::locale::conv::method_type::skip);
    return String(utf8Str.data(), SInt64(utf8Str.length()));
}

///
/// @brief Create String from UTF-8 C string without runtime checks.
///
/// Copies content of incoming C string into new String object without any runtime checks.
///
/// @note For safer version, see Utf8ToString().
///
/// @param[in] sv A view of UTF-8 string.
///
/// @return Constructed String.
///
auto StringFunction::ConvertUtf8ToStringUnchecked(std::string_view sv) -> String
{
    // `String` uses `memcpy` to copy buffer, so we can use `reinterpret_cast` without UB here.
    // NOTE: This is implementation detail. Users should not rely on this behavior.
    return String(reinterpret_cast<const char8_t*>(sv.data()), SInt64(sv.length()));
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
auto StringFunction::FormatImpl(const StringView& formatter, const std::span<const FormatParameter>& params) -> String
{
    using FormatContext = fmt::buffered_context<char8_t>;

    fmt::dynamic_format_arg_store<FormatContext> args;
    args.reserve(params.size(), 0);

    for (const auto& param : params)
    {
        std::visit([&](const auto& v) { args.push_back(v); }, param);
    }

    try
    {
        const auto data = static_cast<const char8_t*>(formatter.GetData());
        const auto size = static_cast<size_t>(formatter.GetSize());
        const auto str = fmt::vformat<char8_t>(std::basic_string_view(data, size), args);
        return String(StringView(str.data(), StringView::SizeType(str.length())));
    }
    catch (fmt::format_error&)
    {
        throw Exception(ErrorCode::InvalidArgument, "Invalid format arguments");
    }
}
}
