#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>

#include "../testUtils.hpp"

const int CHUNK_SIZE = 100;

class BoundRayCast_AddLightTest : public BaseLightingTest {
	LightmapManager *createTestSubject() {
		return new LightmapManager(TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST, CHUNK_SIZE);
	}
};

TEST_F(BoundRayCast_AddLightTest, basic_lighting_test) {
	const auto RANGE_IN_TILES = 10;
	const auto LIGHT_X = 22;
	const auto LIGHT_Y = 10;
	lightmapManager->addLight(LIGHT_X * TILE_SIZE, LIGHT_Y * TILE_SIZE, PI * 3,
							  RANGE_IN_TILES * TILE_SIZE);
	lightmapManager->update();

	EXPECT_TRUE(isTileLit(22, 10));
}

TEST_F(BoundRayCast_AddLightTest, lighting_range_test) {
	const auto RANGE_IN_TILES = 7;
	const auto LIGHT_X = 22;
	const auto LIGHT_Y = 10;
	lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, PI * 3,
							  RANGE_IN_TILES * TILE_SIZE);
	lightmapManager->update();

	for (int i = 1; i <= RANGE_IN_TILES; i++) {
		EXPECT_TRUE(isTileLit(LIGHT_X + i, LIGHT_Y));
		EXPECT_TRUE(isTileLit(LIGHT_X - i, LIGHT_Y));
		EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y + i));
		EXPECT_TRUE(isTileLit(LIGHT_X, LIGHT_Y - i));
	}

	EXPECT_TRUE(isTileUnlit(LIGHT_X + RANGE_IN_TILES + 1, LIGHT_Y));
	EXPECT_TRUE(isTileUnlit(LIGHT_X - RANGE_IN_TILES - 1, LIGHT_Y));
	EXPECT_TRUE(isTileUnlit(LIGHT_X, LIGHT_Y + RANGE_IN_TILES + 1));
	EXPECT_TRUE(isTileUnlit(LIGHT_X, LIGHT_Y - RANGE_IN_TILES - 1));
}

TEST_F(BoundRayCast_AddLightTest, light_at_edge_of_chunk) {
	const auto RANGE_IN_TILES = 7;
	const auto LIGHT_X = -2;
	const auto LIGHT_Y = 10;
	lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, PI * 3,
							  RANGE_IN_TILES * TILE_SIZE);
	lightmapManager->update();

	EXPECT_TRUE(isTileLit(-1, LIGHT_Y));
	EXPECT_TRUE(isTileLit(0, LIGHT_Y));
	EXPECT_TRUE(isTileLit(1, LIGHT_Y));
}

TEST_F(BoundRayCast_AddLightTest, light_at_edge_of_chunk_no_wraparound) {
	const auto RANGE_IN_TILES = 7;
	const auto LIGHT_X = -2;
	const auto LIGHT_Y = 10;
	lightmapManager->addLight((LIGHT_X + 0.5f) * TILE_SIZE, (LIGHT_Y + 0.5f) * TILE_SIZE, PI * 3,
							  RANGE_IN_TILES * TILE_SIZE);
	lightmapManager->update();

	EXPECT_TRUE(isTileUnlit(CHUNK_SIZE - 1, LIGHT_Y));
	EXPECT_TRUE(isTileUnlit(CHUNK_SIZE - 2, LIGHT_Y));
}