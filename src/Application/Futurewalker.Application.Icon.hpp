// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.IconType.hpp"

#include "Futurewalker.Graphics.FontFamily.hpp"
#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.SceneType.hpp"
#include "Futurewalker.Graphics.SvgDocumentType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Color.RGBAColor.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Icon class.
///
class Icon
{
public:
    static auto MakeBlank(Size<Dp> const& intrinsicSize = {}) -> Icon;
    static auto MakeFromFont(Graphics::FontFamily const& family, char32_t const& codePoint, Graphics::FontSize const& size) -> Icon;
    static auto MakeFromSvgDocument(Unique<Graphics::SvgDocument> svgDocument) -> Icon;

    Icon() = default;

    // TODO: Provide better equality comparison.
    auto operator==(Icon const& other) const -> bool = default;
    auto operator!=(Icon const& other) const -> bool = default;

    auto Draw(Graphics::Scene& scene, Size<Dp> const& size) const -> void;

    auto GetIntrinsicSize() const -> Size<Dp>;

    auto IsEmpty() const -> Bool;

private:
    class Impl;
    Shared<Impl const> _impl;
    explicit Icon(Shared<Impl const> const& impl);
};
}
}
