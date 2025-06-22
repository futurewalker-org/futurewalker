// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.SceneRecorderType.hpp"
#include "Futurewalker.Graphics.SceneType.hpp"
#include "Futurewalker.Graphics.DisplayListType.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Scene recorder.
///
class SceneRecorder : NonCopyable
{
public:
    static auto Make() -> Shared<SceneRecorder>;

public:
    virtual ~SceneRecorder() = 0;
    virtual auto BeginRecording() -> Shared<Scene> = 0;
    virtual auto EndRecording() -> Shared<DisplayList> = 0;
};
}
}
