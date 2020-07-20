#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

#include "../testUtils.hpp"

class BoundRayCast_UpdateLightTest : public BaseLightingTest {
    LightmapManager* createTestSubject() {
        return new LightmapManager(TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST, 100);
    }
};

TEST_F(BoundRayCast_UpdateLightTest, update_light_location)
{
    int LIGHT_X = 22;
    int LIGHT_Y = 10;
    int UPDATED_LIGHT_X = 40;
    int UPDATED_LIGHT_Y = 30;
    int lightId = lightmapManager->addLight(LIGHT_X * TILE_SIZE, LIGHT_Y * TILE_SIZE, PI * 3, 10 * TILE_SIZE);
    lightmapManager->update();
    lightmapManager->updateLight(lightId, UPDATED_LIGHT_X * TILE_SIZE, UPDATED_LIGHT_Y * TILE_SIZE, PI * 3, 10 * TILE_SIZE);
    lightmapManager->update();

    EXPECT_TRUE(isTileUnlit(LIGHT_X, LIGHT_Y));
    EXPECT_TRUE(isTileLit(UPDATED_LIGHT_X, UPDATED_LIGHT_Y));
}

TEST_F(BoundRayCast_UpdateLightTest, update_light_direction)
{
    int LIGHT_X = 22;
    int LIGHT_Y = 10;
    int lightId = lightmapManager->addLight(LIGHT_X * TILE_SIZE, LIGHT_Y * TILE_SIZE, PI, 10 * TILE_SIZE);
    lightmapManager->update();
    lightmapManager->updateLight(lightId, LIGHT_X * TILE_SIZE, LIGHT_Y * TILE_SIZE, PI, 10 * TILE_SIZE, glm::vec2(-1, 0));
    lightmapManager->update();

    EXPECT_TRUE(isTileUnlit(LIGHT_X + 1, LIGHT_Y));
    EXPECT_TRUE(isTileLit(LIGHT_X - 1, LIGHT_Y));
}
