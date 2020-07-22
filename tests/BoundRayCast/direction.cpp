#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

#include "../testUtils.hpp"

class BoundRayCast_LightDirectionTest : public BaseLightingTest {
    LightmapManager* createTestSubject() {
        return new LightmapManager(TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST, 100);
    }
};

TEST_F(BoundRayCast_LightDirectionTest, lighting_semi_circle_test_pointing_left)
{
    const auto RANGE_IN_TILES = 7;
    const auto SPAN = PI;
    const auto DIRECTION = glm::vec2(-1, 0);
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, SPAN, RANGE_IN_TILES * TILE_SIZE, DIRECTION);
    lightmapManager->update();

    EXPECT_TRUE(isTileLit(LIGHT_X - 1, LIGHT_Y - 1));
    EXPECT_TRUE(isTileLit(LIGHT_X - 1, LIGHT_Y));
    EXPECT_TRUE(isTileLit(LIGHT_X - 1, LIGHT_Y + 1));
    EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y - 1));
    EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y));
    EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y + 1));

    EXPECT_TRUE(isTileUnlit(LIGHT_X + 1, LIGHT_Y - 1));
    EXPECT_TRUE(isTileUnlit(LIGHT_X + 1, LIGHT_Y));
    EXPECT_TRUE(isTileUnlit(LIGHT_X + 1, LIGHT_Y + 1));
}

TEST_F(BoundRayCast_LightDirectionTest, lighting_semi_circle_test_pointing_down)
{
    const auto RANGE_IN_TILES = 7;
    const auto SPAN = PI;
    const auto DIRECTION = glm::vec2(0, -1);
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, SPAN, RANGE_IN_TILES * TILE_SIZE, DIRECTION);
    lightmapManager->update();

    EXPECT_TRUE(isTileLit(LIGHT_X - 1, LIGHT_Y - 1));
    EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y - 1));
    EXPECT_TRUE(isTileLit(LIGHT_X + 1, LIGHT_Y - 1));
    EXPECT_TRUE(isTileLit(LIGHT_X - 1, LIGHT_Y));
    EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y));
    EXPECT_TRUE(isTileLit(LIGHT_X + 1, LIGHT_Y));

    EXPECT_TRUE(isTileUnlit(LIGHT_X - 1, LIGHT_Y + 1));
    EXPECT_TRUE(isTileUnlit(LIGHT_X, LIGHT_Y + 1));
    EXPECT_TRUE(isTileUnlit(LIGHT_X + 1, LIGHT_Y + 1));
}