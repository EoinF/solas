#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include "../testUtils.hpp"

class BoundRayCast_RemoveLightTest : public BaseLightingTest {
    CastingAlgorithm getType() {
        return CastingAlgorithm::BOUND_RAY_CAST;
    }
};

TEST_F(BoundRayCast_RemoveLightTest, basic_lighting_test)
{
        auto id = lightmapManager->addLight(22 * TILE_SIZE, 10 * TILE_SIZE, glm::vec2(1, 0), PI, 10);
        lightmapManager->removeLight(id);
        lightmapManager->update();

        EXPECT_TRUE(isTileUnlit(22, 10));
    
}