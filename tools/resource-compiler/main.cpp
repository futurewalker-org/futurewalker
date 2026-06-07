// SPDX-License-Identifier: MPL-2.0

#include "main.hpp"

#include <xercesc/util/PlatformUtils.hpp>

#include <boost/nowide/args.hpp>
#include <boost/program_options.hpp>

#include <print>

auto main(int argc, char* argv[]) -> int
{
    xercesc::XMLPlatformUtils::Initialize();

    auto nowide = boost::nowide::args(argc, argv);

    auto printUsage = [] {
        std::println("Usage:");
        std::println("  resource-compiler --help");
        std::println("  resource-compiler --version");
        std::println("  resource-compiler --generate <source directory> [--cache PATH] [--depfile PATH] [--header PATH]");
        std::println("  resource-compiler --bundle <cache-path>... --output PATH");
    };

    if (argc < 2)
    {
        printUsage();
        return 1;
    }

    const auto command = std::string(argv[1]);
    const auto rest = std::vector<std::string>(argv + 2, argv + argc);

    if (command == "--help")
    {
        printUsage();
        return 1;
    }
    else if (command == "--version")
    {
        std::println("Futurewalker Resource Compiler version 1.0");
        return 0;
    }
    else if (command == "--generate")
    {
        return ResourceCompiler::GenerateResourceFiles(rest);
    }
    else if (command == "--bundle")
    {
        return ResourceCompiler::GenerateBundleFile(rest);
    }
    else
    {
        std::println("Unknown command: {}", command);
        return 1;
    }
}
