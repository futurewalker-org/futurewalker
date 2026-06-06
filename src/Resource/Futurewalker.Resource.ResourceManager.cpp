// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Resource.ResourceManager.hpp"
#include "Futurewalker.Resource.ResourceBundle.hpp"

#include "Futurewalker.Base.Path.hpp"
#include "Futurewalker.Base.Debug.hpp"
#include "Futurewalker.Base.Filesystem.hpp"
#include "Futurewalker.Base.FileInputStream.hpp"
#include "Futurewalker.Base.Locale.hpp"
#include "Futurewalker.Base.LocaleFunction.hpp"

#include <boost/unordered_set.hpp>

namespace FW_DETAIL_NS
{
///
/// @brief Get resource handle for specified module UUID.
///
/// @param[in] uuid UUID of module to look for.
///
auto ResourceManager::GetResource(Uuid const& uuid) -> Shared<Resource>
{
    if (auto const manager = Locator::GetInstance<ResourceManager>())
    {
        return manager->InternalGetResource(uuid);
    }
    FW_DEBUG_ASSERT(false);
    return {};
}

///
/// @brief Set preferred locales to loaded bundles.
///
/// @param[in] locales Preferred locales.
///
auto ResourceManager::SetPreferredLocales(std::vector<Locale> const& locales) -> void
{
    if (auto const manager = Locator::GetInstance<ResourceManager>())
    {
        manager->InternalSetPreferredLocales(locales);
    }
}

///
/// @brief Get representative locale of resources.
///
/// Each resource module can have its own current locale that matches preferred locales.
/// Representative locale is useful when you need single locale that represents current locale of all loaded resource modules.
///
auto ResourceManager::GetRepresentativeLocale() -> Optional<Locale>
{
    if (auto const manager = Locator::GetInstance<ResourceManager>())
    {
        return manager->InternalGetRepresentativeLocale();
    }
    return {};
}

///
/// @brief Constructor.
///
ResourceManager::ResourceManager()
{
}

///
/// @brief Load bundles under specified resource path.
///
/// @param[in] resourcePath Resource path.
///
/// @note Replaces existing bundles.
///
auto ResourceManager::LoadBundles(Path const& resourcePath) -> void
{
    InternalLoadBundles(resourcePath);
}

///
/// @brief Load bundles from resource path to populate _bundles.
///
auto ResourceManager::InternalLoadBundles(Path const& resourcePath) -> void
{
    try
    {
        _bundles.clear();

        auto const path = resourcePath.AsStdFilesystemPath();

        // Base path must be a directory.
        if (!std::filesystem::is_directory(path))
        {
            FW_DEBUG_LOG_ERROR("Resource path is not a directory: {}", path.string());
            return;
        }

        // Register resource files in the base path.
        auto bundles = std::vector<Shared<ResourceBundle>>();
        for (auto const& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_regular_file())
            {
                auto const resourceFilePath = Path::MakeFromStdFilesystemPath(entry.path());

                if (auto bundle = ResourceBundle::LoadFromFile(resourceFilePath))
                {
                    bundle->SetPreferredLocales(_preferredLocales);
                    bundles.push_back(std::move(bundle));
                }
            }
        }

        _bundles = std::move(bundles);
    }
    catch (...)
    {
        FW_DEBUG_LOG_ERROR("Exception occurred while initializing resources");
    }
}

///
/// @brief Set preferred locales to loaded bundles.
///
auto ResourceManager::InternalSetPreferredLocales(std::vector<Locale> const& locales) -> void
{
    _preferredLocales = locales;

    // We want to decide representative locale for UI layout and text rendering.
    // Collect resolved locales of each module and match against preferred locales (can be improved?).
    auto localeSet = boost::unordered_set<Locale>();

    for (auto const& bundle : _bundles)
    {
        bundle->SetPreferredLocales(_preferredLocales);

        for (auto i = SInt64(0); i < bundle->GetModuleCount(); ++i)
        {
            localeSet.insert(bundle->GetCurrentLocale(i));
        }
    }

    // TODO: Remove extra allocation.
    auto localeArray = std::vector(localeSet.begin(), localeSet.end());
    _representativeLocale = LocaleFunction::Match(localeArray, locales);
}

///
/// @brief Get resource from UUID.
///
auto ResourceManager::InternalGetResource(Uuid const& uuid) -> Shared<Resource>
{
    for (auto const& bundle : _bundles)
    {
        if (auto resource = bundle->GetResource(uuid))
        {
            return resource;
        }
    }
    return {};
}

///
/// @brief Get representative locale.
///
auto ResourceManager::InternalGetRepresentativeLocale() -> Optional<Locale>
{
    return _representativeLocale;
}

auto Locator::Resolver<ResourceManager>::Resolve() -> Shared<ResourceManager>
{
    return Shared<ResourceManager>::Make();
}
}
