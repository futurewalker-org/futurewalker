// SPDX-License-Identifier: MPL-2.0

#include "main.hpp"

#include <boost/program_options.hpp>

#include <print>
#include <ranges>
#include <filesystem>
#include <fstream>
#include <variant>
#include <cassert>

namespace ResourceCompiler
{
namespace
{
struct BundleHeader
{
    uint8_t signature[4]; // "FWRB"
    uint32_t version;     // LE Version
    uint32_t options;     // LE Options
    uint32_t modulePtr;   // LE Offset of first module data chunk.
};

struct BundleModuleChunk
{
    uint32_t chunkSize;        // LE Chunk size.
    uint32_t namePtr;          // LE offset of name string chunk.
    uint32_t primaryLocalePtr; // LE Offset of locale string chunk.
    uint8_t uuid[16];          // UUID of the module.
    uint32_t localesPtr;       // LE offset of first locale chunk.
    uint32_t resourcesPtr;     // LE offset of first resources chunk.
    uint32_t nextPtr;          // LE Offset of next module chunk.
};

struct BundleLocaleChunk
{
    uint32_t chunkSize; // LE Chunk size.
    uint32_t localePtr; // LE Offset of locale string chunk.
    uint32_t nextPtr;   // LE Offset of next locale chunk.
};

enum class BundleResourcesChunkType : uint32_t
{
    String = 1,
    File = 2,
};

struct BundleResourcesChunk
{
    uint32_t chunkSize;   // LE Chunk size.
    uint32_t idPtr;       // LE Offset of ID string chunk.
    uint32_t number;      // LE Resource number.
    uint32_t type;        // LE Resource type.
    uint32_t resourcePtr; // LE Offset of first resource data chunk.
    uint32_t nextPtr;     // LE Offset of next resources chunk.
};

struct BundleStringResourceChunk
{
    uint32_t chunkSize; // LE Chunk size.
    uint32_t localePtr; // LE Offset of locale string chunk.
    uint32_t valuePtr;  // LE Offset of value string chunk.
    uint32_t nextPtr;   // LE Offset of next resource data chunk.
};

struct BundleFileResourceChunk
{
    uint32_t chunkSize; // LE Chunk size.
    uint32_t localePtr; // LE Offset of locale string chunk.
    uint32_t dataPtr;   // LE Offset of data chunk.
    uint32_t nextPtr;   // LE Offset of next resource data chunk.
};

struct BundleDataChunk
{
    uint32_t chunkSize; // LE Chunk size.
    uint8_t data[];     // Variable length data.
};

auto ParseModuleDataFromCacheFiles(std::vector<ModuleData>& moduleDataList, std::vector<std::string> cachePaths) -> int
{
    std::sort(cachePaths.begin(), cachePaths.end());
    cachePaths.erase(std::unique(cachePaths.begin(), cachePaths.end()), cachePaths.end());

    for (auto const& cachePath : cachePaths)
    {
        auto moduleData = ModuleData();
        if (ParseCacheFile(cachePath, moduleData))
        {
            std::println("Parsed module from cache: {}", StringToUtf8(moduleData.name));
            moduleDataList.push_back(std::move(moduleData));
        }
    }

    if (moduleDataList.empty())
    {
        std::println("No module data found in cache file");
        return 1;
    }
    return 0;
}

template <class T>
auto StreamWrite(std::ofstream& stream, T const& value)
{
    stream.write(reinterpret_cast<char const*>(&value), sizeof(T));
}

auto WriteString(std::ofstream& stream, std::u16string const& string)
{
    auto p = stream.tellp();
    auto const utf8 = StringToUtf8(string);
    StreamWrite(stream, uint32_t(4 + utf8.length())); // chunk_size
    stream.write(reinterpret_cast<char const*>(utf8.data()), utf8.length()); // data
    return static_cast<uint32_t>(p);
}

auto WriteFile(std::ofstream& stream, fs::path const& filePath) -> uint32_t
{
    auto p = stream.tellp();
    auto file = std::ifstream(filePath, std::ios_base::binary);
    file.seekg(0, std::ios_base::end);
    auto const fileSize = static_cast<uint32_t>(file.tellg());
    file.seekg(0, std::ios_base::beg);
    StreamWrite(stream, uint32_t(4 + fileSize));
    stream << file.rdbuf();
    return static_cast<uint32_t>(p);
}

using EntryVariant = std::variant<StringEntry, FileEntry>;

auto GetBundleResourceType(EntryVariant const& entry) -> BundleResourcesChunkType
{
    if (std::holds_alternative<StringEntry>(entry))
    {
        return BundleResourcesChunkType::String;
    }
    else if (std::holds_alternative<FileEntry>(entry))
    {
        return BundleResourcesChunkType::File;
    }
    assert(false);
    return {};
}

auto WriteResourceValue(std::ofstream& stream, size_t i, std::vector<StringValue> const& values) -> uint32_t
{
    if (i >= values.size())
    {
        return 0;
    }

    auto const& value = values[i];
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleStringResourceChunk), std::ios_base::cur);
    auto nextPtr = WriteResourceValue(stream, i + 1, values);
    auto localePtr = WriteString(stream, value.locale);
    auto valuePtr = WriteString(stream, value.value);
    auto q = stream.tellp();
    stream.seekp(p);
    StreamWrite(stream, uint32_t(sizeof(BundleStringResourceChunk)));
    StreamWrite(stream, localePtr);
    StreamWrite(stream, valuePtr);
    StreamWrite(stream, nextPtr);
    stream.seekp(q);
    return static_cast<uint32_t>(p);
}

