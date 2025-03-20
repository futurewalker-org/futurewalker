#include <catch2/catch_all.hpp>

#include <Futurewalker.Graphics.SceneRecorder.hpp>
#include <Futurewalker.Graphics.Scene.hpp>

using namespace Futurewalker;

TEST_CASE("SceneRecorder")
{
    auto recorder = Graphics::SceneRecorder::Make();
    REQUIRE(recorder);

    auto scene = recorder->BeginRecording();
    REQUIRE(scene);
    REQUIRE(recorder->BeginRecording());

    auto displayList = recorder->EndRecording();
    REQUIRE(displayList);
    REQUIRE(!recorder->EndRecording());

    scene->AddLine({});
}
