#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

const auto TILE_SIZE = 5;

TEST(add_light, basic_lighting_test)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);

    lightmapManager->addLight(22 * TILE_SIZE, 10 * TILE_SIZE, glm::vec2(1, 0), 2, 10);
    lightmapManager->update();

    EXPECT_GE(lightmapManager->getTileState(22, 10)->r(), 1);
}

TEST(add_light, wall_blocks_light_horizontal_left)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);

    lightmapManager->getTileState(25, 10)->isWall = true;
    lightmapManager->addLight(22 * TILE_SIZE, 10 * TILE_SIZE, glm::vec2(1, 0), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(26, 10)->r(), 0);
}

TEST(add_light, wall_blocks_light_horizontal_right)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);

    lightmapManager->getTileState(25, 10)->isWall = true;
    lightmapManager->addLight(22 * TILE_SIZE, 10 * TILE_SIZE, glm::vec2(1, 0), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(26, 10)->r(), 0);
}

TEST(add_light, wall_blocks_light_vertical_down)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);

    lightmapManager->getTileState(25, 10)->isWall = true;
    lightmapManager->addLight(25 * TILE_SIZE, 8 * TILE_SIZE, glm::vec2(0, 1), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(25, 11)->r(), 0);
}

TEST(add_light, wall_blocks_light_vertical_up)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);

    lightmapManager->getTileState(25, 10)->isWall = true;
    lightmapManager->addLight(25 * TILE_SIZE, 12 * TILE_SIZE, glm::vec2(0, -1), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(25, 9)->r(), 0);
}
