// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.MemoryInputStreamType.hpp"
#include "Futurewalker.Core.InputStream.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.SharedArray.hpp"
#include "Futurewalker.Core.PassKey.hpp"

#include <span>
#include <variant>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief InputStream backed by owned or non-owned memory buffer.
///
class MemoryInputStream : public InputStream
{
public:
    static auto Make(std::span<const std::byte> const data, Bool const copy) -> Unique<MemoryInputStream>;

public:
    MemoryInputStream(PassKey<MemoryInputStream>, std::span<const std::byte> const data, Bool const copy);
    MemoryInputStream(PassKey<MemoryInputStream>, std::variant<SharedArray<std::byte>, std::span<const std::byte>> const& data);

    auto SetPosition(SInt64 const position, SeekPosition const origin) -> Optional<SInt64> override;
    auto GetPosition() -> Optional<SInt64> override;
    auto Read(std::span<std::byte> buffer) -> Optional<SInt64> override;

    auto Clone() const -> Unique<MemoryInputStream>;

private:
    auto InternalGetData() const -> Pointer<const std::byte>;
    auto InternalGetSize() const -> SInt64;

private:
    std::variant<SharedArray<std::byte>, std::span<const std::byte>> _data;
    SInt64 _position = 0;
};
}
}
