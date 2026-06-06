// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Path.hpp"
#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Construct path from UTF-8 String.
///
/// @param[in] string String.
///
auto Path::MakeFromString(String const& string) -> Path
{
    return Path(std::u8string_view(static_cast<char8_t const*>(string.GetView().GetData()), static_cast<size_t>(string.GetView().GetSize())));
}

///
/// @brief Construct Path from native format path string.
///
auto Path::MakeFromNativeString(std::string_view const native) -> Path
{
    return Path(native);
}

///
/// @brief Construct Path from native format path string.
///
auto Path::MakeFromNativeString(std::wstring_view const native) -> Path
{
    return Path(native);
}

///
/// @brief Construct Path from std::filesystem::path.
///
auto Path::MakeFromStdFilesystemPath(std::filesystem::path const& path) -> Path
{
    return Path(path);
}

///
/// @brief Returns UTF-8 encoded path string in platform's native path format.
///
/// @note This conversion is lossy if original path contains non-Unicode characters.
///
auto Path::GetNativePathString() const -> String
{
    return String::MakeFromStdU8String(_path.u8string());
}

///
/// @brief Returns UTF-8 encoded path string in generic path format.
///
/// @note This conversion is lossy if original path contains non-Unicode characters.
///
auto Path::GetGenericPathString() const -> String
{
    return String::MakeFromStdU8String(_path.generic_u8string());
}

auto Path::IsEmpty() const -> Bool
{
    return _path.empty();
}

auto Path::IsAbsolute() const -> Bool
{
    return _path.is_absolute();
}

auto Path::IsRelative() const -> Bool
{
    return _path.is_relative();
}

auto Path::HasRootPath() const -> Bool
{
    return _path.has_root_path();
}

auto Path::HasParentPath() const -> Bool
{
    return _path.has_parent_path();
}

auto Path::HasRelativePath() const -> Bool
{
    return _path.has_relative_path();
}

auto Path::HasFileName() const -> Bool
{
    return _path.has_filename();
}

auto Path::HasExtension() const -> Bool
{
    return _path.has_extension();
}

auto Path::GetRootPath() const -> Path
{
    return Path(_path.root_path());
}

auto Path::GetParentPath() const -> Path
{
    return Path(_path.parent_path());
}

auto Path::GetRelativePath() const -> Path
{
    return Path(_path.relative_path());
}

auto Path::GetFileName() const -> Path
{
    return Path(_path.filename());
}

auto Path::GetExtension() const -> Path
{
    return Path(_path.extension());
}

auto Path::Append(Path const& path) -> void
{
    _path /= path._path;
}

auto Path::Concat(Path const& path) -> void
{
    _path += path._path;
}

auto Path::ReplaceFileName(Path const& path) -> void
{
    _path.replace_filename(path._path);
}

auto Path::ReplaceExtension(Path const& path) -> void
{
    _path.replace_extension(path._path);
}

auto Path::GetLexicallyNormal() const -> Path
{
    return Path(_path.lexically_normal());
}

auto Path::GetLexicallyRelative(Path const& base) const -> Path
{
    return Path(_path.lexically_relative(base._path));
}

auto Path::GetLexicallyProximate(Path const& base) const -> Path
{
    return Path(_path.lexically_proximate(base._path));
}

auto Path::Clear() -> void
{
    _path.clear();
}

auto Path::AsStdFilesystemPath() const -> std::filesystem::path
{
    return _path;
}

Path::Path(std::filesystem::path path)
  : _path {std::move(path)}
{
}
}
