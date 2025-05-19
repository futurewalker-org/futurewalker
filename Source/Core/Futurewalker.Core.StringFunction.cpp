// SPDX-License-Identifier: MIT
#include "Futurewalker.Core.StringFunction.hpp"
#include "Futurewalker.Core.Exception.hpp"

#include <boost/locale.hpp>
#include <boost/functional/overloaded_function.hpp>

#include <fmt/format.h>
#include <fmt/xchar.h>
#include <fmt/args.h>

#include <unicode/utf8.h>

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
/// @brief Create String from ASCII C string with runtime checks.
///
/// Copies content of incoming C string into new String object.
/// Any invalid code points found in incoming string will be ignored (skipped).
///
/// @param[in] sv A view of ASCII string.
///
/// @return Constructed String.
///
auto StringFunction::ConvertASCIIToString(std::string_view const sv) -> String
{
    auto result = String();
    result.Reserve(String::SizeType(sv.size()));
    for (const auto& c : sv)
    {
        if (0 <= c && c <= 0x7F)
        {
            result.Insert(result.GetView().GetSize(), String(String::ValueType(c)));
        }
    }
    return result;
}

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
auto StringFunction::ConvertUtf8ToString(std::string_view const sv) -> String
{
    auto const utf8Str = boost::locale::conv::utf_to_utf<char8_t>(sv.data(), sv.data() + sv.length(), boost::locale::conv::method_type::skip);
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
auto StringFunction::ConvertUtf8ToStringUnchecked(std::string_view const sv) -> String
{
    // `String` uses `memcpy` to copy buffer, so we can use `reinterpret_cast` without UB here.
    // NOTE: This is implementation detail. Users should not rely on this behavior.
    return String(reinterpret_cast<const char8_t*>(sv.data()), SInt64(sv.length()));
}

///
/// @brief Convert String to std::string.
///
/// @param[in] sv UTF-8 string view
///
/// @note No transcoding will be performed.
/// @note Resulting string will be in UTF-8 regardless of current encoding of char.
///
auto StringFunction::ConvertStringToStdString(StringView const sv) -> std::string
{
    auto const data = reinterpret_cast<const char*>(static_cast<const char8_t*>(sv.GetData()));
    auto const size = static_cast<size_t>(sv.GetSize());
    return std::string(data, size);
}

///
/// @brief Convert String to std::u8string.
///
/// @param[in] sv UTF-8 string view
///
/// @note No transcoding will be performed.
///
auto StringFunction::ConvertStringToStdU8String(StringView const sv) -> std::u8string
{
    auto const data = static_cast<const char8_t*>(sv.GetData());
    auto const size = static_cast<size_t>(sv.GetSize());
    return std::u8string(data, size);
}

///
/// @brief Validate UTF-8 string.
///
/// @param[out] result Source string with invalid code points removed or replaced.
/// @param[in] source Source string
///
/// @return Returns true when source and result is identical.
///
/// @note Invalid code points will be either removed or replaced by Unicode replacement character U+FFFD.
///
auto StringFunction::ValidateString(String& result, String const& source) -> Bool
{
    auto const view = source.GetView();
    auto const data = view.GetData();

    auto const str = reinterpret_cast<const uint8_t*>(static_cast<const String::ValueType*>(data));
    auto const length = static_cast<int32_t>(view.GetSize());

    auto buffer = std::vector<uint8_t>();
    buffer.reserve(length);

    auto r = true;
    auto i = int32_t();
    while (i < length)
    {
        auto c = UChar32();
        auto next = i;
        U8_NEXT(str, next, length, c);
        if (0 <= c)
        {
            assert(next - i <= 4);
            buffer.append_range(std::ranges::subrange(&str[i], &str[next]));
        }
        else
        {
            r = false;
            c = 0xfffd;
            auto j = int32_t();
            auto tmp = std::array<uint8_t, 4>();
            U8_APPEND_UNSAFE(tmp.data(), j, c);
            buffer.append_range(std::ranges::subrange(&tmp[0], &tmp[j]));
        }
        i = next;
    }

    result = StringFunction::ConvertUtf8ToStringUnchecked({reinterpret_cast<const char*>(buffer.data()), buffer.size()});

    if (r)
    {
        assert(result == source);
        return true;
    }
    return false;
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
