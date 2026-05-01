#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include <boost/nowide/args.hpp>
#include <boost/program_options.hpp>
#include <boost/uuid.hpp>
#include <boost/variant2.hpp>

#include <filesystem>
#include <print>
#include <string>
#include <set>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
#include <ranges>
#include <iostream>

namespace fs = std::filesystem;
namespace po = boost::program_options;

namespace
{
struct StringEntity
{
    std::u16string id;     /// Identifier of the entity. Must be dot-separated string (e.g. "HelloWorld.MainWindow.Title").
    std::u16string locale; /// Locale of the entity, in IETF BCP 47 format (e.g., "en-US", "ja-JP").
    std::u16string value;  /// Value of the string entity.
};

struct FileEntity
{
    std::u16string id;     /// Identifier of the entity. Must be dot-separated string (e.g. "HelloWorld.MainWindow.Title").
    std::u16string locale; /// Locale of the entity, in IETF BCP 47 format (e.g., "en-US", "ja-JP").
    std::u16string path;   /// Path to the file for the file entity.
};

struct ModuleData
{
    std::u16string name;               /// Display name of the module.
    boost::uuids::uuid uuid;           /// Unique identifier of the module, in UUID format.
    std::u16string primary_locale;     /// Primary locale of the module, in IETF BCP 47 format (e.g., "en-US", "ja-JP").
    std::vector<StringEntity> strings; /// List of string entities in the module.
    std::vector<FileEntity> files;     /// List of file entities in the module
};

auto string_to_xml(std::u16string const& str) -> XMLCh const*
{
    return reinterpret_cast<XMLCh const*>(str.c_str());
}

auto xml_to_string(XMLCh const* xml_ch) -> std::u16string
{
    return std::u16string(reinterpret_cast<char16_t const*>(xml_ch));
}

auto xml_to_narrow_string(XMLCh const* xml_ch) -> std::string
{
    return boost::nowide::narrow(xml_to_string(xml_ch));
}

auto string_to_uuid(std::u16string const& str) -> boost::uuids::uuid
{
    try
    {
        std::string str_utf8 = boost::nowide::narrow(str);
        return boost::uuids::string_generator()(str_utf8);
    }
    catch (const std::exception& e)
    {
        return boost::uuids::uuid();
    }
}

auto is_alpha = [](char16_t ch) -> bool { return (u'a' <= ch && ch <= u'z') || (u'A' <= ch && ch <= u'Z'); };
auto is_digit = [](char16_t ch) -> bool { return (u'0' <= ch && ch <= u'9'); };
auto is_dot = [](char16_t ch) -> bool { return ch == u'.'; };

auto validate_entity_id(const std::u16string& id) -> bool
{
    auto const parts = id | std::ranges::views::split(u'.') | std::ranges::to<std::vector<std::u16string>>();
    for (auto const& part : parts)
    {
        if (part.empty())
        {
            return false;
        }

        if (!is_alpha(part[0]))
        {
            return false;
        }

        for (auto const ch : part)
        {
            if (!is_alpha(ch) && !is_digit(ch) && !is_dot(ch))
            {
                return false;
            }
        }
    }
    return true;
}

auto validate_locale(const std::u16string& locale) -> bool
{
    auto const parts = locale | std::ranges::views::split(u'-') | std::ranges::to<std::vector<std::u16string>>();
    if (parts.size() != 2)
    {
        return false;
    }

    auto const& language = parts[0];
    auto const& region = parts[1];

    if (language.size() != 2 || !std::all_of(language.begin(), language.end(), is_alpha))
    {
        return false;
    }

    if (region.size() != 2 || !std::all_of(region.begin(), region.end(), is_alpha))
    {
        return false;
    }
    return true;
}

auto parse_module_file(const fs::path& module_xml_path, std::u16string& name, boost::uuids::uuid& uuid, std::u16string& primary_locale) -> bool
{
    std::println("Parsing module file: {}", module_xml_path.string());

    auto parser = xercesc::XercesDOMParser();
    parser.setValidationScheme(xercesc::XercesDOMParser::Val_Never);
    parser.setDoNamespaces(false);
    parser.setDoSchema(false);

    try
    {
        parser.parse(module_xml_path.string().c_str());
    }
    catch (const xercesc::XMLException& e)
    {
        std::println("Error: XML parse error in {}: {}", module_xml_path.string(), xml_to_narrow_string(e.getMessage()));
        return false;
    }
    catch (const xercesc::DOMException& e)
    {
        std::println("Error: DOM error in {}: {}", module_xml_path.string(), xml_to_narrow_string(e.getMessage()));
        return false;
    }

    auto* doc = parser.getDocument();
    auto* root = (doc != nullptr) ? doc->getDocumentElement() : nullptr;
    if (root == nullptr)
    {
        std::println("Error: Empty document: {}", module_xml_path.string());
        return false;
    }

    auto root_name = xml_to_string(root->getTagName());
    if (root_name != u"module")
    {
        std::println("Error: Root element must be <module>, got <{}>", boost::nowide::narrow(root_name));
        return false;
    }

    auto module_name = xml_to_string(root->getAttribute(string_to_xml(u"name")));
    auto module_uuid_str = xml_to_string(root->getAttribute(string_to_xml(u"uuid")));
    auto module_primary_locale = xml_to_string(root->getAttribute(string_to_xml(u"primary-locale")));

    if (module_name.empty())
    {
        std::println("Error: <module> is missing 'name' attribute");
        return false;
    }

    auto module_uuid = string_to_uuid(module_uuid_str);
    if (module_uuid.is_nil())
    {
        std::println("Error: <module> has invalid 'uuid' attribute: {}", boost::nowide::narrow(module_uuid_str));
        return false;
    }

    if (!validate_locale(module_primary_locale))
    {
        std::println("Error: <module> has invalid 'primary-locale' attribute: {}", boost::nowide::narrow(module_primary_locale));
        return false;
    }

    name = std::move(module_name);
    uuid = std::move(module_uuid);
    primary_locale = std::move(module_primary_locale);
    return true;
}

auto parse_entity_base(xercesc::DOMElement const* element, std::u16string const& prefix, std::u16string const& default_locale) -> std::optional<std::pair<std::u16string, std::u16string>>
{
    auto id = xml_to_string(element->getAttribute(string_to_xml(u"id")));
    if (!validate_entity_id(id))
    {
        std::println("Error: Invalid entity id: {}", boost::nowide::narrow(id));
        return std::nullopt;
    }

    if (!prefix.empty())
    {
        id = prefix + u"." + id;
    }

    auto locale = xml_to_string(element->getAttribute(string_to_xml(u"locale")));
    if (locale.empty())
    {
        if (default_locale.empty())
        {
            auto const tag_name = xml_to_string(element->getTagName());
            std::println("Error: <{}> with id '{}' is missing 'locale' attribute and no default locale is specified", boost::nowide::narrow(tag_name), boost::nowide::narrow(id));
            return std::nullopt;
        }
        locale = default_locale;
    }
    else if (!validate_locale(locale))
    {
        std::println("Error: Invalid locale: {}", boost::nowide::narrow(locale));
        return std::nullopt;
    }
    return std::make_pair(std::move(id), std::move(locale));
}

auto parse_entity(
  xercesc::DOMElement const* element,
  std::u16string const& prefix,
  std::u16string const& default_locale,
  std::vector<StringEntity>& string_entities,
  std::vector<FileEntity>& file_entities) -> bool
{
    auto const tag_name = xml_to_string(element->getTagName());
    if (tag_name == u"string")
    {
        auto entity_base = parse_entity_base(element, prefix, default_locale);
        if (!entity_base)
        {
            return false;
        }

        auto [id, locale] = std::move(*entity_base);
        auto value = xml_to_string(element->getTextContent());

        string_entities.push_back(
          StringEntity {
              .id = std::move(id),
              .locale = std::move(locale),
              .value = std::move(value),
          });
    }
    else if (tag_name == u"file")
    {
        auto entity_base = parse_entity_base(element, prefix, default_locale);
        if (!entity_base)
        {
            return false;
        }
        auto [id, locale] = std::move(*entity_base);
        auto path = xml_to_string(element->getTextContent());

        file_entities.push_back(
          FileEntity {
              .id = std::move(id),
              .locale = std::move(locale),
              .path = std::move(path),
          });
    }
    return true;
}

auto parse_resource_file(const fs::path& resource_xml_path, std::u16string const& primary_locale, std::vector<StringEntity>& string_entities, std::vector<FileEntity>& file_entities) -> bool
{
    std::println("Parsing resource file: {}", resource_xml_path.string());

    auto parser = xercesc::XercesDOMParser();
    parser.setValidationScheme(xercesc::XercesDOMParser::Val_Never);
    parser.setDoNamespaces(false);
    parser.setDoSchema(false);

    try
    {
        parser.parse(resource_xml_path.string().c_str());
    }
    catch (const xercesc::XMLException& e)
    {
        std::println("Error: XML parse error in {}: {}", resource_xml_path.string(), xml_to_narrow_string(e.getMessage()));
        false;
    }
    catch (const xercesc::DOMException& e)
    {
        std::println("Error: DOM error in {}: {}", resource_xml_path.string(), xml_to_narrow_string(e.getMessage()));
        return false;
    }

    auto doc = parser.getDocument();
    auto root = (doc != nullptr) ? doc->getDocumentElement() : nullptr;
    if (root == nullptr)
    {
        std::println("Error: Empty document: {}", resource_xml_path.string());
        return false;
    }

    auto root_name = xml_to_string(root->getTagName());
    if (root_name != u"resources")
    {
        std::println("Error: Root element must be <resources>, got <{}>", xml_to_narrow_string(root->getTagName()));
        return false;
    }

    auto const prefix = xml_to_string(root->getAttribute(string_to_xml(u"prefix")));
    if (!prefix.empty() && !validate_entity_id(prefix))
    {
        std::println("Error: Invalid prefix: {}", boost::nowide::narrow(prefix));
        return false;
    }

    auto const locale = xml_to_string(root->getAttribute(string_to_xml(u"locale")));
    if (!locale.empty() && !validate_locale(locale))
    {
        std::println("Error: Invalid locale: {}", boost::nowide::narrow(locale));
        return false;
    }

    auto child_element = root->getFirstElementChild();
    while (child_element)
    {
        if (!parse_entity(child_element, prefix, locale, string_entities, file_entities))
        {
            return false;
        }
        child_element = child_element->getNextElementSibling();
    }
    return true;
}

auto validate_module_data(const ModuleData& module_data) -> bool
{
    // Validate that entities with same id must have unique locale and a consistent type.
    auto seen = std::set<std::pair<std::u16string, std::u16string>>();
    auto id_type = std::unordered_map<std::u16string, std::string_view>();

    auto check = [&seen, &id_type](std::u16string const& id, std::u16string const& locale, std::string_view type) -> bool
    {
        auto [it, inserted] = id_type.try_emplace(id, type);
        if (!inserted && it->second != type)
        {
            std::println("Error: Entity id '{}' is used as both '{}' and '{}'", boost::nowide::narrow(id), it->second, type);
            return false;
        }
        if (!seen.insert(std::pair(id, locale)).second)
        {
            std::println("Error: Duplicate entity id '{}' for locale '{}'", boost::nowide::narrow(id), boost::nowide::narrow(locale));
            return false;
        }
        return true;
    };

    for (auto const& entity : module_data.strings)
    {
        if (!check(entity.id, entity.locale, "string"))
        {
            return false;
        }
    }
    for (auto const& entity : module_data.files)
    {
        if (!check(entity.id, entity.locale, "file"))
        {
            return false;
        }
    }
    return true;
}

auto analyze_resources(const std::string& source_directory, const std::string& cache_path, const std::string& depfile_path, const std::string& header_path) -> int
{
    std::println("Analyzing resources in directory: {}", source_directory);
    std::println("Cache path: {}", cache_path);
    std::println("Depfile path: {}", depfile_path);
    std::println("Header path: {}", header_path);

    // Find .module.xml file in source_directory, or return error if not found or multiple found.
    auto source_path = fs::path(source_directory);
    if (!fs::exists(source_path) || !fs::is_directory(source_path))
    {
        std::println("Error: Source directory does not exist or is not a directory: {}", source_directory);
        return 1;
    }
    auto module_files = std::vector<fs::path>();
    for (const auto& entry : fs::directory_iterator(source_path))
    {
        if (entry.is_regular_file() && entry.path().filename().string().ends_with(".module.xml"))
        {
            module_files.push_back(entry.path());
        }
    }
    if (module_files.empty())
    {
        std::println("Error: No .module.xml file found in {}", source_directory);
        return 1;
    }
    if (module_files.size() > 1)
    {
        std::println("Error: Multiple .module.xml files found in {}", source_directory);
        return 1;
    }
    const auto& module_xml_path = module_files.front();

    // Parse .module.xml file and extract resource module information.
    // Module is described by a <module> element, which has the following attributes:
    // - name: Display name of the module.
    // - uuid: Unique identifier of the module, in UUID format.
    // - primary-locale: Primary locale of the module, in IETF BCP 47 format (e.g., "en-US", "ja-JP").
    auto name = std::u16string();
    auto uuid = boost::uuids::uuid();
    auto primary_locale = std::u16string();
    if (!parse_module_file(module_xml_path, name, uuid, primary_locale))
    {
        return 1;
    }

    // Store extracted information in g_modules.
    auto module_data = ModuleData {
        .name = std::move(name),
        .uuid = uuid,
        .primary_locale = std::move(primary_locale),
        .strings = {},
        .files = {},
    };

    // Parse other .xml files in source_directory to collect resource entities.
    for (auto const& entry : fs::recursive_directory_iterator(source_path))
    {
        if (entry.is_regular_file() && entry.path().filename().string().ends_with("resource.xml"))
        {
            if (!parse_resource_file(entry.path(), primary_locale, module_data.strings, module_data.files))
            {
                std::println("Error: Failed to parse resource file: {}", entry.path().string());
                return 1;
            }
        }
    }

    std::println("Finished analyzing resources for module: {}", boost::nowide::narrow(module_data.name));
    std::println("Module name: {}", boost::nowide::narrow(name));
    std::println("Module UUID: {}", boost::uuids::to_string(uuid));
    std::println("Module primary locale: {}", boost::nowide::narrow(primary_locale));

    for (const auto& string_entity : module_data.strings)
    {
        std::println("String entity: id={}, locale={}, value={}", boost::nowide::narrow(string_entity.id), boost::nowide::narrow(string_entity.locale), boost::nowide::narrow(string_entity.value));
    }

    for (const auto& file_entity : module_data.files)
    {
        std::println("File entity: id={}, locale={}, path={}", boost::nowide::narrow(file_entity.id), boost::nowide::narrow(file_entity.locale), boost::nowide::narrow(file_entity.path));
    }

    if (!validate_module_data(module_data))
    {
        return 1;
    }

    // Parse cache file if exist.
    // We need entity ID -> numeric ID mapping to be stable across runs, so we will reuse IDs from previous cache if possible.
    std::unordered_map<std::u16string, uint32_t> entity_id_mapping;
    if (!cache_path.empty())
    {
        if (!parse_cache_file(cache_path, entity_id_mapping))
        {
            std::println("Warning: Failed to parse cache file: {}", cache_path);
        }
    }

    // Generate numeric IDs for entities and save to cache file, and also generate depfile and header file if paths are specified.

    return 0;
}

auto compile_resources(const std::vector<std::string>& cache_paths, const std::string& output_path) -> int
{
    for (const auto& cache_path : cache_paths)
    {
        std::println("Compiling resources from cache: {}", cache_path);
    }
    std::println("Output path: {}", output_path);
    return 0;
}

auto print_usage() -> void
{
    std::println("Usage:");
    std::println("  resource-compiler --help");
    std::println("  resource-compiler --version");
    std::println("  resource-compiler --analyze <source directory> [--cache PATH] [--depfile PATH] [--header PATH]");
    std::println("  resource-compiler --compile <cache-path>... --output PATH");
}

auto print_version() -> void
{
    std::println("Futurewalker Resource Compiler version 1.0");
}

auto run_analyze(const std::vector<std::string>& args) -> int
{
    auto source_directory = std::string();
    auto cache_path = std::string();
    auto depfile_path = std::string();
    auto header_path = std::string();

    auto desc = po::options_description("analyze options");

    desc.add_options()                                                                   //
      ("source-directory", po::value(&source_directory)->required(), "source directory") //
      ("cache", po::value(&cache_path), "cache path")                                    //
      ("depfile", po::value(&depfile_path), "depfile path")                              //
      ("header", po::value(&header_path), "header path");

    auto positional = po::positional_options_description();
    positional.add("source-directory", 1);

    auto vm = po::variables_map();
    try
    {
        po::store(po::command_line_parser(args).options(desc).positional(positional).run(), vm);
        po::notify(vm);
    }
    catch (const po::required_option&)
    {
        std::println("Error: Source directory not specified");
        return 1;
    }
    catch (const po::error& e)
    {
        std::println("Error: {}", e.what());
        return 1;
    }

    return analyze_resources(source_directory, cache_path, depfile_path, header_path);
}

auto run_compile(const std::vector<std::string>& args) -> int
{
    auto cache_paths = std::vector<std::string>();
    auto output_path = std::string();

    auto desc = po::options_description("compile options");

    desc.add_options()                                                //
      ("cache-path", po::value(&cache_paths), "cache paths")          //
      ("output", po::value(&output_path)->required(), "output path");

    auto positional = po::positional_options_description();
    positional.add("cache-path", -1);

    auto vm = po::variables_map();
    try
    {
        po::store(po::command_line_parser(args).options(desc).positional(positional).run(), vm);
        po::notify(vm);
    }
    catch (const po::required_option& e)
    {
        if (e.get_option_name() == "--output")
        {
            std::println("Error: Output path not specified");
        }
        else
        {
            std::println("Error: {}", e.what());
        }
        return 1;
    }
    catch (const po::error& e)
    {
        std::println("Error: {}", e.what());
        return 1;
    }

    if (cache_paths.empty())
    {
        std::println("Error: No cache paths specified");
        return 1;
    }

    return compile_resources(cache_paths, output_path);
}

} // namespace

auto main(int argc, char* argv[]) -> int
{
    xercesc::XMLPlatformUtils::Initialize();

    auto nowide = boost::nowide::args(argc, argv);

    if (argc < 2)
    {
        print_usage();
        return 1;
    }

    const auto command = std::string(argv[1]);
    const auto rest = std::vector<std::string>(argv + 2, argv + argc);

    if (command == "--help")
    {
        print_usage();
        return 1;
    }
    if (command == "--version")
    {
        print_version();
        return 0;
    }
    if (command == "--analyze")
    {
        return run_analyze(rest);
    }
    if (command == "--compile")
    {
        return run_compile(rest);
    }
    std::println("Unknown command: {}", command);
    return 1;
}
