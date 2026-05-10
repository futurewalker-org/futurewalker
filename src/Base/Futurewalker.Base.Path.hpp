// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.PathType.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.U16String.hpp"
#include "Futurewalker.Core.Optional.hpp"

#include <filesystem>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Represents file path as string in platform-specific encoding.
///
class Path
{
public:
    static auto MakeFromString(String const& string) -> Path;
    static auto MakeFromNativeString(std::string_view native) -> Path;
    static auto MakeFromNativeString(std::wstring_view native) -> Path;

    Path() = default;

    auto GetNativePathString() const -> String;
    auto GetGenericPathString() const -> String;

    auto IsEmpty() const -> Bool;

    auto IsAbsolute() const -> Bool;
    auto IsRelative() const -> Bool;

    auto HasRootPath() const -> Bool;
    auto HasParentPath() const -> Bool;
    auto HasRelativePath() const -> Bool;
    auto HasFileName() const -> Bool;
    auto HasExtension() const -> Bool;

    auto GetRootPath() const -> Path;
    auto GetParentPath() const -> Path;
    auto GetRelativePath() const -> Path;

    auto GetFileName() const -> Path;
    auto GetExtension() const -> Path;

    auto Append(Path const& path) -> void;
    auto Concat(Path const& path) -> void;

    auto ReplaceFileName(Path const& path) -> void;
    auto ReplaceExtension(Path const& path) -> void;

    auto GetLexicallyNormal() const -> Path;
    auto GetLexicallyRelative(Path const& base) const -> Path;
    auto GetLexicallyProximate(Path const& base) const -> Path;

    auto Clear() -> void;

    auto AsStdFilesystemPath() const -> std::filesystem::path;

private:
    explicit Path(std::filesystem::path path);
    std::filesystem::path _path;
};
}
}
