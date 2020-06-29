#pragma once
#include <SolasLib/core.hpp>

const auto TILE_SIZE = 4;
const float PI = glm::pi<float>();

class BaseLightingTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        lightmapManager = new LightmapManager(50, 50, TILE_SIZE, getType());
    }

    // void TearDown() override {}

public:
    virtual CastingAlgorithm getType() = 0;
    LightmapManager * lightmapManager;

    ::testing::AssertionResult isTileLit(int x, int y)
    {
        if (lightmapManager->getTileState(x, y)->r() >= 1) {
            return ::testing::AssertionSuccess();
        }
        else {
            return ::testing::AssertionFailure() << "Expected tile " << x << "," << y << " to be lit";
        }
    }
    ::testing::AssertionResult isTileUnlit(int x, int y)
    {
        if (lightmapManager->getTileState(x, y)->r() == 0) {
            return ::testing::AssertionSuccess();
        }
        else {
            return ::testing::AssertionFailure() << "Expected tile " << x << "," << y << " NOT to be lit";
        }
    }
};
