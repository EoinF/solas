#pragma once
#include <SolasLib/core.hpp>

const auto TILE_SIZE = 5;
const float PI = glm::pi<float>();

class BaseLightingTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        lightmapManager = new LightmapManager(50, 50, TILE_SIZE);
    }

    // void TearDown() override {}

public:
    LightmapManager* lightmapManager;

    ::testing::AssertionResult isTileLit(int x, int y)
    {
        if (lightmapManager->getTileState(x, y)->r() >= 1) {
            return ::testing::AssertionSuccess();
        }
        else {
            return ::testing::AssertionFailure() << "tile " << x << "," << y << " is not lit";
        }
    }
};
