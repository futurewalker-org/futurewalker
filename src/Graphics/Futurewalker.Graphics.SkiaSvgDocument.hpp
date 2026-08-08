// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.SkiaSvgDocumentType.hpp"

#include "Futurewalker.Graphics.SvgDocument.hpp" 

#include "Futurewalker.Core.PassKey.hpp"

#include <modules/svg/include/SkSVGDOM.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
class SkiaSvgDocument : public SvgDocument
{
public:
    static auto LoadFromStream(Shared<InputStream> const& stream) -> Unique<SkiaSvgDocument>;

public:
    SkiaSvgDocument(PassKey<SkiaSvgDocument>, sk_sp<SkSVGDOM> const& svg);

    auto Draw(Scene& scene, Size<Dp> const& viewport) -> void override;
    auto GetIntrinsicSize(Size<Dp> const& viewport) const -> Size<Dp> override;

private:
    sk_sp<SkSVGDOM> _svg;
};
}
}
