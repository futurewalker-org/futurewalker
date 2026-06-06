#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include <boost/nowide/args.hpp>
#include <boost/program_options.hpp>
#include <boost/uuid.hpp>
#include <boost/variant2.hpp>
#include <boost/lexical_cast.hpp>

#include <filesystem>
#include <fstream>
#include <print>
#include <string>
#include <set>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
#include <ranges>
#include <iostream>
#include <map>
#include <variant>

#include <unicode/ustring.h>

namespace fs = std::filesystem;
namespace po = boost::program_options;

namespace
{
struct StringValue
{
    std::u16string locale; /// Locale of the entity, in IETF BCP 47 format (e.g., "en-US", "ja-JP").
    std::u16string value;  /// Value of the string entity.
};

struct FileValue
{
    std::u16string locale; /// Locale of the entity, in IETF BCP 47 format (e.g., "en-US", "ja-JP").
    std::u16string path;   /// Path to the file for the file entity.
};

struct StringEntity
{
    std::u16string id;   /// Identifier of the entity. Must be dot-separated string (e.g. "HelloWorld.MainWindow.Title").
    uint32_t number = 0; /// Resource number. Used as identifier to query entity from resource bundle.
    std::vector<StringValue> values;
};

struct FileEntity
{
    std::u16string id;   /// Identifier of the entity. Must be dot-separated string (e.g. "HelloWorld.MainWindow.Title").
    uint32_t number = 0; /// Resource number. Used as identifier to query entity from resource bundle.
    std::vector<FileValue> values;
};

struct ModuleData
{
    std::u16string name;                            /// Display name of the module.
    boost::uuids::uuid uuid;                        /// Unique identifier of the module, in UUID format.
    std::u16string primary_locale;                  /// Primary locale of the module, in IETF BCP 47 format (e.g., "en-US", "ja-JP").
    std::set<std::u16string> locales;               /// List of supported locales in the module, in IETF BCP 47 format (e.g., "en-US", "ja-JP").
    std::map<std::u16string, StringEntity> strings; /// List of string entities in the module.
    std::map<std::u16string, FileEntity> files;     /// List of file entities in the module
};

template <class T>
auto stream_write(std::ofstream& stream, T const& value)
{
    stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

auto string_to_xml(std::u16string const& str) -> XMLCh const*
{
    return reinterpret_cast<XMLCh const*>(str.c_str());
}

auto xml_to_string(XMLCh const* xml_ch) -> std::u16string
{
    return std::u16string(reinterpret_cast<char16_t const*>(xml_ch));
}

auto string_to_utf8(std::u16string const& str) -> std::string
{
    auto src = str.data();
    auto srcLength = static_cast<int32_t>(str.size());
    auto ec = UErrorCode();
    auto dstLength = int32_t(0);
    u_strToUTF8(nullptr, 0, &dstLength, src, srcLength, &ec);
    ec = U_ZERO_ERROR;
    auto dst = std::string();
    dst.resize(static_cast<size_t>(dstLength));
    u_strToUTF8(dst.data(), dstLength, nullptr, src, srcLength, &ec);
    return dst;
}

auto utf8_to_string(std::string const& str) -> std::u16string
{
    auto src = str.data();
    auto srcLength = static_cast<int32_t>(str.size());
    auto ec = UErrorCode();
    auto dstLength = int32_t(0);
    u_strFromUTF8(nullptr, 0, &dstLength, src, srcLength, &ec);
    ec = U_ZERO_ERROR;
    auto dst = std::u16string();
    dst.resize(static_cast<size_t>(dstLength));
    u_strFromUTF8(dst.data(), dstLength, nullptr, src, srcLength, &ec);
    return dst;
}

auto xml_to_utf8(XMLCh const* xml_ch) -> std::string
{
    return string_to_utf8(xml_to_string(xml_ch));
}

auto string_to_uuid(std::u16string const& str) -> boost::uuids::uuid
{
    try
    {
        std::string str_utf8 = string_to_utf8(str);
        return boost::uuids::string_generator()(str_utf8);
    }
    catch (const std::exception& e)
    {
        return boost::uuids::uuid();
    }
}

auto string_to_unsigned_int(std::u16string const& str) -> uint32_t
{
    try
    {
        auto utf8 = string_to_utf8(str);
        return static_cast<uint32_t>(std::stoul(utf8));
    }
    catch (...)
    {
        return 0;
    }
}

auto is_alpha = [](char16_t ch) -> bool { return (u'a' <= ch && ch <= u'z') || (u'A' <= ch && ch <= u'Z'); };
auto is_upper_alpha = [](char16_t ch) -> bool { return (u'A' <= ch && ch <= u'Z'); };
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

auto validate_locale(const std::u16string& locale, bool allow_empty) -> bool
{
    if (allow_empty && locale.empty())
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

    if (language.size() != 2 || !std::all_of(language.begin(), language.end(), is_alpha))
    {
        return false;
    }

    if (region.size() != 2 || !std::all_of(region.begin(), region.end(), is_upper_alpha))
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
    catch (const xercesc::SAXException& e)
    {
        std::println("Error: XML parse error in {}: {}", module_xml_path.string(), xml_to_utf8(e.getMessage()));
        return false;
    }
    catch (...)
    {
        std::println("Error: Unknown exception during parsing XML file {}", module_xml_path.string());
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
        std::println("Error: Root element must be <module>, got <{}>", string_to_utf8(root_name));
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
        std::println("Error: <module> has invalid 'uuid' attribute: {}", string_to_utf8(module_uuid_str));
        return false;
    }

    if (!validate_locale(module_primary_locale, false))
    {
        std::println("Error: <module> has invalid 'primary-locale' attribute: {}", string_to_utf8(module_primary_locale));
        return false;
    }

    name = std::move(module_name);
    uuid = std::move(module_uuid);
    primary_locale = std::move(module_primary_locale);
    return true;
}

auto parse_entity_base(xercesc::DOMElement const* element, std::u16string const& prefix) -> std::optional<std::u16string>
{
    auto id = xml_to_string(element->getAttribute(string_to_xml(u"id")));
    if (!validate_entity_id(id))
    {
        std::println("Error: Invalid entity id: {}", string_to_utf8(id));
        return std::nullopt;
    }

    if (!prefix.empty())
    {
        id = prefix + u"." + id;
    }
    return id;
}

auto parse_entity(
  xercesc::DOMElement const* element,
  fs::path const& base_path,
  std::u16string const& prefix,
  std::u16string const& locale,
  std::map<std::u16string, StringEntity>& string_entities,
  std::map<std::u16string, FileEntity>& file_entities,
  std::vector<fs::path>& dependent_files) -> bool
{
    auto const tag_name = xml_to_string(element->getTagName());
    if (tag_name == u"string")
    {
        auto entity_base = parse_entity_base(element, prefix);
        if (!entity_base)
        {
            return false;
        }

        auto id = std::move(*entity_base);
        auto value = xml_to_string(element->getTextContent());

        string_entities[id].values.push_back(
          StringValue {
              .locale = locale,
              .value = std::move(value),
          });
    }
    else if (tag_name == u"file")
    {
        auto entity_base = parse_entity_base(element, prefix);
        if (!entity_base)
        {
            return false;
        }
        auto id = std::move(*entity_base);
        auto path = fs::path(xml_to_string(element->getTextContent()));

        if (path.is_relative())
        {
            path = base_path / path;
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

        file_entities[id].values.push_back(
          FileValue {
              .locale = locale,
              .path = path.u16string(),
          });

        dependent_files.push_back(path);
    }
    return true;
}

auto parse_resource_file(
  const fs::path& resource_xml_path,
  std::u16string const& primary_locale,
  std::set<std::u16string>& locales,
  std::map<std::u16string, StringEntity>& string_entities,
  std::map<std::u16string, FileEntity>& file_entities,
  std::vector<fs::path>& dependent_files) -> bool
{
    std::println("Parsing resource file: {}", resource_xml_path.string());

    auto handler = xercesc::HandlerBase();
    auto parser = xercesc::XercesDOMParser();
    parser.setValidationScheme(xercesc::XercesDOMParser::Val_Never);
    parser.setDoNamespaces(false);
    parser.setDoSchema(false);
    parser.setErrorHandler(&handler);

    try
    {
        parser.parse(resource_xml_path.string().c_str());
    }
    catch (const xercesc::SAXException& e)
    {
        std::println("Error: XML parse error in {}: {}", resource_xml_path.string(), xml_to_utf8(e.getMessage()));
        return false;
    }
    catch (...)
    {
        std::println("Error: Unknown exception during parsing XML file {}", resource_xml_path.string());
        return false;
    }

    dependent_files.push_back(resource_xml_path);

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
        std::println("Error: Root element must be <resources>, got <{}>", xml_to_utf8(root->getTagName()));
        return false;
    }

    auto const prefix = xml_to_string(root->getAttribute(string_to_xml(u"prefix")));
    if (!prefix.empty() && !validate_entity_id(prefix))
    {
        std::println("Error: Invalid prefix: {}", string_to_utf8(prefix));
        return false;
    }

    auto const locale = xml_to_string(root->getAttribute(string_to_xml(u"locale")));
    if (!locale.empty() && !validate_locale(locale, true))
    {
        std::println("Error: Invalid locale: {}", string_to_utf8(locale));
        return false;
    }
    locales.insert(locale);

    auto base_path = resource_xml_path.parent_path();

    auto child_element = root->getFirstElementChild();
    while (child_element)
    {
        if (!parse_entity(child_element, base_path, prefix, locale, string_entities, file_entities, dependent_files))
        {
            return false;
        }
        child_element = child_element->getNextElementSibling();
    }
    return true;
}

auto find_child_elements(xercesc::DOMElement* element, std::u16string_view const& tag, std::vector<xercesc::DOMElement*>& elements)
{
    auto child = element->getFirstElementChild();
    while (child)
    {
        auto child_name = xml_to_string(child->getTagName());
        if (child_name == tag)
        {
            elements.push_back(child);
        }
        child = child->getNextElementSibling();
    }
}

auto parse_cache_file(fs::path const& cache_xml_path, ModuleData& module_data) -> bool
{
    std::println("Parsing cache file: {}", cache_xml_path.string());

    auto handler = xercesc::HandlerBase();
    auto parser = xercesc::XercesDOMParser();
    parser.setValidationScheme(xercesc::XercesDOMParser::Val_Never);
    parser.setDoNamespaces(false);
    parser.setDoSchema(false);
    parser.setErrorHandler(&handler);

    try
    {
        parser.parse(cache_xml_path.string().c_str());
    }
    catch (const xercesc::SAXException& e)
    {
        std::println("Error: XML parse error in {}: {}", cache_xml_path.string(), xml_to_utf8(e.getMessage()));
        return false;
    }
    catch (...)
    {
        std::println("Error: Unknown exception during parsing XML file {}", cache_xml_path.string());
        return false;
    }

    auto document = parser.getDocument();
    auto root = document->getDocumentElement();

    auto root_name = xml_to_string(root->getTagName());
    if (root_name != u"module")
    {
        std::println("Error: Root element must be <module>, got <{}>", string_to_utf8(root_name));
        return false;
    }

    auto module_name = xml_to_string(root->getAttribute(string_to_xml(u"name")));
    auto module_uuid_str = xml_to_string(root->getAttribute(string_to_xml(u"uuid")));
    auto module_primary_locale = xml_to_string(root->getAttribute(string_to_xml(u"primary-locale")));

    module_data.name = module_name;
    module_data.uuid = string_to_uuid(module_uuid_str);
    module_data.primary_locale = module_primary_locale;

    auto locales_elements = std::vector<xercesc::DOMElement*>();
    find_child_elements(root, u"locales", locales_elements);

    for (auto const& locales_element : locales_elements)
    {
        auto locale_elements = std::vector<xercesc::DOMElement*>();
        find_child_elements(locales_element, u"locale", locale_elements);

        for (auto const& locale_element : locale_elements)
        {
            auto locale = xml_to_string(locale_element->getTextContent());
            module_data.locales.insert(locale);
        }
    }

    auto resources_elements = std::vector<xercesc::DOMElement*>();
    find_child_elements(root, u"resources", resources_elements);

    for (auto const& resources_element : resources_elements)
    {
        auto resource_elements = std::vector<xercesc::DOMElement*>();
        find_child_elements(resources_element, u"resource", resource_elements);

        for (auto const& resource_element : resource_elements)
        {
            auto id = xml_to_string(resource_element->getAttribute(string_to_xml(u"id")));
            auto number = xml_to_string(resource_element->getAttribute(string_to_xml(u"number")));
            auto type = xml_to_string(resource_element->getAttribute(string_to_xml(u"type")));

            auto value_elements = std::vector<xercesc::DOMElement*>();
            find_child_elements(resource_element, type, value_elements);

            if (type == u"string")
            {
                auto entity = StringEntity();
                entity.id = id;
                entity.number = string_to_unsigned_int(number);
                for (auto const& value_element : value_elements)
                {
                    auto value = xml_to_string(value_element->getTextContent());
                    auto locale = xml_to_string(value_element->getAttribute(string_to_xml(u"locale")));
                    auto string_value = StringValue();
                    string_value.value = value;
                    string_value.locale = locale;
                    entity.values.push_back(string_value);
                }
                module_data.strings.emplace(id, entity);
            }
            else if (type == u"file")
            {
                auto entity = FileEntity();
                entity.id = id;
                entity.number = string_to_unsigned_int(number);
                for (auto const& value_element : value_elements)
                {
                    auto path = xml_to_string(value_element->getTextContent());
                    auto locale = xml_to_string(value_element->getAttribute(string_to_xml(u"locale")));
                    auto file_value = FileValue();
                    file_value.path = path;
                    file_value.locale = locale;
                    entity.values.push_back(file_value);
                }
                module_data.files.emplace(id, entity);
            }
        }
    }

    return true;
}

auto assign_entity_numbers(ModuleData& module_data, std::map<std::u16string, uint32_t> const& entity_number_mapping)
{
    auto result = std::map<std::u16string, uint32_t>();

    auto max_id = 0u;
    for (auto const& [id, entity] : module_data.strings)
    {
        max_id = std::max(max_id, entity.number);
    }

    for (auto& [id, entity] : module_data.strings)
    {
        auto const it = entity_number_mapping.find(id);
        if (it != entity_number_mapping.end())
        {
            entity.number = it->second;
        }
        else
        {
            entity.number = ++max_id;
        }
    }

    for (auto& [id, entity] : module_data.files)
    {
        auto const it = entity_number_mapping.find(id);
        if (it != entity_number_mapping.end())
        {
            entity.number = it->second;
        }
        else
        {
            entity.number = ++max_id;
        }
    }
    return result;
}

auto validate_module_data(const ModuleData& module_data) -> bool
{
    // Validate that entities with same id have unique locale and a consistent type.
    auto seen = std::multimap<std::u16string, std::u16string>();
    auto id_type = std::unordered_map<std::u16string, std::string_view>();

    auto check = [&seen, &id_type](std::u16string const& id, std::u16string const& locale, std::string_view type) -> bool
    {
        auto [it, inserted] = id_type.try_emplace(id, type);
        if (!inserted && it->second != type)
        {
            std::println("Error: Entity id '{}' is used as both '{}' and '{}'", string_to_utf8(id), it->second, type);
            return false;
        }

        auto [begin, end] = seen.equal_range(id);
        if (std::find_if(begin, end, [&locale](const auto& pair) { return pair.second == locale; }) != end)
        {
            std::println("Error: Duplicate entity id '{}' for locale '{}'", string_to_utf8(id), string_to_utf8(locale));
            return false;
        }
        seen.emplace(id, locale);

        return true;
    };

    for (auto const& [id, entity] : module_data.strings)
    {
        for (auto const& value : entity.values)
        {
            if (!check(id, value.locale, "string"))
            {
                return false;
            }
        }
    }
    for (auto const& [id, entity] : module_data.files)
    {
        for (auto const& value : entity.values)
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
        auto upper_bound = seen.upper_bound(it->first);
        if (std::find(it, upper_bound, std::make_pair(it->first, module_data.primary_locale)) == upper_bound)
        {
            std::println("Error: Entity id '{}' is missing for primary locale '{}'", string_to_utf8(it->first), string_to_utf8(module_data.primary_locale));
            return false;
        }
        it = upper_bound;
    }
    return true;
}

template <class DOMObjectType>
using unique_dom_ptr = std::unique_ptr<DOMObjectType, decltype([](DOMObjectType* obj) { if (obj) { obj->release(); } })>;

template <class DOMObjectType>
auto wrap_dom_ptr(DOMObjectType* ptr)
{
    return unique_dom_ptr<DOMObjectType>(ptr);
}

auto write_cache_file(fs::path const& cache_xml_path, ModuleData const& module_data) -> bool
{
    std::println("write_cache_file: {}", cache_xml_path.string());

    try
    {
        fs::create_directories(cache_xml_path.parent_path());
    }
    catch (...)
    {
        std::println("Failed to create directories for path {}", cache_xml_path.string());
    }

    auto impl = xercesc::DOMImplementation::getImplementation();

    auto doc = wrap_dom_ptr(impl->createDocument(nullptr, string_to_xml(u"module"), nullptr));

    auto const uuid_string = utf8_to_string(to_string(module_data.uuid));
    auto const root_node = doc->getDocumentElement();
    root_node->setAttribute(string_to_xml(u"name"), string_to_xml(module_data.name));
    root_node->setAttribute(string_to_xml(u"uuid"), string_to_xml(uuid_string));
    root_node->setAttribute(string_to_xml(u"primary-locale"), string_to_xml(module_data.primary_locale));

    auto locales_node = doc->createElement(string_to_xml(u"locales"));
    for (auto const& locale : module_data.locales)
    {
        auto locale_node = doc->createElement(string_to_xml(u"locale"));
        locale_node->setTextContent(string_to_xml(locale));
        locales_node->appendChild(locale_node);
    }
    root_node->appendChild(locales_node);

    auto resources_node = doc->createElement(string_to_xml(u"resources"));

    for (auto const& [id, entity] : module_data.strings)
    {
        auto const entity_node = doc->createElement(string_to_xml(u"resource"));
        auto const number_string = utf8_to_string(std::to_string(entity.number));
        entity_node->setAttribute(string_to_xml(u"id"), string_to_xml(id));
        entity_node->setAttribute(string_to_xml(u"number"), string_to_xml(number_string));
        entity_node->setAttribute(string_to_xml(u"type"), string_to_xml(u"string"));
        for (auto const& value : entity.values)
        {
            auto const string_node = doc->createElement(string_to_xml(u"string"));
            string_node->setTextContent(string_to_xml(value.value));
            string_node->setAttribute(string_to_xml(u"locale"), string_to_xml(value.locale));
            entity_node->appendChild(string_node);
        }
        resources_node->appendChild(entity_node);
    }

    for (auto const& [id, entity] : module_data.files)
    {
        auto const entity_node = doc->createElement(string_to_xml(u"resource"));
        auto const number_string = utf8_to_string(std::to_string(entity.number));
        entity_node->setAttribute(string_to_xml(u"id"), string_to_xml(id));
        entity_node->setAttribute(string_to_xml(u"number"), string_to_xml(number_string));
        entity_node->setAttribute(string_to_xml(u"type"), string_to_xml(u"file"));
        for (auto const& value : entity.values)
        {
            auto const string_node = doc->createElement(string_to_xml(u"file"));
            string_node->setTextContent(string_to_xml(value.path));
            string_node->setAttribute(string_to_xml(u"locale"), string_to_xml(value.locale));
            entity_node->appendChild(string_node);
        }
        resources_node->appendChild(entity_node);
    }

    root_node->appendChild(resources_node);

    auto fileTarget = xercesc::LocalFileFormatTarget(cache_xml_path.string().c_str());
    auto output = wrap_dom_ptr(impl->createLSOutput());
    output->setByteStream(&fileTarget);
    auto serializer = wrap_dom_ptr(impl->createLSSerializer());
    serializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);
    serializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTXercesPrettyPrint, false);
    serializer->write(doc.get(), output.get());
    return true;
}

