// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SvgDocument.hpp"
#include "Futurewalker.Graphics.SkiaSvgDocument.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @note Load SVG data from the given input stream.
///
/// @param[in] stream Input stream containing SVG data.
///
/// @return Shared SvgDocument or null if failed to load.
///
auto SvgDocument::LoadFromStream(Shared<InputStream> const& stream) -> Unique<SvgDocument>
{
    return SkiaSvgDocument::LoadFromStream(stream);
}

SvgDocument::~SvgDocument() = default;
}
