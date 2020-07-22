#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

#include "../testUtils.hpp"

class BoundRayCast_UpdateLightTest : public BaseLightingTest {
    LightmapManager* createTestSubject() {
        return new LightmapManager(TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST, 100);
    }
};
//
//TEST_F(BoundRayCast_UpdateLightTest, update_light_location)
//{
//    int LIGHT_X = 22;
//    int LIGHT_Y = 10;
//    int UPDATED_LIGHT_X = 40;
//    int UPDATED_LIGHT_Y = 30;
//    int lightId = lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, PI * 3, 10 * TILE_SIZE);
//    lightmapManager->update();
//    lightmapManager->updateLight(lightId, UPDATED_LIGHT_X * TILE_SIZE, UPDATED_LIGHT_Y * TILE_SIZE, PI * 3);
//    lightmapManager->update();
//
//    EXPECT_TRUE(isTileUnlit(LIGHT_X, LIGHT_Y));
//    EXPECT_TRUE(isTileLit(UPDATED_LIGHT_X, UPDATED_LIGHT_Y));
//}
//
//TEST_F(BoundRayCast_UpdateLightTest, update_light_direction)
//{
//    int LIGHT_X = 22;
//    int LIGHT_Y = 10;
//    int lightId = lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, PI, 10 * TILE_SIZE);
//    lightmapManager->update();
//    lightmapManager->updateLight(lightId, (LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, PI, glm::vec2(-1, 0));
//    lightmapManager->update();
//
//    EXPECT_TRUE(isTileUnlit(LIGHT_X + 1, LIGHT_Y));
//    EXPECT_TRUE(isTileLit(LIGHT_X - 1, LIGHT_Y));
//}

TEST_F(BoundRayCast_UpdateLightTest, update_light_span)
{
    const auto RANGE_IN_TILES = 7;
    const auto SPAN = PI / 2.0f;
    const auto UPDATED_SPAN = PI;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    int lightId = lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, SPAN, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();
    lightmapManager->updateLight(lightId, (LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, UPDATED_SPAN);
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

TEST_F(BoundRayCast_UpdateLightTest, lighting_semi_circle_test)
{
    const auto RANGE_IN_TILES = 7;
    const auto SPAN = PI;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, SPAN, RANGE_IN_TILES * TILE_SIZE);
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