#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

#include "../testUtils.hpp"

class BoundRayCast_LightSpanTest : public BaseLightingTest {
    LightmapManager* createTestSubject() {
        return new LightmapManager(100, 100, TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST);
    }
};

TEST_F(BoundRayCast_LightSpanTest, lighting_semi_circle_test)
{
    const auto RANGE_IN_TILES = 7;
    const auto SPAN = PI;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, SPAN, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();

    EXPECT_TRUE(isTileLit(LIGHT_X + 1, LIGHT_Y - 1));
    EXPECT_TRUE(isTileLit(LIGHT_X + 1, LIGHT_Y));
    EXPECT_TRUE(isTileLit(LIGHT_X + 1, LIGHT_Y + 1));
    EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y - 1));
    EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y));
    EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y + 1));
    EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y - 2));
    EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y + 2));

    EXPECT_TRUE(isTileUnlit(LIGHT_X - 1, LIGHT_Y - 1));
    EXPECT_TRUE(isTileUnlit(LIGHT_X - 1, LIGHT_Y));
    EXPECT_TRUE(isTileUnlit(LIGHT_X - 1, LIGHT_Y + 1));
}

TEST_F(BoundRayCast_LightSpanTest, lighting_quarter_circle_test)
{
    const auto RANGE_IN_TILES = 7;
    const auto SPAN = PI / 2.0f;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, SPAN, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();

    EXPECT_TRUE(isTileLit(LIGHT_X + 1, LIGHT_Y - 1));
    EXPECT_TRUE(isTileLit(LIGHT_X + 1, LIGHT_Y));
    EXPECT_TRUE(isTileLit(LIGHT_X + 1, LIGHT_Y + 1));
    EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y));

    EXPECT_TRUE(isTileUnlit(LIGHT_X, LIGHT_Y - 2));
    EXPECT_TRUE(isTileUnlit(LIGHT_X, LIGHT_Y + 2));
    EXPECT_TRUE(isTileUnlit(LIGHT_X - 1, LIGHT_Y - 1));
    EXPECT_TRUE(isTileUnlit(LIGHT_X - 1, LIGHT_Y));
    EXPECT_TRUE(isTileUnlit(LIGHT_X - 1, LIGHT_Y + 1));
}

TEST_F(BoundRayCast_LightSpanTest, lighting_full_circle_test)
{
    const auto RANGE_IN_TILES = 7;
    const auto SPAN = 2.0f * PI;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, SPAN, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();

    for (auto lightData : getOrderedLightData(LIGHT_X, LIGHT_Y, 2))
    {
        EXPECT_TRUE(isTileLit(lightData.x, lightData.y));
    }
}