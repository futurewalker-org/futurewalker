// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.ApplicationContext.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
ApplicationContext::ApplicationContext()
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
auto ApplicationContext::GetAttributeNode() const ->  AttributeNode const&
{
    return *_attributeNode;
}

///
/// @brief
///
auto Locator::Resolver<ApplicationContext>::Resolve() -> Shared<ApplicationContext>
{
    return Shared<ApplicationContext>::Make();
}
}
