// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Resource.ResourceBundleType.hpp"
#include "Futurewalker.Resource.ResourceType.hpp"

#include "Futurewalker.Base.Path.hpp"
#include "Futurewalker.Base.FileInputStream.hpp"
#include "Futurewalker.Base.Locale.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Uuid.hpp"
#include "Futurewalker.Core.HashMap.hpp"
#include "Futurewalker.Core.SharedArray.hpp"
#include "Futurewalker.Core.MemoryInputStream.hpp"

#include <boost/unordered_map.hpp>

#include <vector>

namespace FW_DETAIL_NS
{
///
/// @brief Holds data for single resource bundle file.
///
class ResourceBundle : NonCopyable
{
public:
    static auto LoadFromFile(Path const& path) -> Shared<ResourceBundle>;

    ResourceBundle(PassKey<ResourceBundle>);

    auto GetResource(Uuid const& uuid) -> Shared<Resource>;

    auto GetModuleCount() const -> SInt64;

    auto LoadString(SInt64 const moduleIndex, ResourceId const id) -> Optional<String>;
    auto LoadFile(SInt64 const moduleIndex, ResourceId const id) -> Shared<InputStream>;

    auto GetPrimaryLocale(SInt64 const moduleIndex) -> Locale const&;
    auto GetCurrentLocale(SInt64 const moduleIndex) -> Locale const&;

    auto SetPreferredLocales(std::span<Locale const> preferredLocales) -> void;

private:
    auto InternalLoadStringResource(ResourceBundleResourcesChunk const& resourcesChunk, Locale const& locale) -> Optional<String>;
    auto InternalLoadFileResource(ResourceBundleResourcesChunk const& resourcesChunk, Locale const& locale) -> Unique<MemoryInputStream>;

private:
    struct ResourceModule
    {
        ResourceBundleModuleChunk moduleChunk; // Module header chunk data;
        std::vector<Locale> locales;           // Supported locales.
        Locale primaryLocale;                  // Primary locale.
        Locale currentLocale;                  // Current locale.
        boost::unordered_map<ResourceId, String> strings;   // Cached strings.
        boost::unordered_map<ResourceId, Unique<MemoryInputStream>> files; // Cached files.
        Shared<Resource> resource;             // Loaded resource (cached).
    };
    Weak<ResourceBundle> _self;           // Self.
    Unique<FileInputStream> _stream;      // Stream of bundle file.
    ResourceBundleHeader _header;         // Bundle header chunk data.
    std::vector<ResourceModule> _modules; // List of module in the bundle.
};
}
