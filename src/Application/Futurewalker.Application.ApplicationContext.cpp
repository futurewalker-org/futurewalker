// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ApplicationContext.hpp"

#include "Futurewalker.Graphics.FontManager.hpp"

#include "Futurewalker.Resource.ResourceManager.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp"

#include "Futurewalker.Base.Filesystem.hpp"
#include "Futurewalker.Base.Locale.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
ApplicationContext::ApplicationContext(Shared<Graphics::FontManager> const& fontManager, Shared<ResourceManager> const& resourceManager)
  : _fontManager(fontManager)
  , _resourceManager(resourceManager)
{
    _attributeNode = AttributeNode::Make();

    auto exePath = Filesystem::GetCurrentExecutablePath();
    auto resourceDir = exePath.GetParentPath();
    resourceDir.Append(Path::MakeFromString(u8"Resource"));
    _resourceManager->LoadBundles(resourceDir);
}

///
/// @brief
///
auto ApplicationContext::GetAttributeNode() -> AttributeNode&
{
    return *_attributeNode;
}

///
/// @brief
///
auto ApplicationContext::GetAttributeNode() const -> AttributeNode const&
{
    return *_attributeNode;
}

///
/// @brief
///
auto Locator::Resolver<ApplicationContext>::Resolve() -> Shared<ApplicationContext>
{
    auto const fontManager = Locator::ResolveWithDefault<Graphics::FontManager>();
    auto const resourceManager = Locator::ResolveWithDefault<ResourceManager>();
    return Shared<ApplicationContext>::Make(fontManager, resourceManager);
}
}
