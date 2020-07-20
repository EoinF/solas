#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

#include "../testUtils.hpp"

const int CHUNK_SIZE = 100;

class BoundRayCast_BrightnessTest : public BaseLightingTest {
    LightmapManager* createTestSubject() {
        return new LightmapManager(TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST, CHUNK_SIZE);
    }
};


TEST_F(BoundRayCast_BrightnessTest, brightness_at_center)
{
    const auto BRIGHTNESS = 987;
    const auto RANGE_IN_TILES = 7;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, PI * 3, RANGE_IN_TILES * TILE_SIZE, glm::vec2(1, 0), BRIGHTNESS);
    lightmapManager->update();

    EXPECT_EQ(BRIGHTNESS, lightmapManager->getTileState(LIGHT_X, LIGHT_Y)->brightness());
}

TEST_F(BoundRayCast_BrightnessTest, brightness_decreases)
{
    const auto RANGE_IN_TILES = 7;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, PI * 3, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();

    int brightnessAtCenter = lightmapManager->getTileState(LIGHT_X, LIGHT_Y)->brightness();
    int brightnessAtEdge = lightmapManager->getTileState(LIGHT_X + RANGE_IN_TILES, LIGHT_Y)->brightness();

    EXPECT_LT(brightnessAtEdge, brightnessAtCenter);
}

TEST_F(BoundRayCast_BrightnessTest, brightness_decreases_with_distance_from_source)
{
    const auto RANGE_IN_TILES = 7;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 21;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, PI * 3, RANGE_IN_TILES * TILE_SIZE);
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

TEST_F(BoundRayCast_BrightnessTest, brightness_decreases_smoothly)
{
    const auto DELTA_LIMIT = 20;
    const auto RANGE_IN_TILES = 7;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, PI * 3, RANGE_IN_TILES * TILE_SIZE);
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


TEST_F(BoundRayCast_BrightnessTest, brightness_smooth_at_span_edge)
{
    const auto DELTA_LIMIT = 30;
    const auto RANGE_IN_TILES = 7;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, PI, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();

    int brightnessAtEdge = lightmapManager->getTileState(LIGHT_X, LIGHT_Y - 2)->brightness();
    int brightnessOutsideEdge = lightmapManager->getTileState(LIGHT_X - 1, LIGHT_Y - 2)->brightness();

    EXPECT_LT(brightnessAtEdge - brightnessOutsideEdge, DELTA_LIMIT);
}
