#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include "../testUtils.hpp"

const int CHUNK_SIZE = 100;

class BoundRayCast_RemoveLightTest : public BaseLightingTest
{
    LightmapManager *createTestSubject()
    {
        return new LightmapManager(TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST, CHUNK_SIZE);
    }
};

TEST_F(BoundRayCast_RemoveLightTest, all_lighting_cleared_after_removing)
{
    const auto RANGE_IN_TILES = 10;
    const auto LIGHT_X = 40;
    const auto LIGHT_Y = 65;

    auto id = lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, PI, RANGE_IN_TILES);
    lightmapManager->update();
    lightmapManager->removeLight(id);
    lightmapManager->update();

    auto lightDataList = getOrderedLightData(LIGHT_X, LIGHT_Y, RANGE_IN_TILES);
    for (auto data : lightDataList)
    {
        ASSERT_TRUE(isTileUnlit(data.x, data.y));
    }
}

TEST_F(BoundRayCast_RemoveLightTest, all_lighting_cleared_after_removing_at_edge)
{
    const auto RANGE_IN_TILES = 10;
    const auto LIGHT_X = 95;
    const auto LIGHT_Y = 65;

    auto id = lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, PI, RANGE_IN_TILES);
    lightmapManager->update();
    lightmapManager->removeLight(id);
    lightmapManager->update();

    auto lightDataList = getOrderedLightData(LIGHT_X, LIGHT_Y, RANGE_IN_TILES);
    for (auto data : lightDataList)
    {
        ASSERT_TRUE(isTileUnlit(data.x, data.y));
    }
}

TEST_F(BoundRayCast_RemoveLightTest, adding_a_wall_after_removing_light_does_not_crash)
{
    const auto RANGE_IN_TILES = 10;
    const auto LIGHT_X = 40;
    const auto LIGHT_Y = 65;

    auto id = lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, PI, RANGE_IN_TILES);
    lightmapManager->update();
    lightmapManager->removeLight(id);
    lightmapManager->update();
    lightmapManager->updateTile(LIGHT_X + 1, LIGHT_Y, true);

    ASSERT_TRUE(isTileUnlit(LIGHT_X + 1, LIGHT_Y));
}