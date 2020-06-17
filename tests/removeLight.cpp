#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

const auto TILE_SIZE = 5;
const float PI = glm::pi<float>();

TEST(removeLight, basic_lighting_test)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);

    auto id = lightmapManager->addLight(22 * TILE_SIZE, 10 * TILE_SIZE, glm::vec2(1, 0), PI, 10);
    lightmapManager->removeLight(id);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(22, 10)->r(), 0);
}