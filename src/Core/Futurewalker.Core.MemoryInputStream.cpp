// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Core.MemoryInputStream.hpp"

#include "Futurewalker.Core.Overloaded.hpp"
#include "Futurewalker.Core.DataFunction.hpp"

#include <algorithm>

namespace FW_DETAIL_NS
{
namespace
{
auto MakeInternalData(std::span<const std::byte> const data, Bool const copy) -> std::variant<SharedArray<std::byte>, std::span<const std::byte>>
{
    if (copy)
    {
        auto buffer = SharedArray<std::byte>();
        buffer.Reserve(std::ssize(data));
        for (auto const& byte : data)
        {
            buffer.PushBack(byte);
        }
        return std::move(buffer);
    }
    else
    {
        return data;
    }
}
}

///
/// @brief Create new MemoryInputStream object.
///
/// @param[in] data Memory buffer.
/// @param[in] copy Whether to copy the data or not.
///
/// @note If copy is false, the caller must ensure that the data remains valid for the lifetime of the MemoryInputStream object.
///
auto MemoryInputStream::Make(std::span<const std::byte> const data, Bool const copy) -> Unique<MemoryInputStream>
{
    return Unique<MemoryInputStream>::Make(PassKey<MemoryInputStream>(), data, copy);
}

///
/// @brief Constructor.
///
MemoryInputStream::MemoryInputStream(PassKey<MemoryInputStream>, std::span<const std::byte> const data, Bool const copy)
  : _data {MakeInternalData(data, copy)}
{
}

///
/// @brief Constructor.
///
MemoryInputStream::MemoryInputStream(PassKey<MemoryInputStream>, std::variant<SharedArray<std::byte>, std::span<const std::byte>> const& data)
  : _data {data}
{
}

///
/// @brief Set current reading position of the stream.
///
/// @param[in] position New position of the stream.
/// @param[in] origin Origin of position movement.
///
/// @return New position or null if error.
///
auto MemoryInputStream::SetPosition(SInt64 const position, SeekPosition const origin) -> Optional<SInt64>
{
    SInt64 base = 0;
    switch (origin)
    {
        case SeekPosition::Begin:
            base = 0;
            break;
        case SeekPosition::Current:
            base = _position;
            break;
        case SeekPosition::End:
            base = InternalGetSize();
            break;
        default:
            return {};
    }

    auto const newPosition = base + position;
    if (newPosition < 0)
    {
        return {};
    }

    _position = newPosition;
    return _position;
}

///
/// @brief Get current reading position of the stream.
///
/// @return Current reading position or null if error.
///
auto MemoryInputStream::GetPosition() -> Optional<SInt64>
{
    return _position;
}

///
/// @brief Read data from the stream by moving current reading position until given buffer is filled.
///
/// @param[out] buffer Buffer.
///
/// @return Amount of data read or null if the stream cannot be read anymore.
///
/// @note This function blocks until either buffer becomes full or it can no longer read data.
///
auto MemoryInputStream::Read(std::span<std::byte> buffer) -> Optional<SInt64>
{
    auto const size = InternalGetSize();
    if (_position < 0 || _position > size)
    {
        return {};
    }

    auto const available = size - _position;
    auto const toRead = std::min(available, SInt64(std::ssize(buffer)));
    if (toRead > 0)
    {
        DataFunction::MemoryCopy(Pointer(buffer.data()), InternalGetData() + _position, toRead);
        _position += toRead;
    }
    return toRead;
}

///
/// @brief Clone the MemoryInputStream object.
///
/// @note The cloned object shares the same underlying memory buffer and has its own independent reading position.
///
auto MemoryInputStream::Clone() const -> Unique<MemoryInputStream>
{
    return Unique<MemoryInputStream>::Make(PassKey<MemoryInputStream>(), _data);
}

///
/// @brief Get data pointer of the memory buffer.
///
auto MemoryInputStream::InternalGetData() const -> Pointer<const std::byte>
{
    return std::visit(//
      Overloaded {
          [](SharedArray<std::byte> const& buffer) -> Pointer<const std::byte> { return buffer.GetValues().data(); },
          [](std::span<const std::byte> const& span) -> Pointer<const std::byte> { return span.data(); }},
      _data);
}

///
/// @brief Get size of the memory buffer.
///
auto MemoryInputStream::InternalGetSize() const -> SInt64
{
    return std::visit(//
      Overloaded {
          [](SharedArray<std::byte> const& buffer) -> SInt64 { return buffer.GetSize(); },
          [](std::span<const std::byte> const& span) -> SInt64 { return std::ssize(span); }},
      _data);
}
}
