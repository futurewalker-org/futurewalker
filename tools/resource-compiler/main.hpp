// SPDX-License-Identifier: MPL-2.0
#pragma once

#include <xercesc/util/XMLString.hpp>

#include <boost/uuid.hpp> 
#include <boost/program_options.hpp>

#include <unicode/ustring.h>

#include <cstdint>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <filesystem>

namespace ResourceCompiler
{
namespace fs = std::filesystem;
namespace po = boost::program_options;

struct StringValue
{
    std::u16string locale; /// Locale of the entry, in IETF BCP 47 format (e.g., "en-US", "ja-JP").
    std::u16string value;  /// Value of the string entry.
};

struct FileValue
{
    std::u16string locale; /// Locale of the entry, in IETF BCP 47 format (e.g., "en-US", "ja-JP").
    std::u16string path;   /// Path to the file for the file entry.
};

struct StringEntry
{
    std::u16string id;   /// Identifier of the entry. Must be dot-separated string (e.g. "HelloWorld.MainWindow.Title").
    uint32_t number = 0; /// Resource number. Used as identifier to query entry from resource bundle.
    std::vector<StringValue> values;
};

struct FileEntry
{
    std::u16string id;   /// Identifier of the entry. Must be dot-separated string (e.g. "HelloWorld.MainWindow.Title").
    uint32_t number = 0; /// Resource number. Used as identifier to query entry from resource bundle.
    std::vector<FileValue> values;
};

struct ModuleData
{
    std::u16string name;                           /// Display name of the module.
    boost::uuids::uuid uuid;                       /// Unique identifier of the module, in UUID format.
    std::u16string primaryLocale;                  /// Primary locale of the module, in IETF BCP 47 format.
    std::set<std::u16string> locales;              /// List of supported locales in the module, in IETF BCP 47 format.
    std::map<std::u16string, StringEntry> strings; /// List of string entries in the module.
    std::map<std::u16string, FileEntry> files;     /// List of file entries in the module
};

template <class DOMObjectType>
using UniqueDomPtr = std::unique_ptr<DOMObjectType, decltype([](DOMObjectType* obj) { if (obj) { obj->release(); } })>;

template <class DOMObjectType>
auto WrapDomPtr(DOMObjectType* ptr)
{
    return UniqueDomPtr<DOMObjectType>(ptr);
}

auto StringToXml(std::u16string const& str) -> XMLCh const*;
auto XmlToString(XMLCh const* xmlCh) -> std::u16string;
auto StringToUtf8(std::u16string const& str) -> std::string;
auto Utf8ToString(std::string const& str) -> std::u16string;
auto XmlToUtf8(XMLCh const* xmlCh) -> std::string;
auto StringToUuid(std::u16string const& str) -> boost::uuids::uuid;
auto StringToUnsignedInt(std::u16string const& str) -> uint32_t;
auto IsAlpha(char16_t ch) -> bool;
auto IsUpperAlpha(char16_t ch) -> bool;
auto IsDigit(char16_t ch) -> bool;
auto IsDot(char16_t ch) -> bool;

auto ParseCacheFile(fs::path const& cacheXmlPath, ModuleData& moduleData) -> bool;
auto WriteCacheFile(fs::path const& cacheXmlPath, ModuleData const& moduleData) -> bool;

auto GenerateResourceFiles(std::vector<std::string> const& args) -> int;
auto GenerateBundleFile(std::vector<std::string> const& args) -> int;
}
