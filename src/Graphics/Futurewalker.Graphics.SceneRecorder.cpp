// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SceneRecorder.hpp"
#include "Futurewalker.Graphics.SkiaSceneRecorder.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief Create recording surface.
///
auto SceneRecorder::Make() -> Shared<SceneRecorder>
{
    return Shared<SkiaSceneRecorder>::Make();
}

///
/// @brief Destructor.
///
SceneRecorder::~SceneRecorder() = default;
}
