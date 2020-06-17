#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

#include "testUtils.hpp"

class AddLightTest : public BaseLightingTest {};

TEST_F(AddLightTest, basic_lighting_test)
{
    lightmapManager->addLight(22 * TILE_SIZE, 10 * TILE_SIZE, glm::vec2(1, 0), PI * 2, 10 * TILE_SIZE);
    lightmapManager->update();

    EXPECT_TRUE(isTileLit(22, 10));
}

TEST_F(AddLightTest, lighting_range_test)
{
    const auto rangeInTiles = 10;
    lightmapManager->addLight((22 * TILE_SIZE) + 1, 10 * TILE_SIZE, glm::vec2(1, 0), PI * 2, rangeInTiles * TILE_SIZE);
    lightmapManager->update();

    for (int i = 0; i < rangeInTiles; i++)
    {
        EXPECT_TRUE(isTileLit(22 + i, 10));
    }
}