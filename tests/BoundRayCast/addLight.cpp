#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

#include "../testUtils.hpp"

class BoundRayCast_AddLightTest : public BaseLightingTest {
    CastingAlgorithm getType() {
        return CastingAlgorithm::BOUND_RAY_CAST;
    }
};

TEST_F(BoundRayCast_AddLightTest, basic_lighting_test)
{
    lightmapManager->addLight(22 * TILE_SIZE, 10 * TILE_SIZE, glm::vec2(1, 0), PI * 2, 10 * TILE_SIZE);
    lightmapManager->update();

    EXPECT_TRUE(isTileLit(22, 10));
}

TEST_F(BoundRayCast_AddLightTest, lighting_range_test)
{
    const auto RANGE_IN_TILES = 7;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, glm::vec2(1, 0), PI * 2, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();

    for (int i = 1; i <= RANGE_IN_TILES; i++)
    {
        EXPECT_TRUE(isTileLit(LIGHT_X + i, LIGHT_Y));
        EXPECT_TRUE(isTileLit(LIGHT_X - i, LIGHT_Y));
        EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y + i));
        EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y - i));
    }

    EXPECT_TRUE(isTileUnlit(LIGHT_X + RANGE_IN_TILES + 1, LIGHT_Y));
    EXPECT_TRUE(isTileUnlit(LIGHT_X - RANGE_IN_TILES - 1, LIGHT_Y));
    EXPECT_TRUE(isTileUnlit(LIGHT_X, LIGHT_Y + RANGE_IN_TILES + 1));
    EXPECT_TRUE(isTileUnlit(LIGHT_X, LIGHT_Y - RANGE_IN_TILES - 1));
}

TEST_F(BoundRayCast_AddLightTest, brightness_decreases)
{
    const auto RANGE_IN_TILES = 7;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, glm::vec2(1, 0), PI * 2, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();

    int brightnessAtCenter = lightmapManager->getTileState(LIGHT_X, LIGHT_Y)->brightness();
    int brightnessAtEdge = lightmapManager->getTileState(LIGHT_X + RANGE_IN_TILES, LIGHT_Y)->brightness();

    EXPECT_LT(brightnessAtEdge, brightnessAtCenter);
}


TEST_F(BoundRayCast_AddLightTest, brightness_decreases_with_distance_from_source)
{
    const auto RANGE_IN_TILES = 7;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 21;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, glm::vec2(1, 0), PI * 2, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();

    int brightness = lightmapManager->getTileState(LIGHT_X, LIGHT_Y)->brightness();

    auto orderedLightData = getOrderedLightData(LIGHT_X, LIGHT_Y, RANGE_IN_TILES);

    TestLightData previousLightData(255, 0, 0, 0);
    for (auto lightData : orderedLightData)
    {
        EXPECT_TRUE(previousLightData.brightness <= lightData.brightness
            ? ::testing::AssertionSuccess() :
            ::testing::AssertionFailure()
            << std::endl
            << "(x, y) | distance | brightness" << std::endl
            << "(" << lightData.x << "," << lightData.y << ") | " << lightData.distance << " | " << lightData.brightness << std::endl
            << "(" << previousLightData.x << "," << previousLightData.y << ") | " << previousLightData.distance << " | " << previousLightData.brightness << std::endl
        );

        previousLightData = lightData;
    }
}

TEST_F(BoundRayCast_AddLightTest, brightness_decreases_smoothly)
{
    const auto DELTA_LIMIT = 40;
    const auto RANGE_IN_TILES = 7;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, glm::vec2(1, 0), PI * 2, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();

    int brightness = lightmapManager->getTileState(LIGHT_X, LIGHT_Y)->brightness();

    auto orderedLightData = getOrderedLightData(LIGHT_X, LIGHT_Y, RANGE_IN_TILES);

    int previousBrightness = 0;
    for (auto lightData : orderedLightData)
    {
        EXPECT_LT(lightData.brightness - previousBrightness, DELTA_LIMIT);

        previousBrightness = lightData.brightness;
    }
}