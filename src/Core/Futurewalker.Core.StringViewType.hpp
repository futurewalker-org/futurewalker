// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class Char>
class StringViewT;

using StringView = StringViewT<char8_t>;
using U16StringView = StringViewT<char16_t>;
}
}