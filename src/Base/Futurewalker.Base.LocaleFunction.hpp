// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.LocaleType.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.Optional.hpp"

#include <span>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Locale functions.
///
struct LocaleFunction : NonConstructible
{
public:
    static auto Match(std::span<Locale const> availableLocales, std::span<Locale const> preferredLocales) -> Optional<Locale>;
};
}
}
