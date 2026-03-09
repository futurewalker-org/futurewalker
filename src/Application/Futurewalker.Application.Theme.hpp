// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ThemeType.hpp"

#include "Futurewalker.Attribute.AttributeNodeType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Base class of themes.
///
class Theme : NonCopyable
{
public:
    virtual ~Theme() = 0;

    ///
    /// @brief Apply the theme.
    ///
    /// @param target The target attribute node to apply attributes.
    ///
    virtual auto Apply(AttributeNode& target) -> void = 0;
};
}
}