auto write_depfile(fs::path const& depfile_path, fs::path const& cache_xml_path, std::vector<fs::path> const& generated_files, std::vector<fs::path> const& dependent_files) -> bool
{
    std::println("write_depfile: {}", depfile_path.string());

    try
    {
        fs::create_directories(depfile_path.parent_path());
    }
    catch (...)
    {
        std::println("Failed to create directories for path {}", cache_xml_path.string());
    }

    std::ofstream depfile(depfile_path);
    if (!depfile)
    {
        std::println("Error: Failed to open depfile for writing: {}", depfile_path.string());
        return false;
    }

    for (auto const& gen : generated_files)
    {
        depfile << gen.string();
        depfile << " ";
    }
    depfile << ":";
    for (auto const& dep : dependent_files)
    {
        depfile << " ";
        depfile << dep.string();
    }
    depfile << "\n";

    return true;
}

struct Tree
{
    uint32_t number = 0;
    boost::uuids::uuid uuid;
    std::map<std::u16string, std::unique_ptr<Tree>> nodes;

    auto is_leaf() const -> bool
    {
        return nodes.empty();
    }

    auto has_leaf() const -> bool
    {
        for (auto const& [_, node] : nodes)
        {
            if (node->is_leaf())
            {
                return true;
            }
        }
        return false;
    }

