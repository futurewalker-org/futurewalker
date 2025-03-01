// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Prelude.hpp"
#include "Futurewalker.Core.Concepts.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <Concepts::Integral T, class Tag = void>
class Integer;

using UInt8 = Integer<uint8_t>;
using UInt16 = Integer<uint16_t>;
using UInt32 = Integer<uint32_t>;
using UInt64 = Integer<uint64_t>;

using SInt8 = Integer<int8_t>;
using SInt16 = Integer<int16_t>;
using SInt32 = Integer<int32_t>;
using SInt64 = Integer<int64_t>;

using UIntPtr = Integer<uintptr_t>;
using SIntPtr = Integer<intptr_t>;
}
}