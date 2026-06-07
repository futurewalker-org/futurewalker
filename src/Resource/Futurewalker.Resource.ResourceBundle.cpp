// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Resource.ResourceBundle.hpp"
#include "Futurewalker.Resource.Resource.hpp"

#include "Futurewalker.Base.LocaleFunction.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace
{
template <class T>
auto ReadExact(FileInputStream& stream, T& value) -> Bool
{
    if (auto const readBytes = stream.Read({reinterpret_cast<std::byte*>(&value), sizeof(value)}))
    {
        return *readBytes == SInt64(sizeof(value));
    }
    return false;
}

auto LoadResourceBundleHeader(FileInputStream& stream, UInt32 const offset, ResourceBundleHeader& header) -> Bool
{
    stream.SetPosition(offset, SeekPosition::Begin);

    auto magic = std::array<std::byte, 4>();
    if (!ReadExact(stream, magic))
    {
        return false;
    }
    header.signature[0] = std::to_integer<uint8_t>(magic[0]);
    header.signature[1] = std::to_integer<uint8_t>(magic[1]);
    header.signature[2] = std::to_integer<uint8_t>(magic[2]);
    header.signature[3] = std::to_integer<uint8_t>(magic[3]);

    if (!ReadExact(stream, header.version))
    {
        return false;
    }
    if (!ReadExact(stream, header.options))
    {
        return false;
    }
    if (!ReadExact(stream, header.module_ptr))
    {
        return false;
    }
    return true;
}

auto LoadResourceBundleModuleChunk(FileInputStream& stream, UInt32 const offset, ResourceBundleModuleChunk& chunk) -> void
{
    stream.SetPosition(offset, SeekPosition::Begin);
    stream.Read({(std::byte*)&chunk.chunk_size, sizeof(chunk.chunk_size)});
    stream.Read({(std::byte*)&chunk.name_ptr, sizeof(chunk.name_ptr)});
    stream.Read({(std::byte*)&chunk.primary_locale_ptr, sizeof(chunk.primary_locale_ptr)});
    stream.Read({(std::byte*)&chunk.uuid[0], sizeof(chunk.uuid)});
    stream.Read({(std::byte*)&chunk.locales_ptr, sizeof(chunk.locales_ptr)});
    stream.Read({(std::byte*)&chunk.resources_ptr, sizeof(chunk.resources_ptr)});
    stream.Read({(std::byte*)&chunk.next_ptr, sizeof(chunk.next_ptr)});
}

auto LoadResourceBundleModuleChunks(FileInputStream& stream, UInt32 offset, std::vector<ResourceBundleModuleChunk>& chunks) -> void
{
    while (offset != 0u)
    {
        auto chunk = ResourceBundleModuleChunk();
        LoadResourceBundleModuleChunk(stream, offset, chunk);
        chunks.push_back(chunk);
        offset = chunk.next_ptr;
    }
}

auto LoadResourceBundleLocaleChunk(FileInputStream& stream, UInt32 const offset, BundleLocaleChunk& chunk) -> void
{
    stream.SetPosition(offset, SeekPosition::Begin);
    stream.Read({(std::byte*)&chunk.chunk_size, sizeof(chunk.chunk_size)});
    stream.Read({(std::byte*)&chunk.locale_ptr, sizeof(chunk.locale_ptr)});
    stream.Read({(std::byte*)&chunk.next_ptr, sizeof(chunk.next_ptr)});
}

auto LoadResourceBundleLocaleChunks(FileInputStream& stream, UInt32 offset, std::vector<BundleLocaleChunk>& chunks) -> void
{
    while (offset != 0u)
    {
        auto chunk = BundleLocaleChunk();
        LoadResourceBundleLocaleChunk(stream, offset, chunk);
        chunks.push_back(chunk);
        offset = chunk.next_ptr;
    }
}

auto LoadResourceBundleResourceChunk(FileInputStream& stream, UInt32 const offset, ResourceBundleResourcesChunk& chunk) -> void
{
    stream.SetPosition(offset, SeekPosition::Begin);
    stream.Read({(std::byte*)&chunk.chunk_size, sizeof(chunk.chunk_size)});
    stream.Read({(std::byte*)&chunk.id_ptr, sizeof(chunk.id_ptr)});
    stream.Read({(std::byte*)&chunk.number, sizeof(chunk.number)});
    stream.Read({(std::byte*)&chunk.type, sizeof(chunk.type)});
    stream.Read({(std::byte*)&chunk.resource_ptr, sizeof(chunk.resource_ptr)});
    stream.Read({(std::byte*)&chunk.next_ptr, sizeof(chunk.next_ptr)});
}

auto LoadResourceStringResourceChunk(FileInputStream& stream, UInt32 const offset, ResourceBundleStringResourceChunk& chunk) -> void
{
    stream.SetPosition(offset, SeekPosition::Begin);
    stream.Read({(std::byte*)&chunk.chunk_size, sizeof(chunk.chunk_size)});
    stream.Read({(std::byte*)&chunk.locale_ptr, sizeof(chunk.locale_ptr)});
    stream.Read({(std::byte*)&chunk.value_ptr, sizeof(chunk.value_ptr)});
    stream.Read({(std::byte*)&chunk.next_ptr, sizeof(chunk.next_ptr)});
}

auto LoadResourceDataChunkAsString(FileInputStream& stream, UInt32 const offset, String& value) -> void
{
    stream.SetPosition(offset, SeekPosition::Begin);
    auto dataChunk = ResourceBundleDataChunk();
    stream.Read({(std::byte*)&dataChunk.chunk_size, sizeof(dataChunk.chunk_size)});
    auto buffer = std::vector<std::byte>(std::max(dataChunk.chunk_size, sizeof(dataChunk.chunk_size)) - sizeof(dataChunk.chunk_size));
    stream.Read({buffer.data(), buffer.size()});
    value = String::MakeFromStdString({(char const*)buffer.data(), buffer.size()});
}
}