    auto has_uuid() const
    {
        return !uuid.is_nil();
    }

    auto node_at(std::u16string const& key) -> Tree&
    {
        auto& node = nodes[key];
        if (!node)
        {
            node = std::make_unique<Tree>();
        }
        return *node;
    }
};

auto insert_module_node(Tree& tree, std::u16string const& id, boost::uuids::uuid const& uuid)
{
    auto const components = id | std::ranges::views::split(u'.') | std::ranges::to<std::vector<std::u16string>>();
    auto tree_node = &tree;
    for (auto const& component : components)
    {
        tree_node = &tree_node->node_at(component);
    }
    tree_node->uuid = uuid;
}

auto insert_entity_node(Tree& tree, std::u16string const& id, uint32_t number)
{
    auto const components = id | std::ranges::views::split(u'.') | std::ranges::to<std::vector<std::u16string>>();
    auto tree_node = &tree;
    for (auto const& component : components)
    {
        tree_node = &tree_node->node_at(component);
    }
    tree_node->number = number;
}

auto enumerate_node_with_leaves(Tree& tree, std::vector<std::u16string>& path_stack, auto f) -> void
{
    if (tree.has_leaf() || tree.has_uuid())
    {
        f(tree, path_stack);
    }

    for (auto const& [key, node] : tree.nodes)
    {
        path_stack.push_back(key);
        enumerate_node_with_leaves(*node, path_stack, f);
        path_stack.pop_back();
    }
}

