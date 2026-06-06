// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Resource.ResourceManagerType.hpp"
#include "Futurewalker.Resource.ResourceType.hpp"
#include "Futurewalker.Resource.ResourceBundleType.hpp"

#include "Futurewalker.Base.PathType.hpp"
#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Locale.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Uuid.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Resource manager.
///
/// Manages list of loaded resource bundles generated from resource XML files.
/// Each resource bundle contains multiple resource modules.
/// Each resource module contains map of resource data such as localized strings.
///
/// To access resource value in a resource module, you first need to call `GetResource()` with UUID of resource module.
/// Then, call member functions of obtained Resource object to get values by resource IDs.
///
/// ResourceManager also manages current locale of the resource system.
/// Call `SetPreferredLocales()` at the start of program so that each resource module can pick best maching localization.
///
class ResourceManager : NonCopyable
{
public:
    static auto GetResource(Uuid const& uuid) -> Shared<Resource>;
    static auto SetPreferredLocales(std::vector<Locale> const& locales) -> void;
    static auto GetRepresentativeLocale() -> Optional<Locale>;

public:
    ResourceManager();

    auto LoadBundles(Path const& resourcePath) -> void;

private:
    auto InternalLoadBundles(Path const& resourcePath) -> void;
    auto InternalSetPreferredLocales(std::vector<Locale> const& locales) -> void;
    auto InternalGetResource(Uuid const& uuid) -> Shared<Resource>;
    auto InternalGetRepresentativeLocale() -> Optional<Locale>;

private:
    std::vector<Shared<ResourceBundle>> _bundles;
    std::vector<Locale> _preferredLocales;
    Optional<Locale> _representativeLocale;
};

template <>
struct Locator::Resolver<ResourceManager>
{
    using Interface = ResourceManager;
    static auto Resolve() -> Shared<ResourceManager>;
};
}
}
