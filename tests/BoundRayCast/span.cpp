#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

#include "../testUtils.hpp"

class BoundRayCast_LightSpanTest : public BaseLightingTest {
    LightmapManager* createTestSubject() {
        return new LightmapManager(100, 100, TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST);
    }
};

float getSmallestAngleToTile(float x, float y)
{
    float angleTopLeft = std::atan2(y, x);
    float angleTopRight = std::atan2(y, x + 1);
    float angleBottomLeft = std::atan2(y + 1, x);
    float angleBottomRight = std::atan2(y + 1, x + 1);
    return glm::min(abs(angleTopLeft), abs(angleTopRight), abs(angleBottomLeft), abs(angleBottomRight));
}

TEST_F(BoundRayCast_LightSpanTest, lighting_semi_circle_test)
{
    const auto RANGE_IN_TILES = 7;
    const auto SPAN = PI;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, SPAN, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();

    for (auto lightData: getOrderedLightData(LIGHT_X, LIGHT_Y, 2))
    {
        float angle = getSmallestAngleToTile(lightData.x - (LIGHT_X + 0.5f), lightData.y - (LIGHT_Y + 0.5f));
        if (2.0f * angle <= SPAN)
        {
            EXPECT_TRUE(isTileLit(lightData.x, lightData.y));
        }
        else {
            EXPECT_TRUE(isTileUnlit(lightData.x, lightData.y));
        }
    }
}

TEST_F(BoundRayCast_LightSpanTest, lighting_quarter_circle_test)
{
    const auto RANGE_IN_TILES = 7;
    const auto SPAN = PI / 2.0f;
    const auto MARGIN_OF_ERROR = 0.0001f;
    const auto LIGHT_X = 22;
    const auto LIGHT_Y = 10;
    lightmapManager->addLight((LIGHT_X + 0.5) * TILE_SIZE, (LIGHT_Y + 0.5) * TILE_SIZE, SPAN, RANGE_IN_TILES * TILE_SIZE);
    lightmapManager->update();

    for (auto lightData : getOrderedLightData(LIGHT_X, LIGHT_Y, 2))
    {
        float angle = getSmallestAngleToTile(lightData.x - (LIGHT_X + 0.5f), lightData.y - (LIGHT_Y + 0.5f));
        if (SPAN - (2.0f * angle) > MARGIN_OF_ERROR) { // Give some leeway on tiles that are on the perimeter of the span
            if (2.0f * angle < SPAN)
            {
                EXPECT_TRUE(isTileLit(lightData.x, lightData.y));
            }
            else {
                EXPECT_TRUE(isTileUnlit(lightData.x, lightData.y));
            }
        }
    }
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