auto concat_path_stack(std::vector<std::u16string> const& path_stack, std::string_view delim)
{
    auto path = std::string();
    for (auto const& p : path_stack)
    {
        if (!path.empty())
        {
            path += delim;
        }
        path += string_to_utf8(p);
    }
    return path;
}

auto write_header_file(fs::path const& header_path, ModuleData const& module_data, std::vector<fs::path>& generated_files) -> bool
{
    try
    {
        fs::create_directories(header_path);
    }
    catch (...)
    {
        std::println("Failed to create header path directory at {}", header_path.string());
        return false;
    }

    auto tree = Tree();

    insert_module_node(tree, module_data.name, module_data.uuid);

    for (auto const& [id, entity] : module_data.strings)
    {
        insert_entity_node(tree, id, entity.number);
    }
    for (auto const& [id, entity] : module_data.files)
    {
        insert_entity_node(tree, id, entity.number);
    }

    auto result = true;
    auto path_stack = std::vector<std::u16string>();
    enumerate_node_with_leaves(tree, path_stack, [&](Tree const& tree, std::vector<std::u16string> const& path_stack) {
        auto const namespace_path = concat_path_stack(path_stack, "::");
        auto filename_path = concat_path_stack(path_stack, ".");
        if (filename_path.empty())
        {
            filename_path = string_to_utf8(module_data.name);
        }

        auto const header_file_path = header_path / (filename_path + ".hpp");
        {
            std::ofstream header_file(header_file_path);
            if (!header_file)
            {
                std::println("Error: Failed to open header file for writing: {}", header_file_path.string());
                result = false;
                return;
            }

            header_file << "#include <Futurewalker.Core.Uuid.hpp>\n\n";

            if (!tree.uuid.is_nil())
            {
                auto module_path_stack = path_stack;
                if (!module_path_stack.empty())
                {
                    auto module_id = module_path_stack.back();
                    module_path_stack.pop_back();
                    auto const module_namespace_path = concat_path_stack(module_path_stack, "::");

                    header_file << "namespace Futurewalker::M";
                    if (module_namespace_path.empty())
                    {
                        header_file << "\n";
                    }
                    else
                    {
                        header_file << "::" << module_namespace_path << "\n";
                    }
                    header_file << "{\n";
                    header_file << "inline constexpr auto " << string_to_utf8(module_id) << " = Uuid {";
                    for (int i = 0; i < 16; ++i)
                    {
                        auto const val = tree.uuid.data()[i];
                        header_file << (int)val << ",";
                    }
                    header_file << "};\n";
                    header_file << "}\n";
                }
            }

            header_file << "namespace Futurewalker::R";
            if (namespace_path.empty())
            {
                header_file << "\n";
            }
            else
            {
                header_file << "::" << namespace_path << "\n";
            }
            header_file << "{\n";
            for (auto const& [id, node] : tree.nodes)
            {
                if (node->is_leaf() && node->number != 0)
                {
                    header_file << "inline constexpr auto " << string_to_utf8(id) << " = UInt32(" << node->number << "U);\n";
                }
            }
            header_file << "}\n";
        }
        generated_files.push_back(header_file_path);
    });
    return result;
}