auto WriteResourceValue(std::ofstream& stream, size_t i, std::vector<FileValue> const& values) -> uint32_t
{
    if (i >= values.size())
    {
        return 0;
    }

    auto const& value = values[i];
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleFileResourceChunk), std::ios_base::cur);
    auto nextPtr = WriteResourceValue(stream, i + 1, values);
    auto localePtr = WriteString(stream, value.locale);
    auto valuePtr = WriteFile(stream, value.path);
    auto q = stream.tellp();
    stream.seekp(p);
    StreamWrite(stream, uint32_t(sizeof(BundleFileResourceChunk)));
    StreamWrite(stream, localePtr);
    StreamWrite(stream, valuePtr);
    StreamWrite(stream, nextPtr);
    stream.seekp(q);
    return static_cast<uint32_t>(p);
}

auto WriteResourcesEntries(std::ofstream& stream, size_t index, std::vector<EntryVariant> const& entries) -> uint32_t
{
    if (index >= entries.size())
    {
        return 0;
    }

    auto const& entry = entries[index];
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleResourcesChunk), std::ios_base::cur);
    auto nextPtr = WriteResourcesEntries(stream, index + 1, entries);
    auto resourcePtr = std::visit([&](auto const& e) { return WriteResourceValue(stream, 0, e.values); }, entry);
    auto idPtr = WriteString(stream, std::visit([](auto const& e) { return e.id; }, entry));
    auto q = stream.tellp();
    stream.seekp(p);
    StreamWrite(stream, uint32_t(sizeof(BundleResourcesChunk)));
    StreamWrite(stream, idPtr);
    StreamWrite(stream, std::visit([](auto const& e) { return e.number; }, entry));
    StreamWrite(stream, uint32_t(GetBundleResourceType(entry)));
    StreamWrite(stream, resourcePtr);
    StreamWrite(stream, nextPtr);
    stream.seekp(q);
    return static_cast<uint32_t>(p);
}

auto WriteResources(std::ofstream& stream, ModuleData const& moduleData) -> uint32_t
{
    std::vector<EntryVariant> entries;
    entries.reserve(moduleData.strings.size() + moduleData.files.size());
    for (auto const& [id, entry] : moduleData.strings)
    {
        entries.emplace_back(entry);
    }
    for (auto const& [id, entry] : moduleData.files)
    {
        entries.emplace_back(entry);
    }
    return WriteResourcesEntries(stream, 0, entries);
}

auto WriteBundleLocale(std::ofstream& stream, size_t index, std::vector<std::u16string> const& locales) -> uint32_t
{
    if (index >= locales.size())
    {
        return 0;
    }

    auto const& locale = locales[index];
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleLocaleChunk), std::ios_base::cur);
    auto nextPtr = WriteBundleLocale(stream, index + 1, locales);
    auto localePtr = WriteString(stream, locale);
    auto q = stream.tellp();
    stream.seekp(p);
    StreamWrite(stream, uint32_t(sizeof(BundleLocaleChunk)));
    StreamWrite(stream, localePtr);
    StreamWrite(stream, nextPtr);
    stream.seekp(q);
    return static_cast<uint32_t>(p);
}

