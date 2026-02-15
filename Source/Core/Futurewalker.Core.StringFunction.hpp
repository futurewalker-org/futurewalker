// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"

#include <span>

namespace FW_DETAIL_NS 
{
namespace FW_EXPORT
{
///
/// @brief String utility functions.
///
class StringFunction final : NonConstructible
{
public:
    template <class... Args>
    static auto Format(StringView const formatter, Args&&... args) -> String;

    static auto Join(String const separator, std::span<String const> const parts) -> String;

private:
    // clang-format off
    using FormatParameter = std::variant<uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, float32_t, float64_t, UInt8, UInt16, UInt32, UInt64, SInt8, SInt16, SInt32, SInt64, Float32, Float64, char8_t, const char8_t*, std::basic_string_view<char8_t>, StringView, bool, Bool>;
    // clang-format on
    static String FormatImpl(StringView const format, std::span<FormatParameter const> const& params);
};
}

///
/// @brief Format string based on std::format style syntax.
///
/// @param[in] formatter Formatter string.
/// @param[in] args Format arguments.
///
/// @return Formatted string.
///
/// @note Named arguments are not supported.
/// @note Passing extra parameters are not treated as an error.
///
/// @throws Exception on invalid arguments.
///
template <class... Args>
auto StringFunction::Format(StringView const formatter, Args&&... args) -> String
{
    auto const params = std::array<FormatParameter, sizeof...(Args)> {static_cast<FormatParameter>(std::forward<Args>(args))...};
    return FormatImpl(formatter, params);
}
}

