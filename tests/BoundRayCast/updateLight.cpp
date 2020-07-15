#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

#include "../testUtils.hpp"

class BoundRayCast_UpdateLightTest : public BaseLightingTest {
    LightmapManager* createTestSubject() {
        return new LightmapManager(TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST, 100);
    }
};

TEST_F(BoundRayCast_UpdateLightTest, basic_lighting_test)
{
    int LIGHT_X = 22;
    int LIGHT_Y = 10;
    int UPDATED_LIGHT_X = 40;
    int UPDATED_LIGHT_Y = 30;
    int lightId = lightmapManager->addLight(LIGHT_X * TILE_SIZE, LIGHT_Y * TILE_SIZE, PI * 2, 10 * TILE_SIZE);
    lightmapManager->update();
    lightmapManager->updateLight(lightId, UPDATED_LIGHT_X * TILE_SIZE, UPDATED_LIGHT_Y * TILE_SIZE, PI * 2, 10 * TILE_SIZE);
    lightmapManager->update();

    EXPECT_TRUE(isTileUnlit(LIGHT_X, LIGHT_Y));
    EXPECT_TRUE(isTileLit(UPDATED_LIGHT_X, UPDATED_LIGHT_Y));
}