auto analyze_resources(const std::string& source_directory, const std::string& cache_xml_path, const std::string& depfile_path, const std::string& header_directory) -> int
{
    std::println("Analyzing resources in directory: {}", source_directory);
    std::println("Cache path: {}", cache_xml_path);
    std::println("Depfile path: {}", depfile_path);
    std::println("Header path: {}", header_directory);

    // Find .module.xml file in source_directory, or return error if not found or multiple found.
    auto source_path = fs::absolute(fs::path(source_directory)).lexically_normal();
    if (!fs::exists(source_path) || !fs::is_directory(source_path))
    {
        std::println("Error: Source directory does not exist or is not a directory: {}", source_directory);
        return 1;
    }

    auto module_files = std::vector<std::pair<fs::path, ModuleData>>();
    for (const auto& entry : fs::directory_iterator(source_path))
    {
        if (entry.is_regular_file() && entry.path().filename().string().ends_with(".xml"))
        {
            auto const path = entry.path();
            auto name = std::u16string();
            auto uuid = boost::uuids::uuid();
            auto primary_locale = std::u16string();
            if (parse_module_file(path, name, uuid, primary_locale))
            {
                auto module_data = ModuleData {
                    .name = std::move(name),
                    .uuid = uuid,
                    .primary_locale = std::move(primary_locale),
                    .strings = {},
                    .files = {},
                };
                module_files.push_back({path, std::move(module_data)});
            }
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
    auto& [module_xml_path, module_data] = module_files[0];

    // Parse other .xml files in source_directory to collect resource entities.
    auto dependent_files = std::vector<fs::path>();
    auto generated_files = std::vector<fs::path>();
    dependent_files.push_back(module_xml_path);
    generated_files.push_back(cache_xml_path);

    for (auto const& entry : fs::recursive_directory_iterator(source_path))
    {
        if (entry.is_regular_file() && entry.path().filename().string().ends_with(".xml"))
        {
            if (entry.path() == module_xml_path)
            {
                continue;
            }

            if (!parse_resource_file(entry.path(), module_data.primary_locale, module_data.locales, module_data.strings, module_data.files, dependent_files))
            {
                std::println("Error: Failed to parse resource file: {}", entry.path().string());
                return 1;
            }
        }
    }

    std::println("Finished analyzing resources for module: {}", string_to_utf8(module_data.name));
    std::println("Module name: {}", string_to_utf8(module_data.name));
    std::println("Module UUID: {}", boost::uuids::to_string(module_data.uuid));
    std::println("Module primary locale: {}", string_to_utf8(module_data.primary_locale));

    for (const auto& locale : module_data.locales)
    {
        std::println("Locale: {}", string_to_utf8(locale));
    }

    for (const auto& [id, entity] : module_data.strings)
    {
        for (auto const& value : entity.values)
        {
            std::println("String entity: id={}", string_to_utf8(id), string_to_utf8(value.locale), string_to_utf8(value.value));
        }
    }

    for (const auto& [id, entity] : module_data.files)
    {
        for (auto const& value : entity.values)
        {
            std::println("File entity: id={}, locale={}, path={}", string_to_utf8(id), string_to_utf8(value.locale), string_to_utf8(value.path));
        }
    }

    if (!validate_module_data(module_data))
    {
        return 1;
    }

    // Parse cache file if exist.
    // We need entity ID -> numeric ID mapping to be stable across runs, so we will reuse IDs from previous cache if possible.
    std::map<std::u16string, uint32_t> entity_number_mapping;
    if (!cache_xml_path.empty())
    {
        if (!cache_xml_path.ends_with(".cache.xml"))
        {
            std::println("Error: Failed to load cache file at {}. Filename of cache files should be *.cache.xml", cache_xml_path);
            return 1;
        }

        if (fs::exists(cache_xml_path))
        {
            if (fs::is_regular_file(cache_xml_path))
            {
                auto cache_module_data = ModuleData();
                if (parse_cache_file(cache_xml_path, cache_module_data))
                {
                    for (auto const& [id, entity] : cache_module_data.strings)
                    {
                        entity_number_mapping[id] = entity.number;
                    }
                    for (auto const& [id, entity] : cache_module_data.files)
                    {
                        entity_number_mapping[id] = entity.number;
                    }
                }
                else
                {
                    std::println("Warning: Failed to parse cache file: {}", cache_xml_path);
                    entity_number_mapping = {};
                }
            }
            else
            {
                std::println("Error: Failed to load cache file at {}. Cache path is not a regular file", cache_xml_path);
                return 1;
            }
        }
    }

    assign_entity_numbers(module_data, entity_number_mapping);

    if (!cache_xml_path.empty())
    {
        if (!write_cache_file(cache_xml_path, module_data))
        {
            std::println("Error: Failed to write cache file at {}", cache_xml_path);
            return 1;
        }
    }

    if (!header_directory.empty())
    {
        if (!write_header_file(header_directory, module_data, generated_files))
        {
            std::println("Error: Failed to write headers file at directory {}", header_directory);
            return 1;
        }
    }

    if (!depfile_path.empty())
    {
        if (!write_depfile(depfile_path, cache_xml_path, generated_files, dependent_files))
        {
            std::println("Error: Failed to write depfile at {}", depfile_path);
            return 1;
        }
    }

    return 0;
}

struct BundleHeader
{
    uint8_t signature[4]; // "FWRB"
    uint32_t version;     // LE Version
    uint32_t options;     // LE Options
    uint32_t module_ptr;  // LE Offset of first module data chunk.
};

struct BundleModuleChunk
{
    uint32_t chunk_size;         // LE Chunk size.
    uint32_t name_ptr;           // LE offset of name string chunk.
    uint32_t primary_locale_ptr; // LE Offset of locale string chunk.
    uint8_t uuid[16];            // UUID of the module.
    uint32_t locales_ptr;        // LE offset of first locale chunk.
    uint32_t resources_ptr;      // LE offset of first resources chunk.
    uint32_t next_ptr;           // LE Offset of next module chunk.
};

struct BundleLocaleChunk
{
    uint32_t chunk_size; // LE Chunk size.
    uint32_t locale_ptr; // LE Offset of locale string chunk.
    uint32_t next_ptr;   // LE Offset of next locale chunk.
};

enum class BundleResourcesChunkType : uint32_t
{
    String = 1,
    File = 2,
};

struct BundleResourcesChunk
{
    uint32_t chunk_size;   // LE Chunk size.
    uint32_t id_ptr;       // LE Offset of ID string chunk.
    uint32_t number;       // LE Resource number.
    uint32_t type;         // LE Resource type.
    uint32_t resource_ptr; // LE Offset of first resource data chunk.
    uint32_t next_ptr;     // LE Offset of next resources chunk.
};

struct BundleStringResourceChunk
{
    uint32_t chunk_size; // LE Chunk size.
    uint32_t locale_ptr; // LE Offset of locale string chunk.
    uint32_t value_ptr;  // LE Offset of value string chunk.
    uint32_t next_ptr;   // LE Offset of next resource data chunk.
};

struct BundleFileResourceChunk
{
    uint32_t chunk_size; // LE Chunk size.
    uint32_t locale_ptr; // LE Offset of locale string chunk.
    uint32_t data_ptr;   // LE Offset of data chunk.
    uint32_t next_ptr;   // LE Offset of next resource data chunk.
};

struct BundleDataChunk
{
    uint32_t chunk_size; // LE Chunk size.
    uint8_t data[];      // Variable length data.
};

auto write_string(std::ofstream& stream, std::u16string const& string)
{
    auto p = stream.tellp();
    auto const utf8 = string_to_utf8(string);
    stream_write(stream, uint32_t(4 + utf8.length())); // chunk_size
    stream.write(reinterpret_cast<char const*>(utf8.data()), utf8.length()); // data
    return static_cast<uint32_t>(p);
}

auto write_file(std::ofstream& stream, fs::path const& file_path) -> uint32_t
{
    auto p = stream.tellp();
    auto file = std::ifstream(file_path, std::ios_base::binary);
    file.seekg(0, std::ios_base::end);
    auto const file_size = static_cast<uint32_t>(file.tellg());
    file.seekg(0, std::ios_base::beg);
    stream_write(stream, uint32_t(4 + file_size));
    stream << file.rdbuf();
    return static_cast<uint32_t>(p);
}

using EntityVariant = std::variant<StringEntity, FileEntity>;

auto get_bundle_resource_type(EntityVariant const& entity) -> BundleResourcesChunkType
{
    if (std::holds_alternative<StringEntity>(entity))
    {
        return BundleResourcesChunkType::String;
    }
    else if (std::holds_alternative<FileEntity>(entity))
    {
        return BundleResourcesChunkType::File;
    }
    assert(false);
    return {};
}

auto write_resource_value(std::ofstream& stream, size_t i, std::vector<StringValue> const& values) -> uint32_t
{
    if (i >= values.size())
    {
        return 0;
    }

    auto const& value = values[i];
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleStringResourceChunk), std::ios_base::cur);
    auto next_ptr = write_resource_value(stream, i + 1, values);
    auto locale_ptr = write_string(stream, value.locale);
    auto value_ptr = write_string(stream, value.value);
    auto q = stream.tellp();
    stream.seekp(p);
    stream_write(stream, uint32_t(sizeof(BundleStringResourceChunk)));
    stream_write(stream, locale_ptr);
    stream_write(stream, value_ptr);
    stream_write(stream, next_ptr);
    stream.seekp(q);
    return static_cast<uint32_t>(p);
}