///
/// @brief Load resource bundle from file.
///
/// @param[in] path Path to resource bundle.
///
auto ResourceBundle::LoadFromFile(Path const& path) -> Shared<ResourceBundle>
{
    auto stream = Unique<FileInputStream>::Make(path);
    if (!stream->IsOpen())
    {
        FW_DEBUG_LOG_ERROR("Failed to open resource file: {}", path.GetNativePathString().ToStdString());
        return nullptr;
    }

    auto header = ResourceBundleHeader();
    if (!LoadResourceBundleHeader(*stream, 0u, header))
    {
        FW_DEBUG_LOG_ERROR("Invalid resource bundle signature in file: {}", path.GetNativePathString().ToStdString());
        return nullptr;
    }

    if (std::string_view((char const*)header.signature, sizeof(header.signature)) != "FWRB")
    {
        FW_DEBUG_LOG_ERROR("Invalid resource bundle signature in file: {}", path.GetNativePathString().ToStdString());
        return nullptr;
    }

    auto moduleChunks = std::vector<ResourceBundleModuleChunk>();
    LoadResourceBundleModuleChunks(*stream, header.module_ptr, moduleChunks);

    auto modules = std::vector<ResourceModule>();
    for (auto const& moduleChunk : moduleChunks)
    {
        auto primaryLocaleString = String();
        LoadResourceDataChunkAsString(*stream, moduleChunk.primary_locale_ptr, primaryLocaleString);

        auto primaryLocale = Locale::MakeFromLanguageTag(primaryLocaleString);
        if (!primaryLocale)
        {
            FW_DEBUG_LOG_ERROR("Failed to load resource bundle's primary locale: {}", path.GetNativePathString().ToStdString());
            return nullptr;
        }

        auto localeChunks = std::vector<BundleLocaleChunk>();
        LoadResourceBundleLocaleChunks(*stream, moduleChunk.locales_ptr, localeChunks);

        auto locales = std::vector<Locale>();
        locales.reserve(std::ssize(localeChunks));
        for (auto const& localeChunk : localeChunks)
        {
            auto localeString = String();
            LoadResourceDataChunkAsString(*stream, localeChunk.locale_ptr, localeString);

            auto locale = Locale::MakeFromLanguageTag(localeString);
            if (!locale)
            {
                FW_DEBUG_LOG_ERROR("Failed to load resource locale: {}", path.GetNativePathString().ToStdString());
                return nullptr;
            }
            locales.push_back(std::move(*locale));
        }

        modules.push_back({
            .moduleChunk = moduleChunk,
            .locales = std::move(locales),
            .primaryLocale = *primaryLocale,
            .currentLocale = *primaryLocale,
        });
    }

    auto bundle = Shared<ResourceBundle>::Make(PassKey<ResourceBundle>());
    bundle->_self = bundle;
    bundle->_stream = std::move(stream);
    bundle->_header = header;
    bundle->_modules = std::move(modules);
    return bundle;
}

///
/// @brief Constructor.
///
ResourceBundle::ResourceBundle(PassKey<ResourceBundle>)
{
}

