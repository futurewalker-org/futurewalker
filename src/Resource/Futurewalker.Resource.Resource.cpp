// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Resource.Resource.hpp"
#include "Futurewalker.Resource.ResourceBundle.hpp"

#include "Futurewalker.Base.Locale.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Constructor.
///
/// @param[in] bundle Source bundle.
/// @param[in] moduleIndex Index of the module.
///
Resource::Resource(Weak<ResourceBundle> const& bundle, SInt64 const& moduleIndex)
  : _bundle(bundle)
  , _moduleIndex(moduleIndex)
{
}

///
/// @brief Get string value.
///
/// @param[in] id Resource ID.
///
/// @throw Exception when resource ID is invalid or Resource is referencing unloaded bundle.
///
auto Resource::GetString(ResourceId const id) -> String
{
    if (auto const bundle = _bundle.Lock())
    {
        if (auto string = bundle->LoadString(_moduleIndex, id))
        {
            return *string;
        }
    }
    throw Exception(ErrorCode::InvalidArgument, "Invalid resource ID");
}
}
