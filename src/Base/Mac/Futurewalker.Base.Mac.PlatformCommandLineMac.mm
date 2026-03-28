// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Mac.PlatformCommandLineMac.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include <crt_externs.h>

namespace FW_DETAIL_NS
{
PlatformCommandLineMac::PlatformCommandLineMac()
{
    auto argc = ::_NSGetArgc();
    auto argv = ::_NSGetArgv();

    if (argc && argv)
    {
        for (int i = 0; i < *argc; ++i)
        {
            _args.emplace_back(String::MakeFromStdString(*argv[i]));
        }
    }
}

auto PlatformCommandLineMac::GetArguments() const -> std::vector<String>
{
    return _args;
}

auto Locator::Resolver<PlatformCommandLineMac>::Resolve() -> Shared<PlatformCommandLineMac>
{
    return Shared<PlatformCommandLineMac>::Make();
}
}
