// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ApplicationContext.hpp"

#include "Futurewalker.Graphics.FontManager.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
ApplicationContext::ApplicationContext(Shared<Graphics::FontManager> const& fontManager)
  : _fontManager {fontManager}
{
    _attributeNode = AttributeNode::Make();
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
    return Shared<ApplicationContext>::Make(fontManager);
}
}
