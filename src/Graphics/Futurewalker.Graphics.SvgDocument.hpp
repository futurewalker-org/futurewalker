// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.SvgDocumentType.hpp"
#include "Futurewalker.Graphics.SceneType.hpp"

#include "Futurewalker.Unit.Graphics.hpp"

#include "Futurewalker.Geometry.Size.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.InputStream.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief SVG document class.
///
class SvgDocument : public NonCopyable
{
public:
    static auto LoadFromStream(Shared<InputStream> const& stream) -> Unique<SvgDocument>;

public:
    virtual ~SvgDocument() = 0;

    ///
    /// @brief Draw SVG data to the given scene.
    ///
    /// @param[in] scene Scene to render to.
    /// @param[in] viewport The viewport size to consider.
    ///
    virtual auto Draw(Scene& scene, Size<Dp> const& viewport) -> void = 0;

    ///
    /// @brief Get the intrinsic size of the SVG data.
    ///
    /// @param[in] viewport The viewport size to consider.
    ///
    /// @return The intrinsic size of the SVG data within the given viewport.
    ///
    /// @note When the intrinsic size is not relative to the viewport, viewport size will be ignored.
    ///
    virtual auto GetIntrinsicSize(Size<Dp> const& viewport) const -> Size<Dp> = 0;
};
}
}