auto write_resource_value(std::ofstream& stream, size_t i, std::vector<FileValue> const& values) -> uint32_t
{
    if (i >= values.size())
    {
        return 0;
    }

    auto const& value = values[i];
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleStringResourceChunk), std::ios_base::cur);
    auto next_ptr = write_resource_value(stream, i + 1, values);
    auto locale_ptr = write_string(stream, value.locale);
    auto value_ptr = write_file(stream, value.path);
    auto q = stream.tellp();
    stream.seekp(p);
    stream_write(stream, uint32_t(sizeof(BundleStringResourceChunk)));
    stream_write(stream, locale_ptr);
    stream_write(stream, value_ptr);
    stream_write(stream, next_ptr);
    stream.seekp(q);
    return static_cast<uint32_t>(p);
}

auto write_resources_entities(std::ofstream& stream, size_t index, std::vector<EntityVariant> const& entities) -> uint32_t
{
    if (index >= entities.size())
    {
        return 0;
    }

    auto const& entity = entities[index];
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleResourcesChunk), std::ios_base::cur);
    auto next_ptr = write_resources_entities(stream, index + 1, entities);
    auto resource_ptr = std::visit([&](auto const& e) { return write_resource_value(stream, 0, e.values); }, entity);
    auto id_ptr = write_string(stream, std::visit([](auto const& e) { return e.id; }, entity));
    auto q = stream.tellp();
    stream.seekp(p);
    stream_write(stream, uint32_t(sizeof(BundleResourcesChunk)));
    stream_write(stream, id_ptr);
    stream_write(stream, std::visit([](auto const& e) { return e.number; }, entity));
    stream_write(stream, uint32_t(get_bundle_resource_type(entity)));
    stream_write(stream, resource_ptr);
    stream_write(stream, next_ptr);
    stream.seekp(q);
    return static_cast<uint32_t>(p);
}

