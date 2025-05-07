// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.ApplicationContextType.hpp"

#include "Futurewalker.Attribute.AttributeNodeType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
class ApplicationContext : NonCopyable
{
public:
    ApplicationContext();

    auto GetAttributeNode() -> AttributeNode&;
    auto GetAttributeNode() const ->  AttributeNode const&;

private:
    Shared<AttributeNode> _attributeNode;
};

template <>
struct Locator::Resolver<ApplicationContext>
{
    using Interface = ApplicationContext;
    static auto Resolve() -> Shared<ApplicationContext>;
};
}
