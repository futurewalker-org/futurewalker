// SPDX-License-Identifier: MPL-2.0

#include "main.hpp"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include <boost/program_options.hpp>

#include <print>
#include <ranges>
#include <filesystem>
#include <fstream>

namespace ResourceCompiler
{
namespace
{
struct Tree
{
    uint32_t number = 0;
    boost::uuids::uuid uuid;
    std::map<std::u16string, std::unique_ptr<Tree>> nodes;

    auto IsLeaf() const -> bool
    {
        return nodes.empty();
    }

    auto HasLeaf() const -> bool
    {
        for (auto const& [_, node] : nodes)
        {
            if (node->IsLeaf())
            {
                return true;
            }
        }
        return false;
    }

    auto HasUuid() const
    {
        return !uuid.is_nil();
    }

    auto NodeAt(std::u16string const& key) -> Tree&
    {
        auto& node = nodes[key];
        if (!node)
        {
            node = std::make_unique<Tree>();
        }
        return *node;
    }
};

auto ValidateEntryId(std::u16string const& id) -> bool
{
    auto const parts = id | std::ranges::views::split(u'.') | std::ranges::to<std::vector<std::u16string>>();
    for (auto const& part : parts)
    {
        if (part.empty())
        {
            return false;
        }

        if (!IsAlpha(part[0]))
        {
            return false;
        }

        for (auto const ch : part)
        {
            if (!IsAlpha(ch) && !IsDigit(ch) && !IsDot(ch))
            {
                return false;
            }
        }
    }
    return true;
}

auto ValidateLocale(std::u16string const& locale, bool allowEmpty) -> bool
{
    if (allowEmpty && locale.empty())
    {
        return true;
    }

    auto const parts = locale | std::ranges::views::split(u'-') | std::ranges::to<std::vector<std::u16string>>();
    if (parts.size() != 2)
    {
        return false;
    }

    auto const& language = parts[0];
    auto const& region = parts[1];

    if (language.size() != 2 || !std::all_of(language.begin(), language.end(), IsAlpha))
    {
        return false;
    }

    if (region.size() != 2 || !std::all_of(region.begin(), region.end(), IsUpperAlpha))
    {
        return false;
    }
    return true;
}

auto ValidateModuleData(ModuleData const& moduleData) -> bool
{
    // Validate that entries with same id have unique locale and a consistent type.
    auto seen = std::multimap<std::u16string, std::u16string>();
    auto idType = std::unordered_map<std::u16string, std::string_view>();

    auto check = [&seen, &idType](std::u16string const& id, std::u16string const& locale, std::string_view type) -> bool
    {
        auto [it, inserted] = idType.try_emplace(id, type);
        if (!inserted && it->second != type)
        {
            std::println("Error: Entry id '{}' is used as both '{}' and '{}'", StringToUtf8(id), it->second, type);
            return false;
        }

        auto [begin, end] = seen.equal_range(id);
        if (std::find_if(begin, end, [&locale](auto const& pair) { return pair.second == locale; }) != end)
        {
            std::println("Error: Duplicate entry id '{}' for locale '{}'", StringToUtf8(id), StringToUtf8(locale));
            return false;
        }
        seen.emplace(id, locale);

        return true;
    };

    for (auto const& [id, entry] : moduleData.strings)
    {
        for (auto const& value : entry.values)
        {
            if (!check(id, value.locale, "string"))
            {
                return false;
            }
        }
    }
    for (auto const& [id, entry] : moduleData.files)
    {
        for (auto const& value : entry.values)
        {
            if (!check(id, value.locale, "file"))
            {
                return false;
            }
        }
    }

    auto it = seen.begin();
    while (it != seen.end())
    {
        auto upperBound = seen.upper_bound(it->first);
        if (std::find(it, upperBound, std::make_pair(it->first, moduleData.primaryLocale)) == upperBound)
        {
            std::println("Error: Entry id '{}' is missing for primary locale '{}'", StringToUtf8(it->first), StringToUtf8(moduleData.primaryLocale));
            return false;
        }
        it = upperBound;
    }
    return true;
}

auto ParseModuleFile(fs::path const& moduleXmlPath, std::u16string& name, boost::uuids::uuid& uuid, std::u16string& primaryLocale) -> bool
{
    std::println("Parsing module file: {}", moduleXmlPath.string());

    auto parser = xercesc::XercesDOMParser();
    parser.setValidationScheme(xercesc::XercesDOMParser::Val_Never);
    parser.setDoNamespaces(false);
    parser.setDoSchema(false);

    try
    {
        parser.parse(moduleXmlPath.string().c_str());
    }
    catch (xercesc::SAXException const& e)
    {
        std::println("Error: XML parse error in {}: {}", moduleXmlPath.string(), XmlToUtf8(e.getMessage()));
        return false;
    }
    catch (...)
    {
        std::println("Error: Unknown exception during parsing XML file {}", moduleXmlPath.string());
        return false;
    }

    auto* doc = parser.getDocument();
    auto* root = (doc != nullptr) ? doc->getDocumentElement() : nullptr;
    if (root == nullptr)
    {
        std::println("Error: Empty document: {}", moduleXmlPath.string());
        return false;
    }

    auto rootName = XmlToString(root->getTagName());
    if (rootName != u"module")
    {
        std::println("Error: Root element must be <module>, got <{}>", StringToUtf8(rootName));
        return false;
    }

    auto moduleName = XmlToString(root->getAttribute(StringToXml(u"name")));
    auto moduleUuidStr = XmlToString(root->getAttribute(StringToXml(u"uuid")));
    auto modulePrimaryLocale = XmlToString(root->getAttribute(StringToXml(u"primary-locale")));

    if (moduleName.empty())
    {
        std::println("Error: <module> is missing 'name' attribute");
        return false;
    }

    auto moduleUuid = StringToUuid(moduleUuidStr);
    if (moduleUuid.is_nil())
    {
        std::println("Error: <module> has invalid 'uuid' attribute: {}", StringToUtf8(moduleUuidStr));
        return false;
    }

    if (!ValidateLocale(modulePrimaryLocale, false))
    {
        std::println("Error: <module> has invalid 'primary-locale' attribute: {}", StringToUtf8(modulePrimaryLocale));
        return false;
    }

    name = std::move(moduleName);
    uuid = std::move(moduleUuid);
    primaryLocale = std::move(modulePrimaryLocale);
    return true;
}

auto ParseEntryBase(xercesc::DOMElement const* element, std::u16string const& prefix) -> std::optional<std::u16string>
{
    auto id = XmlToString(element->getAttribute(StringToXml(u"id")));
    if (!ValidateEntryId(id))
    {
        std::println("Error: Invalid entry id: {}", StringToUtf8(id));
        return std::nullopt;
    }

    if (!prefix.empty())
    {
        id = prefix + u"." + id;
    }
    return id;
}

auto ParseEntry(
  xercesc::DOMElement const* element,
  fs::path const& basePath,
  std::u16string const& prefix,
  std::u16string const& locale,
  std::map<std::u16string, StringEntry>& stringEntries,
  std::map<std::u16string, FileEntry>& fileEntries,
  std::vector<fs::path>& dependentFiles) -> bool
{
    auto const tagName = XmlToString(element->getTagName());
    if (tagName == u"string")
    {
        auto entryBase = ParseEntryBase(element, prefix);
        if (!entryBase)
        {
            return false;
        }

        auto id = std::move(*entryBase);
        auto value = XmlToString(element->getTextContent());

        stringEntries[id].values.push_back(
          StringValue {
              .locale = locale,
              .value = std::move(value),
          });
    }
    else if (tagName == u"file")
    {
        auto entryBase = ParseEntryBase(element, prefix);
        if (!entryBase)
        {
            return false;
        }
        auto id = std::move(*entryBase);
        auto path = fs::path(XmlToString(element->getTextContent()));

        if (path.is_relative())
        {
            path = basePath / path;
        }

        path = path.lexically_normal();

        if (!fs::exists(path))
        {
            std::println("<file>: file path does not exist: {}", path.string());
            return false;
        }

        if (!fs::is_regular_file(path))
        {
            std::println("<file>: file path is not regular file: {}", path.string());
            return false;
        }

        fileEntries[id].values.push_back(
          FileValue {
              .locale = locale,
              .path = path.u16string(),
          });

        dependentFiles.push_back(path);
    }
    return true;
}

auto ParseResourceFile(
  fs::path const& resourceXmlPath,
  std::u16string const& primaryLocale,
  std::set<std::u16string>& locales,
  std::map<std::u16string, StringEntry>& stringEntries,
  std::map<std::u16string, FileEntry>& fileEntries,
  std::vector<fs::path>& dependentFiles) -> bool
{
    std::println("Parsing resource file: {}", resourceXmlPath.string());

    auto handler = xercesc::HandlerBase();
    auto parser = xercesc::XercesDOMParser();
    parser.setValidationScheme(xercesc::XercesDOMParser::Val_Never);
    parser.setDoNamespaces(false);
    parser.setDoSchema(false);
    parser.setErrorHandler(&handler);

    try
    {
        parser.parse(resourceXmlPath.string().c_str());
    }
    catch (xercesc::SAXException const& e)
    {
        std::println("Error: XML parse error in {}: {}", resourceXmlPath.string(), XmlToUtf8(e.getMessage()));
        return false;
    }
    catch (...)
    {
        std::println("Error: Unknown exception during parsing XML file {}", resourceXmlPath.string());
        return false;
    }

    dependentFiles.push_back(resourceXmlPath);

    auto doc = parser.getDocument();
    auto root = (doc != nullptr) ? doc->getDocumentElement() : nullptr;
    if (root == nullptr)
    {
        std::println("Error: Empty document: {}", resourceXmlPath.string());
        return false;
    }

    auto rootName = XmlToString(root->getTagName());
    if (rootName != u"resources")
    {
        std::println("Error: Root element must be <resources>, got <{}>", XmlToUtf8(root->getTagName()));
        return false;
    }

    auto const prefix = XmlToString(root->getAttribute(StringToXml(u"prefix")));
    if (!prefix.empty() && !ValidateEntryId(prefix))
    {
        std::println("Error: Invalid prefix: {}", StringToUtf8(prefix));
        return false;
    }

    auto const locale = XmlToString(root->getAttribute(StringToXml(u"locale")));
    if (!locale.empty() && !ValidateLocale(locale, true))
    {
        std::println("Error: Invalid locale: {}", StringToUtf8(locale));
        return false;
    }
    locales.insert(locale);

    auto basePath = resourceXmlPath.parent_path();

    auto childElement = root->getFirstElementChild();
    while (childElement)
    {
        if (!ParseEntry(childElement, basePath, prefix, locale, stringEntries, fileEntries, dependentFiles))
        {
            return false;
        }
        childElement = childElement->getNextElementSibling();
    }
    return true;
}

auto ParseResourceFiles(
  ModuleData& parsedModuleData,
  std::vector<fs::path>& dependentFiles,
  std::vector<fs::path>& generatedFiles,
  std::string const& sourceDirectory,
  std::string const& cacheXmlPath,
  std::string const& depfilePath,
  std::string const& headerDirectory) -> int
{
    std::println("Analyzing resources in directory: {}", sourceDirectory);
    std::println("Cache path: {}", cacheXmlPath);
    std::println("Depfile path: {}", depfilePath);
    std::println("Header path: {}", headerDirectory);

    // Find .module.xml file in source_directory, or return error if not found or multiple found.
    auto sourcePath = fs::absolute(fs::path(sourceDirectory)).lexically_normal();
    if (!fs::exists(sourcePath) || !fs::is_directory(sourcePath))
    {
        std::println("Error: Source directory does not exist or is not a directory: {}", sourceDirectory);
        return 1;
    }

    auto moduleFiles = std::vector<std::pair<fs::path, ModuleData>>();
    for (auto const& entry : fs::directory_iterator(sourcePath))
    {
        if (entry.is_regular_file() && entry.path().filename().string().ends_with(".xml"))
        {
            auto const path = entry.path();
            auto name = std::u16string();
            auto uuid = boost::uuids::uuid();
            auto primaryLocale = std::u16string();
            if (ParseModuleFile(path, name, uuid, primaryLocale))
            {
                auto moduleData = ModuleData {
                    .name = std::move(name),
                    .uuid = uuid,
                    .primaryLocale = std::move(primaryLocale),
                    .strings = {},
                    .files = {},
                };
                moduleFiles.push_back({path, std::move(moduleData)});
            }
        }
    }
    if (moduleFiles.empty())
    {
        std::println("Error: No .module.xml file found in {}", sourceDirectory);
        return 1;
    }
    if (moduleFiles.size() > 1)
    {
        std::println("Error: Multiple .module.xml files found in {}", sourceDirectory);
        return 1;
    }
    auto& [moduleXmlPath, moduleData] = moduleFiles[0];

    // Parse other .xml files in source_directory to collect resource entries.
    dependentFiles.push_back(moduleXmlPath);
    generatedFiles.push_back(cacheXmlPath);

    for (auto const& entry : fs::recursive_directory_iterator(sourcePath))
    {
        if (entry.is_regular_file() && entry.path().filename().string().ends_with(".xml"))
        {
            if (entry.path() == moduleXmlPath)
            {
                continue;
            }

            if (!ParseResourceFile(entry.path(), moduleData.primaryLocale, moduleData.locales, moduleData.strings, moduleData.files, dependentFiles))
            {
                std::println("Error: Failed to parse resource file: {}", entry.path().string());
                return 1;
            }
        }
    }

    std::println("Finished analyzing resources for module: {}", StringToUtf8(moduleData.name));
    std::println("Module name: {}", StringToUtf8(moduleData.name));
    std::println("Module UUID: {}", boost::uuids::to_string(moduleData.uuid));
    std::println("Module primary locale: {}", StringToUtf8(moduleData.primaryLocale));

    for (auto const& locale : moduleData.locales)
    {
        std::println("Locale: {}", StringToUtf8(locale));
    }

    for (auto const& [id, entry] : moduleData.strings)
    {
        for (auto const& value : entry.values)
        {
            std::println("String entry: id={}, locale={}, value={}", StringToUtf8(id), StringToUtf8(value.locale), StringToUtf8(value.value));
        }
    }

    for (auto const& [id, entry] : moduleData.files)
    {
        for (auto const& value : entry.values)
        {
            std::println("File entry: id={}, locale={}, path={}", StringToUtf8(id), StringToUtf8(value.locale), StringToUtf8(value.path));
        }
    }

    if (ValidateModuleData(moduleData))
    {
        parsedModuleData = std::move(moduleData);
        return 0;
    }
    return 1;
}

auto AssignEntryNumbers(ModuleData& moduleData, std::map<std::u16string, uint32_t> const& entryNumberMapping) -> void
{
    auto maxId = 0u;
    for (auto const& [id, entry] : moduleData.strings)
    {
        maxId = std::max(maxId, entry.number);
    }

    for (auto& [id, entry] : moduleData.strings)
    {
        auto const it = entryNumberMapping.find(id);
        if (it != entryNumberMapping.end())
        {
            entry.number = it->second;
        }
        else
        {
            entry.number = ++maxId;
        }
    }

    for (auto& [id, entry] : moduleData.files)
    {
        auto const it = entryNumberMapping.find(id);
        if (it != entryNumberMapping.end())
        {
            entry.number = it->second;
        }
        else
        {
            entry.number = ++maxId;
        }
    }
}

auto ParseCacheFileAndAssignEntryNumbers(ModuleData& moduleData, std::string const& cacheXmlPath) -> int
{
    // Parse cache file if exist.
    // We need entry ID -> numeric ID mapping to be stable across runs, so we will reuse IDs from previous cache if possible.
    std::map<std::u16string, uint32_t> entryNumberMapping;
    if (!cacheXmlPath.empty())
    {
        if (!cacheXmlPath.ends_with(".cache.xml"))
        {
            std::println("Error: Failed to load cache file at {}. Filename of cache files should be *.cache.xml", cacheXmlPath);
            return 1;
        }

        if (fs::exists(cacheXmlPath))
        {
            if (fs::is_regular_file(cacheXmlPath))
            {
                auto cacheModuleData = ModuleData();
                if (ParseCacheFile(cacheXmlPath, cacheModuleData))
                {
                    for (auto const& [id, entry] : cacheModuleData.strings)
                    {
                        entryNumberMapping[id] = entry.number;
                    }
                    for (auto const& [id, entry] : cacheModuleData.files)
                    {
                        entryNumberMapping[id] = entry.number;
                    }
                }
                else
                {
                    std::println("Warning: Failed to parse cache file: {}", cacheXmlPath);
                    entryNumberMapping = {};
                }
            }
            else
            {
                std::println("Error: Failed to load cache file at {}. Cache path is not a regular file", cacheXmlPath);
                return 1;
            }
        }
    }
    AssignEntryNumbers(moduleData, entryNumberMapping);
    return 0;
}

auto InsertModuleNode(Tree& tree, std::u16string const& id, boost::uuids::uuid const& uuid)
{
    auto const components = id | std::ranges::views::split(u'.') | std::ranges::to<std::vector<std::u16string>>();
    auto treeNode = &tree;
    for (auto const& component : components)
    {
        treeNode = &treeNode->NodeAt(component);
    }
    treeNode->uuid = uuid;
}

auto InsertEntryNode(Tree& tree, std::u16string const& id, uint32_t number)
{
    auto const components = id | std::ranges::views::split(u'.') | std::ranges::to<std::vector<std::u16string>>();
    auto treeNode = &tree;
    for (auto const& component : components)
    {
        treeNode = &treeNode->NodeAt(component);
    }
    treeNode->number = number;
}

auto EnumerateNodeWithLeaves(Tree& tree, std::vector<std::u16string>& pathStack, auto f) -> void
{
    if (tree.HasLeaf() || tree.HasUuid())
    {
        f(tree, pathStack);
    }

    for (auto const& [key, node] : tree.nodes)
    {
        pathStack.push_back(key);
        EnumerateNodeWithLeaves(*node, pathStack, f);
        pathStack.pop_back();
    }
}

auto ConcatPathStack(std::vector<std::u16string> const& pathStack, std::string_view delim)
{
    auto path = std::string();
    for (auto const& p : pathStack)
    {
        if (!path.empty())
        {
            path += delim;
        }
        path += StringToUtf8(p);
    }
    return path;
}

auto WriteHeaderFile(fs::path const& headerPath, ModuleData const& moduleData, std::vector<fs::path>& generatedFiles) -> bool
{
    try
    {
        fs::create_directories(headerPath);
    }
    catch (...)
    {
        std::println("Failed to create header path directory at {}", headerPath.string());
        return false;
    }

    auto tree = Tree();

    InsertModuleNode(tree, moduleData.name, moduleData.uuid);

    for (auto const& [id, entry] : moduleData.strings)
    {
        InsertEntryNode(tree, id, entry.number);
    }
    for (auto const& [id, entry] : moduleData.files)
    {
        InsertEntryNode(tree, id, entry.number);
    }

    auto result = true;
    auto pathStack = std::vector<std::u16string>();
    EnumerateNodeWithLeaves(tree, pathStack, [&](Tree const& tree, std::vector<std::u16string> const& pathStack) {
        auto const namespacePath = ConcatPathStack(pathStack, "::");
        auto filenamePath = ConcatPathStack(pathStack, ".");
        if (filenamePath.empty())
        {
            filenamePath = StringToUtf8(moduleData.name);
        }

        auto const headerFilePath = headerPath / (filenamePath + ".hpp");
        {
            std::ofstream headerFile(headerFilePath);
            if (!headerFile)
            {
                std::println("Error: Failed to open header file for writing: {}", headerFilePath.string());
                result = false;
                return;
            }

            headerFile << "#include <Futurewalker.Core.Uuid.hpp>\n\n";

            if (!tree.uuid.is_nil())
            {
                auto modulePathStack = pathStack;
                if (!modulePathStack.empty())
                {
                    auto moduleId = modulePathStack.back();
                    modulePathStack.pop_back();
                    auto const moduleNamespacePath = ConcatPathStack(modulePathStack, "::");

                    headerFile << "namespace Futurewalker::M";
                    if (moduleNamespacePath.empty())
                    {
                        headerFile << "\n";
                    }
                    else
                    {
                        headerFile << "::" << moduleNamespacePath << "\n";
                    }
                    headerFile << "{\n";
                    headerFile << "inline constexpr auto " << StringToUtf8(moduleId) << " = Uuid {";
                    for (int i = 0; i < 16; ++i)
                    {
                        auto const val = tree.uuid.data()[i];
                        headerFile << (int)val << ",";
                    }
                    headerFile << "};\n";
                    headerFile << "}\n";
                }
            }

            headerFile << "namespace Futurewalker::R";
            if (namespacePath.empty())
            {
                headerFile << "\n";
            }
            else
            {
                headerFile << "::" << namespacePath << "\n";
            }
            headerFile << "{\n";
            for (auto const& [id, node] : tree.nodes)
            {
                if (node->IsLeaf() && node->number != 0)
                {
                    headerFile << "inline constexpr auto " << StringToUtf8(id) << " = UInt32(" << node->number << "U);\n";
                }
            }
            headerFile << "}\n";
        }
        generatedFiles.push_back(headerFilePath);
    });
    return result;
}

auto WriteDepfile(fs::path const& depfilePath, fs::path const& cacheXmlPath, std::vector<fs::path> const& generatedFiles, std::vector<fs::path> const& dependentFiles) -> bool
{
    std::println("write_depfile: {}", depfilePath.string());

    try
    {
        fs::create_directories(depfilePath.parent_path());
    }
    catch (...)
    {
        std::println("Failed to create directories for path {}", cacheXmlPath.string());
    }

    std::ofstream depfile(depfilePath);
    if (!depfile)
    {
        std::println("Error: Failed to open depfile for writing: {}", depfilePath.string());
        return false;
    }

    for (auto const& gen : generatedFiles)
    {
        depfile << gen.string();
        depfile << " ";
    }
    depfile << ":";
    for (auto const& dep : dependentFiles)
    {
        depfile << " ";
        depfile << dep.string();
    }
    depfile << "\n";

    return true;
}

auto WriteResourceFiles(
  ModuleData const& moduleData,
  std::vector<fs::path>&& dependentFiles,
  std::vector<fs::path>&& generatedFiles,
  std::string const& cacheXmlPath,
  std::string const& depfilePath,
  std::string const& headerDirectory) -> int
{
    if (!cacheXmlPath.empty())
    {
        if (!WriteCacheFile(cacheXmlPath, moduleData))
        {
            std::println("Error: Failed to write cache file at {}", cacheXmlPath);
            return 1;
        }
    }

    if (!headerDirectory.empty())
    {
        if (!WriteHeaderFile(headerDirectory, moduleData, generatedFiles))
        {
            std::println("Error: Failed to write headers file at directory {}", headerDirectory);
            return 1;
        }
    }

    if (!depfilePath.empty())
    {
        if (!WriteDepfile(depfilePath, cacheXmlPath, generatedFiles, dependentFiles))
        {
            std::println("Error: Failed to write depfile at {}", depfilePath);
            return 1;
        }
    }
    return 0;
}
}

auto GenerateResourceFiles(std::vector<std::string> const& args) -> int
{
    auto sourceDirectory = std::string();
    auto cachePath = std::string();
    auto depfilePath = std::string();
    auto headerPath = std::string();

    auto desc = po::options_description("analyze options");

    desc.add_options()                                                                  //
      ("source-directory", po::value(&sourceDirectory)->required(), "source directory") //
      ("cache", po::value(&cachePath), "cache path")                                    //
      ("depfile", po::value(&depfilePath), "depfile path")                              //
      ("header", po::value(&headerPath), "header path");

    auto positional = po::positional_options_description();
    positional.add("source-directory", 1);

    auto vm = po::variables_map();
    try
    {
        po::store(po::command_line_parser(args).options(desc).positional(positional).run(), vm);
        po::notify(vm);
    }
    catch (po::required_option const&)
    {
        std::println("Error: Source directory not specified");
        return 1;
    }
    catch (po::error const& e)
    {
        std::println("Error: {}", e.what());
        return 1;
    }

    auto result = 0;
    auto moduleData = ModuleData();
    auto dependentFiles = std::vector<fs::path>();
    auto generatedFiles = std::vector<fs::path>();
    result = ParseResourceFiles(moduleData, dependentFiles, generatedFiles, sourceDirectory, cachePath, depfilePath, headerPath);
    if (result != 0)
    {
        return result;
    }

    result = ParseCacheFileAndAssignEntryNumbers(moduleData, cachePath);
    if (result != 0)
    {
        return result;
    }

    result = WriteResourceFiles(moduleData, std::move(dependentFiles), std::move(generatedFiles), cachePath, depfilePath, headerPath);
    if (result != 0)
    {
        return result;
    }
    return 0;
}
}
