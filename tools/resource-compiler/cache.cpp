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

#include <print>
#include <ranges>
#include <fstream>

namespace ResourceCompiler
{
namespace
{
auto FindChildElements(xercesc::DOMElement* element, std::u16string_view const& tag, std::vector<xercesc::DOMElement*>& elements)
{
    auto child = element->getFirstElementChild();
    while (child)
    {
        auto childName = XmlToString(child->getTagName());
        if (childName == tag)
        {
            elements.push_back(child);
        }
        child = child->getNextElementSibling();
    }
}
}

auto ParseCacheFile(fs::path const& cacheXmlPath, ModuleData& moduleData) -> bool
{
    std::println("Parsing cache file: {}", cacheXmlPath.string());

    auto handler = xercesc::HandlerBase();
    auto parser = xercesc::XercesDOMParser();
    parser.setValidationScheme(xercesc::XercesDOMParser::Val_Never);
    parser.setDoNamespaces(false);
    parser.setDoSchema(false);
    parser.setErrorHandler(&handler);

    try
    {
        parser.parse(cacheXmlPath.string().c_str());
    }
    catch (xercesc::SAXException const& e)
    {
        std::println("Error: XML parse error in {}: {}", cacheXmlPath.string(), XmlToUtf8(e.getMessage()));
        return false;
    }
    catch (...)
    {
        std::println("Error: Unknown exception during parsing XML file {}", cacheXmlPath.string());
        return false;
    }

    auto document = parser.getDocument();
    auto root = document->getDocumentElement();

    auto rootName = XmlToString(root->getTagName());
    if (rootName != u"module")
    {
        std::println("Error: Root element must be <module>, got <{}>", StringToUtf8(rootName));
        return false;
    }

    auto moduleName = XmlToString(root->getAttribute(StringToXml(u"name")));
    auto moduleUuidStr = XmlToString(root->getAttribute(StringToXml(u"uuid")));
    auto modulePrimaryLocale = XmlToString(root->getAttribute(StringToXml(u"primary-locale")));

    moduleData.name = moduleName;
    moduleData.uuid = StringToUuid(moduleUuidStr);
    moduleData.primaryLocale = modulePrimaryLocale;

    auto localesElements = std::vector<xercesc::DOMElement*>();
    FindChildElements(root, u"locales", localesElements);

    for (auto const& localesElement : localesElements)
    {
        auto localeElements = std::vector<xercesc::DOMElement*>();
        FindChildElements(localesElement, u"locale", localeElements);

        for (auto const& localeElement : localeElements)
        {
            auto locale = XmlToString(localeElement->getTextContent());
            moduleData.locales.insert(locale);
        }
    }

    auto resourcesElements = std::vector<xercesc::DOMElement*>();
    FindChildElements(root, u"resources", resourcesElements);

    for (auto const& resourcesElement : resourcesElements)
    {
        auto resourceElements = std::vector<xercesc::DOMElement*>();
        FindChildElements(resourcesElement, u"resource", resourceElements);

        for (auto const& resourceElement : resourceElements)
        {
            auto id = XmlToString(resourceElement->getAttribute(StringToXml(u"id")));
            auto number = XmlToString(resourceElement->getAttribute(StringToXml(u"number")));
            auto type = XmlToString(resourceElement->getAttribute(StringToXml(u"type")));

            auto valueElements = std::vector<xercesc::DOMElement*>();
            FindChildElements(resourceElement, type, valueElements);

            if (type == u"string")
            {
                auto entry = StringEntry();
                entry.id = id;
                entry.number = StringToUnsignedInt(number);
                for (auto const& valueElement : valueElements)
                {
                    auto value = XmlToString(valueElement->getTextContent());
                    auto locale = XmlToString(valueElement->getAttribute(StringToXml(u"locale")));
                    auto stringValue = StringValue();
                    stringValue.value = value;
                    stringValue.locale = locale;
                    entry.values.push_back(stringValue);
                }
                moduleData.strings.emplace(id, entry);
            }
            else if (type == u"file")
            {
                auto entry = FileEntry();
                entry.id = id;
                entry.number = StringToUnsignedInt(number);
                for (auto const& valueElement : valueElements)
                {
                    auto path = XmlToString(valueElement->getTextContent());
                    auto locale = XmlToString(valueElement->getAttribute(StringToXml(u"locale")));
                    auto fileValue = FileValue();
                    fileValue.path = path;
                    fileValue.locale = locale;
                    entry.values.push_back(fileValue);
                }
                moduleData.files.emplace(id, entry);
            }
        }
    }

    return true;
}

auto WriteCacheFile(fs::path const& cacheXmlPath, ModuleData const& moduleData) -> bool
{
    std::println("write_cache_file: {}", cacheXmlPath.string());

    try
    {
        fs::create_directories(cacheXmlPath.parent_path());
    }
    catch (...)
    {
        std::println("Failed to create directories for path {}", cacheXmlPath.string());
    }

    auto impl = xercesc::DOMImplementation::getImplementation();

    auto doc = WrapDomPtr(impl->createDocument(nullptr, StringToXml(u"module"), nullptr));

    auto const uuidString = Utf8ToString(to_string(moduleData.uuid));
    auto const rootNode = doc->getDocumentElement();
    rootNode->setAttribute(StringToXml(u"name"), StringToXml(moduleData.name));
    rootNode->setAttribute(StringToXml(u"uuid"), StringToXml(uuidString));
    rootNode->setAttribute(StringToXml(u"primary-locale"), StringToXml(moduleData.primaryLocale));

    auto localesNode = doc->createElement(StringToXml(u"locales"));
    for (auto const& locale : moduleData.locales)
    {
        auto localeNode = doc->createElement(StringToXml(u"locale"));
        localeNode->setTextContent(StringToXml(locale));
        localesNode->appendChild(localeNode);
    }
    rootNode->appendChild(localesNode);

    auto resourcesNode = doc->createElement(StringToXml(u"resources"));

    for (auto const& [id, entry] : moduleData.strings)
    {
        auto const entryNode = doc->createElement(StringToXml(u"resource"));
        auto const numberString = Utf8ToString(std::to_string(entry.number));
        entryNode->setAttribute(StringToXml(u"id"), StringToXml(id));
        entryNode->setAttribute(StringToXml(u"number"), StringToXml(numberString));
        entryNode->setAttribute(StringToXml(u"type"), StringToXml(u"string"));
        for (auto const& value : entry.values)
        {
            auto const stringNode = doc->createElement(StringToXml(u"string"));
            stringNode->setTextContent(StringToXml(value.value));
            stringNode->setAttribute(StringToXml(u"locale"), StringToXml(value.locale));
            entryNode->appendChild(stringNode);
        }
        resourcesNode->appendChild(entryNode);
    }

    for (auto const& [id, entry] : moduleData.files)
    {
        auto const entryNode = doc->createElement(StringToXml(u"resource"));
        auto const numberString = Utf8ToString(std::to_string(entry.number));
        entryNode->setAttribute(StringToXml(u"id"), StringToXml(id));
        entryNode->setAttribute(StringToXml(u"number"), StringToXml(numberString));
        entryNode->setAttribute(StringToXml(u"type"), StringToXml(u"file"));
        for (auto const& value : entry.values)
        {
            auto const stringNode = doc->createElement(StringToXml(u"file"));
            stringNode->setTextContent(StringToXml(value.path));
            stringNode->setAttribute(StringToXml(u"locale"), StringToXml(value.locale));
            entryNode->appendChild(stringNode);
        }
        resourcesNode->appendChild(entryNode);
    }

    rootNode->appendChild(resourcesNode);

    auto fileTarget = xercesc::LocalFileFormatTarget(cacheXmlPath.string().c_str());
    auto output = WrapDomPtr(impl->createLSOutput());
    output->setByteStream(&fileTarget);
    auto serializer = WrapDomPtr(impl->createLSSerializer());
    serializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);
    serializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTXercesPrettyPrint, false);
    serializer->write(doc.get(), output.get());
    return true;
}
}
