#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

#include "../testUtils.hpp"

class BoundRayCast_UpdateTileTest : public BaseLightingTest
{
    LightmapManager *createTestSubject()
    {
        return new LightmapManager(TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST, 100);
    }
};

int RANGE_IN_TILES = 5;

TEST_F(BoundRayCast_UpdateTileTest, adding_wall_blocks_existing_light)
{
    int LIGHT_X = 22;
    int LIGHT_Y = 10;
    int WALL_X = LIGHT_X + 1;
    int WALL_Y = LIGHT_Y;
    bool IS_WALL = true;
    lightmapManager->addLight(LIGHT_X * TILE_SIZE, LIGHT_Y * TILE_SIZE, 3 * PI, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();
    lightmapManager->updateTile(WALL_X, WALL_Y, IS_WALL);
    lightmapManager->update();

    EXPECT_TRUE(isTileUnlit(LIGHT_X + 2, LIGHT_Y));
}

TEST_F(BoundRayCast_UpdateTileTest, removing_wall_unblocks_existing_light)
{
    int LIGHT_X = 22;
    int LIGHT_Y = 10;
    int WALL_X = LIGHT_X + 1;
    int WALL_Y = LIGHT_Y;
    bool IS_WALL = true;
    bool IS_NO_WALL = false;
    lightmapManager->updateTile(WALL_X, WALL_Y, IS_WALL);
    lightmapManager->addLight(LIGHT_X * TILE_SIZE, LIGHT_Y * TILE_SIZE, 3 * PI, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();
    lightmapManager->updateTile(WALL_X, WALL_Y, IS_NO_WALL);
    lightmapManager->update();

    EXPECT_TRUE(isTileLit(LIGHT_X + 2, LIGHT_Y));
}
