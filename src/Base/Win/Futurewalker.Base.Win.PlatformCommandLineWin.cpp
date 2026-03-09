// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Win.PlatformCommandLineWin.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include <boost/nowide/args.hpp>

namespace FW_DETAIL_NS
{
///
/// @brief
///
PlatformCommandLineWin::PlatformCommandLineWin()
{
    auto argc = static_cast<int>(0);
    auto argv = static_cast<char**>(nullptr);
    auto args = boost::nowide::args(argc, argv);

    _args.reserve(argc);
    for (int i = 0; i < argc; ++i)
    {
        _args.emplace_back(String::MakeFromStdString(argv[i]));
    }
}

///
/// @brief
///
auto PlatformCommandLineWin::GetArguments() const -> std::vector<String>
{
    return _args;
}

///
/// @brief
///
auto Locator::Resolver<PlatformCommandLineWin>::Resolve() -> Shared<PlatformCommandLineWin>
{
    return Shared<PlatformCommandLineWin>::Make();
}
}