auto write_resources(std::ofstream& stream, ModuleData const& module_data) -> uint32_t
{
    std::vector<EntityVariant> entities;
    entities.reserve(module_data.strings.size() + module_data.files.size());
    for (auto const& [id, entity] : module_data.strings)
    {
        entities.emplace_back(entity);
    }
    for (auto const& [id, entity] : module_data.files)
    {
        entities.emplace_back(entity);
    }
    return write_resources_entities(stream, 0, entities);
}

auto write_bundle_locale(std::ofstream& stream, size_t index, std::vector<std::u16string> const& locales) -> uint32_t
{
    if (index >= locales.size())
    {
        return 0;
    }

    auto const& locale = locales[index];
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleLocaleChunk), std::ios_base::cur);
    auto next_ptr = write_bundle_locale(stream, index + 1, locales);
    auto locale_ptr = write_string(stream, locale);
    auto q = stream.tellp();
    stream.seekp(p);
    stream_write(stream, uint32_t(sizeof(BundleLocaleChunk)));
    stream_write(stream, locale_ptr);
    stream_write(stream, next_ptr);
    stream.seekp(q);
    return static_cast<uint32_t>(p);
}

auto write_bundle_locales(std::ofstream& stream, ModuleData const& module_data) -> uint32_t
{
    std::vector<std::u16string> locales(module_data.locales.begin(), module_data.locales.end());
    return write_bundle_locale(stream, 0, locales);
}

