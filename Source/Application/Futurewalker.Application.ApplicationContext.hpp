// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ApplicationContextType.hpp"

#include "Futurewalker.Graphics.FontManager.hpp"

#include "Futurewalker.Attribute.AttributeNodeType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
class ApplicationContext : NonCopyable
{
public:
    ApplicationContext(Shared<Graphics::FontManager> const& fontManager);

    auto GetAttributeNode() -> AttributeNode&;
    auto GetAttributeNode() const ->  AttributeNode const&;

private:
    Shared<AttributeNode> _attributeNode;
    Shared<Graphics::FontManager> _fontManager;
};

template <>
struct Locator::Resolver<ApplicationContext>
{
    using Interface = ApplicationContext;
    static auto Resolve() -> Shared<ApplicationContext>;
};
}
