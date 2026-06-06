// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.Path.hpp"

#include "Futurewalker.Core.InputStream.hpp"

#include <fstream>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class FileInputStream : public InputStream
{
public:
    explicit FileInputStream(Path const& path);

    auto IsOpen() const -> Bool;
    auto SetPosition(SInt64 const position, SeekPosition const origin) -> Optional<SInt64> override;
    auto GetPosition() -> Optional<SInt64> override;
    auto Read(std::span<std::byte> buffer) -> Optional<SInt64> override;

private:
    std::ifstream _stream;
};
}
}
