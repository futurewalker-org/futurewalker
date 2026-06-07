// SPDX-License-Identifier: MPL-2.0

#include "main.hpp"

namespace ResourceCompiler
{
auto StringToXml(std::u16string const& str) -> XMLCh const*
{
    return reinterpret_cast<XMLCh const*>(str.c_str());
}

auto XmlToString(XMLCh const* xmlCh) -> std::u16string
{
    return std::u16string(reinterpret_cast<char16_t const*>(xmlCh));
}

auto StringToUtf8(std::u16string const& str) -> std::string
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

auto Utf8ToString(std::string const& str) -> std::u16string
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

auto XmlToUtf8(XMLCh const* xmlCh) -> std::string
{
    return StringToUtf8(XmlToString(xmlCh));
}

auto StringToUuid(std::u16string const& str) -> boost::uuids::uuid
{
    try
    {
        std::string strUtf8 = StringToUtf8(str);
        return boost::uuids::string_generator()(strUtf8);
    }
    catch (...)
    {
        return boost::uuids::uuid();
    }
}

auto StringToUnsignedInt(std::u16string const& str) -> uint32_t
{
    try
    {
        auto utf8 = StringToUtf8(str);
        return static_cast<uint32_t>(std::stoul(utf8));
    }
    catch (...)
    {
        return 0;
    }
}

auto IsAlpha(char16_t ch) -> bool
{
    return (u'a' <= ch && ch <= u'z') || (u'A' <= ch && ch <= u'Z');
}

auto IsUpperAlpha(char16_t ch) -> bool
{
    return (u'A' <= ch && ch <= u'Z');
}

auto IsDigit(char16_t ch) -> bool
{
    return (u'0' <= ch && ch <= u'9');
}

auto IsDot(char16_t ch) -> bool
{
    return ch == u'.';
}
}
