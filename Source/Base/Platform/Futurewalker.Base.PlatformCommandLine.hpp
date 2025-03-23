// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.PlatformCommandLineType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.String.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Command line.
///
class PlatformCommandLine : NonCopyable
{
public:
    virtual ~PlatformCommandLine() = 0;
    virtual auto GetArguments() const -> std::vector<String> = 0;
};
}
}
