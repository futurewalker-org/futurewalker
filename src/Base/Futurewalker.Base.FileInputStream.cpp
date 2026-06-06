// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.FileInputStream.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace
{
auto ToSeekdir(SeekPosition const dir) -> std::ios_base::seekdir
{
    switch (dir)
    {
        case SeekPosition::Begin:
            return std::ios_base::beg;
        case SeekPosition::Current:
            return std::ios_base::cur;
        case SeekPosition::End:
            return std::ios_base::end;
        default: {
            FW_DEBUG_ASSERT(false);
            return std::ios_base::beg;
        }
    }
}
}

///
/// @brief Open file stream.
///
/// @param path[in] Path to the file.
///
FileInputStream::FileInputStream(Path const& path)
{
    _stream.open(path.AsStdFilesystemPath(), std::ios_base::in | std::ios_base::binary);
}

///
/// @brief Check if a file is opened by this stream.
///
auto FileInputStream::IsOpen() const -> Bool
{
    return _stream.is_open();
}

///
/// @brief Set current reading position of the stream.
///
/// @param[in] position New position of the stream.
/// @param[in] direction How to interpret `position` argument.
///
/// @return New position or null if error.
///
auto FileInputStream::SetPosition(SInt64 const position, SeekPosition const origin) -> Optional<SInt64>
{
    if (!_stream.bad())
    {
        _stream.setstate(_stream.rdstate() & ~std::ios::failbit);
        _stream.seekg(static_cast<int64_t>(position), ToSeekdir(origin));
        if (!_stream.fail())
        {
            auto const pos = _stream.tellg();
            if (pos != std::ifstream::pos_type(-1))
            {
                return pos;
            }
        }
    }
    return {};
}

///
/// @brief Get current reading position of the stream.
///
/// @return Current reading position or null if error.
///
auto FileInputStream::GetPosition() -> Optional<SInt64>
{
    if (!_stream.bad())
    {
        _stream.setstate(_stream.rdstate() & ~std::ios::failbit);
        auto const pos = _stream.tellg();
        if (!_stream.fail())
        {
            if (pos != std::ifstream::pos_type(-1))
            {
                return pos;
            }
        }
    }
    return {};
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
auto FileInputStream::Read(std::span<std::byte> buffer) -> Optional<SInt64>
{
    if (!_stream.bad() && !_stream.eof())
    {
        _stream.setstate(_stream.rdstate() & ~std::ios::failbit);
        _stream.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
        // read() sets failbit | eofbit when EOF is reached.
        if (!_stream.fail() || _stream.eof())
        {
            return _stream.gcount();
        }
    }
    return {};
}
}