///
/// @brief Get resource object for module.
///
/// @param[in] uuid Module UUID.
///
auto ResourceBundle::GetResource(Uuid const& uuid) -> Shared<Resource>
{
    auto index = SInt64(0);
    for (auto& module : _modules)
    {
        auto const& chunk = module.moduleChunk;
        if (std::equal(std::begin(chunk.uuid), std::end(chunk.uuid), uuid.data.begin()))
        {
            if (!module.resource)
            {
                module.resource = Shared<Resource>::Make(_self, index);
            }
            return module.resource;
        }
        ++index;
    }
    return {};
}

///
/// @brief Get number of modules.
///
auto ResourceBundle::GetModuleCount() const -> SInt64
{
    return std::ssize(_modules);
}

///
/// @brief Load string resource.
///
/// @param[in] moduleIndex Index of resource module.
/// @param[in] id Resource ID.
///
/// @return Loaded resource string.
///
auto ResourceBundle::LoadString(SInt64 const moduleIndex, ResourceId const id) -> Optional<String>
{
    if (moduleIndex < 0 || std::ssize(_modules) <= moduleIndex)
    {
        FW_DEBUG_ASSERT(false);
        return {};
    }

    auto& module = _modules[static_cast<size_t>(moduleIndex)];

    // Check if the resource is already loaded.
    if (auto const it = module.strings.find(id); it != module.strings.end())
    {
        return it->second;
    }

    // Load from input stream, cache the result.
    auto offset = module.moduleChunk.resources_ptr;
    while (offset != 0)
    {
        auto resourcesChunk = ResourceBundleResourcesChunk();
        LoadResourceBundleResourceChunk(*_stream, offset, resourcesChunk);

        if (resourcesChunk.number == static_cast<UInt32>(id))
        {
            if (resourcesChunk.type == ResourceBundleResourceChunkType::String)
            {
                if (auto string = InternalLoadStringResource(resourcesChunk, module.currentLocale))
                {
                    return module.strings[id] = std::move(*string);
                }

                if (auto string = InternalLoadStringResource(resourcesChunk, module.primaryLocale))
                {
                    return module.strings[id] = std::move(*string);
                }
            }
            FW_DEBUG_LOG_ERROR("Mismatched resource type for resource ID");
            return {};
        }
        offset = resourcesChunk.next_ptr;
    }
    return {};
}

///
/// @brief Get primary locale of module.
///
/// @param[in] moduleIndex Module index. 
///
auto ResourceBundle::GetPrimaryLocale(SInt64 const moduleIndex) -> Locale const&
{
    if (0 <= moduleIndex && moduleIndex < std::ssize(_modules))
    {
        return _modules[static_cast<size_t>(moduleIndex)].primaryLocale;
    }
    throw Exception(ErrorCode::InvalidArgument);
}

///
/// @brief Get current locale of module.
///
/// @param[in] moduleIndex Module index.
///
auto ResourceBundle::GetCurrentLocale(SInt64 const moduleIndex) -> Locale const&
{
    if (0 <= moduleIndex && moduleIndex < std::ssize(_modules))
    {
        return _modules[static_cast<size_t>(moduleIndex)].currentLocale;
    }
    throw Exception(ErrorCode::InvalidArgument);
}

///
/// @brief Set preferred locales to bundles.
///
/// @param[in] preferredLocales Preferred locales.
///
/// Changes current locale to best matched locale from preferred locales.
///
auto ResourceBundle::SetPreferredLocales(std::span<Locale const> preferredLocales) -> void
{
    for (auto& module : _modules)
    {
        if (auto matchedLocale = LocaleFunction::Match(module.locales, preferredLocales))
        {
            module.strings.clear();
            module.currentLocale = std::move(*matchedLocale);
        }
    }
}

///
/// @brief Load string resource for given locale.
///
/// @param[in] resourcesChunk Resources chunk.
/// @param[in] locale locale.
///
auto ResourceBundle::InternalLoadStringResource(ResourceBundleResourcesChunk const& resourcesChunk, Locale const& locale) -> Optional<String>
{
    auto dataOffset = resourcesChunk.resource_ptr;
    while (dataOffset != 0)
    {
        auto stringResourceChunk = ResourceBundleStringResourceChunk();
        LoadResourceStringResourceChunk(*_stream, dataOffset, stringResourceChunk);

        auto resourceLocale = String();
        LoadResourceDataChunkAsString(*_stream, stringResourceChunk.locale_ptr, resourceLocale);

        if (resourceLocale == locale.GetLanguageTag())
        {
            auto value = String();
            LoadResourceDataChunkAsString(*_stream, stringResourceChunk.value_ptr, value);
            return value;
        }
        dataOffset = stringResourceChunk.next_ptr;
    }
    return {};
}
}