auto write_bundle_module(std::ofstream& stream, size_t index, std::vector<ModuleData> const& module_data_list) -> uint32_t
{
    if (index >= module_data_list.size())
    {
        return 0;
    }

    auto const& module_data = module_data_list[index];
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleModuleChunk), std::ios_base::cur);
    auto next_ptr = write_bundle_module(stream, index + 1, module_data_list);
    auto locales_ptr = write_bundle_locales(stream, module_data);
    auto resources_ptr = write_resources(stream, module_data);
    auto name_ptr = write_string(stream, module_data.name);
    auto primary_locale_ptr = write_string(stream, module_data.primary_locale);
    auto q = stream.tellp();
    stream.seekp(p);
    stream_write(stream, uint32_t(sizeof(BundleModuleChunk)));
    stream_write(stream, name_ptr);
    stream_write(stream, primary_locale_ptr);
    for (auto const& data : module_data.uuid)
    {
        stream_write(stream, data);
    }
    stream_write(stream, locales_ptr);
    stream_write(stream, resources_ptr);
    stream_write(stream, next_ptr);
    stream.seekp(q);
    return static_cast<uint32_t>(p);
}

auto write_bundle_modules(std::ofstream& stream, std::vector<ModuleData> const& module_data_list) -> uint32_t
{
    return write_bundle_module(stream, 0, module_data_list);
}

auto write_bundle_header(std::ofstream& stream, std::vector<ModuleData> const& module_data_list)
{
    auto p = stream.tellp();
    stream.seekp(sizeof(BundleHeader), std::ios_base::cur);
    auto const modules_ptr = write_bundle_modules(stream, module_data_list);
    stream.seekp(p);
    stream.write("FWRB", 4);
    stream_write(stream, uint32_t(1));
    stream_write(stream, uint32_t(0));
    stream_write(stream, modules_ptr);
}

auto compile_resources(std::vector<std::string> cache_paths, const std::filesystem::path& output_path) -> int
{
    cache_paths.erase(std::unique(cache_paths.begin(), cache_paths.end()), cache_paths.end());

    std::vector<ModuleData> module_data_list;
    for (const auto& cache_path : cache_paths)
    {
        auto module_data = ModuleData();
        if (parse_cache_file(cache_path, module_data))
        {
            std::println("Parsed module from cache: {}", string_to_utf8(module_data.name));
            module_data_list.push_back(std::move(module_data));
        }
    }
    std::println("Output path: {}", output_path.string());

    try
    {
        fs::create_directories(output_path.parent_path());
    }
    catch (...)
    {
        std::println("Failed to create directories for path {}", output_path.parent_path().string());
        return 1;
    }

    std::ofstream bundle_file(output_path, std::ios::binary);
    if (!bundle_file)
    {
        std::println("Failed to open output file: {}", output_path.string());
        return 1;
    }

    write_bundle_header(bundle_file, module_data_list);

    return 0;
}

auto print_usage() -> void
{
    std::println("Usage:");
    std::println("  resource-compiler --help");
    std::println("  resource-compiler --version");
    std::println("  resource-compiler --generate <source directory> [--cache PATH] [--depfile PATH] [--header PATH]");
    std::println("  resource-compiler --bundle <cache-path>... --output PATH");
}

auto print_version() -> void
{
    std::println("Futurewalker Resource Compiler version 1.0");
}

auto run_generate(const std::vector<std::string>& args) -> int
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

auto run_bundle(const std::vector<std::string>& args) -> int
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
    if (command == "--generate")
    {
        return run_generate(rest);
    }
    if (command == "--bundle")
    {
        return run_bundle(rest);
    }
    std::println("Unknown command: {}", command);
    return 1;
}
