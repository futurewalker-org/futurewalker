// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.SeekPosition.hpp"
#include "Futurewalker.Core.Optional.hpp"

#include <span>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class InputStream
{
public:
    virtual ~InputStream() = 0;

    ///
    /// @brief Set current reading position of the stream.
    ///
    /// @param[in] position New position of the stream.
    /// @param[in] origin Origin of position movement.
    ///
    /// @return New position or null if error.
    ///
    virtual auto SetPosition(SInt64 const position, SeekPosition const origin) -> Optional<SInt64> = 0;

    ///
    /// @brief Get current reading position of the stream.
    ///
    /// @return Current reading position or null if error.
    ///
    virtual auto GetPosition() -> Optional<SInt64> = 0;

    ///
    /// @brief Read data from the stream by moving current reading position until given buffer is filled.
    ///
    /// @param[out] buffer Buffer.
    ///
    /// @return Amount of data read or null if the stream cannot be read anymore.
    ///
    /// @note This function blocks until either buffer becomes full or it can no longer read data.
    ///
    virtual auto Read(std::span<uint8_t> buffer) -> Optional<SInt64> = 0;
};
}
}
