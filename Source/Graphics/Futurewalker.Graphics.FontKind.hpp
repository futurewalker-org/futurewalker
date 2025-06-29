// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Prelude.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"

#include "Futurewalker.Core.Identifier.hpp"
#include "Futurewalker.Core.Primitive.hpp" 

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Identifier of named font kind.
///
using FontKind = Identifier<UInt64, class IdTagFontKind>;

///
/// @brief Identifier of named font kind.
///
using UniqueFontKind = UniqueIdentifier<class IdTagFontKind>;

///
/// @brief Named identifier of generic sans serif font.
///
extern UniqueFontKind FontKindGenericSerif;

///
/// @brief Named identifier of generic sans-serif font.
///
extern UniqueFontKind FontKindGenericSansSerif;

///
/// @brief Named identifier of generic monospace font.
///
extern UniqueFontKind FontKindGenericMonospace;
}
}
