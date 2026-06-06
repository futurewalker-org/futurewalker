// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.PlatformLocaleType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp" 
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformLocale : NonCopyable
{
public:
    virtual ~PlatformLocale() = 0;
    virtual auto GetLanguageTag() const -> String = 0;
    virtual auto GetLanguage() const -> String = 0;
    virtual auto GetScript() const -> String = 0;
    virtual auto GetCountry() const -> String = 0;
    virtual auto Equals(PlatformLocale const& other) const -> Bool = 0;
    virtual auto GetHash() const -> UInt32 = 0;
};
}
}