auto WriteBundleLocales(std::ofstream& stream, ModuleData const& moduleData) -> uint32_t
{
    std::vector<std::u16string> locales(moduleData.locales.begin(), moduleData.locales.end());
    return WriteBundleLocale(stream, 0, locales);
}

auto WriteBundleModule(std::ofstream& stream, size_t index, std::vector<ModuleData> const& moduleDataList) -> uint32_t
{
    if (index >= moduleDataList.size())
    {
        return 0;
    }

    auto const& moduleData = moduleDataList[index];
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleModuleChunk), std::ios_base::cur);
    auto nextPtr = WriteBundleModule(stream, index + 1, moduleDataList);
    auto localesPtr = WriteBundleLocales(stream, moduleData);
    auto resourcesPtr = WriteResources(stream, moduleData);
    auto namePtr = WriteString(stream, moduleData.name);
    auto primaryLocalePtr = WriteString(stream, moduleData.primaryLocale);
    auto q = stream.tellp();
    stream.seekp(p);
    StreamWrite(stream, uint32_t(sizeof(BundleModuleChunk)));
    StreamWrite(stream, namePtr);
    StreamWrite(stream, primaryLocalePtr);
    for (auto const& data : moduleData.uuid)
    {
        StreamWrite(stream, data);
    }
    StreamWrite(stream, localesPtr);
    StreamWrite(stream, resourcesPtr);
    StreamWrite(stream, nextPtr);
    stream.seekp(q);
    return static_cast<uint32_t>(p);
}

auto WriteBundleModules(std::ofstream& stream, std::vector<ModuleData> const& moduleDataList) -> uint32_t
{
    return WriteBundleModule(stream, 0, moduleDataList);
}

auto WriteBundleHeader(std::ofstream& stream, std::vector<ModuleData> const& moduleDataList)
{
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleHeader), std::ios_base::cur);
    auto const modulesPtr = WriteBundleModules(stream, moduleDataList);
    stream.seekp(p);
    stream.write("FWRB", 4);
    StreamWrite(stream, uint32_t(1));
    StreamWrite(stream, uint32_t(0));
    StreamWrite(stream, modulesPtr);
}

auto WriteBundleFile(std::vector<ModuleData> const& moduleDataList, fs::path const& outputPath) -> int
{
    std::println("Output path: {}", outputPath.string());

    try
    {
        fs::create_directories(outputPath.parent_path());
    }
    catch (...)
    {
        std::println("Failed to create directories for path {}", outputPath.parent_path().string());
        return 1;
    }

    std::ofstream bundleFile(outputPath, std::ios::binary);
    if (!bundleFile)
    {
        std::println("Failed to open output file: {}", outputPath.string());
        return 1;
    }

    WriteBundleHeader(bundleFile, moduleDataList);
    return 0;
}
}

auto GenerateBundleFile(std::vector<std::string> const& args) -> int
{
    auto cachePaths = std::vector<std::string>();
    auto outputPath = std::string();

    auto desc = po::options_description("compile options");

    desc.add_options()                                       //
      ("cache-path", po::value(&cachePaths), "cache paths") //
      ("output", po::value(&outputPath)->required(), "output path");

    auto positional = po::positional_options_description();
    positional.add("cache-path", -1);

    auto vm = po::variables_map();
    try
    {
        po::store(po::command_line_parser(args).options(desc).positional(positional).run(), vm);
        po::notify(vm);
    }
    catch (po::required_option const& e)
    {
        if (e.get_option_name() == "--output")
        {
            std::println("Error: Output path not specified");
        }
        else
        {
            std::println("Error: {}", e.what());
        }
        return 1;
    }
    catch (po::error const& e)
    {
        std::println("Error: {}", e.what());
        return 1;
    }

    if (cachePaths.empty())
    {
        std::println("Error: No cache paths specified");
        return 1;
    }

    auto moduleDataList = std::vector<ModuleData>();
    auto result = ParseModuleDataFromCacheFiles(moduleDataList, cachePaths);
    if (result != 0)
    {
        return result;
    }

    return WriteBundleFile(moduleDataList, outputPath);
}
}
