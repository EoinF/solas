#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include "../testUtils.hpp"

class SimpleRayCast_RemoveLightTest : public BaseLightingTest {
    LightmapManager* createTestSubject() {
        return new LightmapManager(100, 100, TILE_SIZE, CastingAlgorithm::SIMPLE_RAY_CAST);
    }
};

TEST_F(SimpleRayCast_RemoveLightTest, basic_lighting_test)
{
        auto id = lightmapManager->addLight(22 * TILE_SIZE, 10 * TILE_SIZE, glm::vec2(1, 0), PI, 10);
        lightmapManager->update();
        lightmapManager->removeLight(id);
        lightmapManager->update();

        EXPECT_TRUE(isTileUnlit(22, 10));
}