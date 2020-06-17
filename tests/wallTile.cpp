#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

const auto TILE_SIZE = 5;
const auto WALL_X = 25;
const auto WALL_Y = 10;

TEST(wall_tile, wall_blocks_light_pointing_left)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);
    lightmapManager->getTileState(WALL_X, WALL_Y)->isWall = true;

    lightmapManager->addLight((WALL_X + 2) * TILE_SIZE, WALL_Y * TILE_SIZE, glm::vec2(-1, 0), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(WALL_X - 1, WALL_Y)->r(), 0);
}

TEST(wall_tile, wall_blocks_light_pointing_right)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);
    lightmapManager->getTileState(WALL_X, WALL_Y)->isWall = true;

    lightmapManager->addLight((WALL_X - 2) * TILE_SIZE, WALL_Y * TILE_SIZE, glm::vec2(1, 0), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(WALL_X + 1, WALL_Y)->r(), 0);
}

TEST(wall_tile, wall_blocks_light_pointing_down)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);
    lightmapManager->getTileState(WALL_X, WALL_Y)->isWall = true;

    lightmapManager->addLight(WALL_X * TILE_SIZE, (WALL_Y - 2) * TILE_SIZE, glm::vec2(0, 1), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(WALL_X, WALL_Y + 1)->r(), 0);
}

TEST(wall_tile, wall_blocks_light_pointing_up)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);
    lightmapManager->getTileState(WALL_X, WALL_Y)->isWall = true;

    lightmapManager->addLight(WALL_X * TILE_SIZE, (WALL_Y + 2) * TILE_SIZE, glm::vec2(0, -1), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(WALL_X, WALL_Y - 1)->r(), 0);
}

TEST(wall_tile, wall_blocks_light_pointing_northwest)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);
    lightmapManager->getTileState(WALL_X, WALL_Y)->isWall = true;

    lightmapManager->addLight((WALL_X + 2) * TILE_SIZE, (WALL_Y + 2) * TILE_SIZE, glm::vec2(-1, -1), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(WALL_X - 1, WALL_Y - 1)->r(), 0);
}

TEST(wall_tile, wall_blocks_light_pointing_northeast)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);
    lightmapManager->getTileState(WALL_X, WALL_Y)->isWall = true;

    lightmapManager->addLight((WALL_X - 2) * TILE_SIZE, (WALL_Y + 2) * TILE_SIZE, glm::vec2(1, -1), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(WALL_X + 1, WALL_Y - 1)->r(), 0);
}

TEST(wall_tile, wall_blocks_light_pointing_southwest)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);
    lightmapManager->getTileState(WALL_X, WALL_Y)->isWall = true;

    lightmapManager->addLight((WALL_X + 2) * TILE_SIZE, (WALL_Y - 2) * TILE_SIZE, glm::vec2(-1, 1), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(WALL_X - 1, WALL_Y + 1)->r(), 0);
}

TEST(wall_tile, wall_blocks_light_pointing_southeast)
{
    auto lightmapManager = new LightmapManager(50, 50, TILE_SIZE);
    lightmapManager->getTileState(WALL_X, WALL_Y)->isWall = false;

    lightmapManager->addLight((WALL_X - 2) * TILE_SIZE, (WALL_Y - 2) * TILE_SIZE, glm::vec2(1, 1), 2, 10);
    lightmapManager->update();

    EXPECT_EQ(lightmapManager->getTileState(WALL_X + 1, WALL_Y + 1)->r(), 0);
}
