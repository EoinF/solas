#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include "../testUtils.hpp"

const auto WALL_X = 25;
const auto WALL_Y = 10;
const auto RANGE_IN_TILES = 4;

class BoundRayCast_WallTileTest : public BaseLightingTest
{
	LightmapManager *createTestSubject()
	{
		return new LightmapManager(TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST, 100);
	}
};

TEST_F(BoundRayCast_WallTileTest, wall_blocks_light_pointing_left)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X + 2) * TILE_SIZE, WALL_Y * TILE_SIZE, 2 * PI, RANGE_IN_TILES * TILE_SIZE);
	lightmapManager->update();

	EXPECT_TRUE(isTileLit(WALL_X, WALL_Y));
	EXPECT_TRUE(isTileUnlit(WALL_X - 1, WALL_Y));
}

TEST_F(BoundRayCast_WallTileTest, wall_blocks_light_pointing_right)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X - 2) * TILE_SIZE, WALL_Y * TILE_SIZE, 2 * PI, RANGE_IN_TILES * TILE_SIZE);
	lightmapManager->update();

	EXPECT_TRUE(isTileLit(WALL_X, WALL_Y));
	EXPECT_TRUE(isTileUnlit(WALL_X + 1, WALL_Y));
}

TEST_F(BoundRayCast_WallTileTest, wall_blocks_light_pointing_down)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight(WALL_X * TILE_SIZE, (WALL_Y - 2) * TILE_SIZE, 2 * PI, RANGE_IN_TILES * TILE_SIZE);
	lightmapManager->update();

	EXPECT_TRUE(isTileLit(WALL_X, WALL_Y));
	EXPECT_TRUE(isTileUnlit(WALL_X, WALL_Y + 1));
}

TEST_F(BoundRayCast_WallTileTest, wall_blocks_light_pointing_up)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight(WALL_X * TILE_SIZE, (WALL_Y + 2) * TILE_SIZE, 2 * PI, RANGE_IN_TILES * TILE_SIZE);
	lightmapManager->update();

	EXPECT_TRUE(isTileLit(WALL_X, WALL_Y));
	EXPECT_TRUE(isTileUnlit(WALL_X, WALL_Y - 1));
}

TEST_F(BoundRayCast_WallTileTest, wall_blocks_light_pointing_northwest)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X + 1) * TILE_SIZE, (WALL_Y + 1) * TILE_SIZE, 2 * PI, RANGE_IN_TILES * TILE_SIZE);
	lightmapManager->update();

	EXPECT_TRUE(isTileLit(WALL_X, WALL_Y));
	EXPECT_TRUE(isTileUnlit(WALL_X - 1, WALL_Y - 1));
}

TEST_F(BoundRayCast_WallTileTest, wall_blocks_light_pointing_northeast)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X - 1) * TILE_SIZE, (WALL_Y + 1) * TILE_SIZE, 2 * PI, 5 * TILE_SIZE);
	lightmapManager->update();

	EXPECT_TRUE(isTileLit(WALL_X, WALL_Y));
	EXPECT_TRUE(isTileUnlit(WALL_X + 1, WALL_Y - 1));
}

TEST_F(BoundRayCast_WallTileTest, wall_blocks_light_pointing_southwest)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X + 1) * TILE_SIZE, (WALL_Y - 1) * TILE_SIZE, 2 * PI, RANGE_IN_TILES * TILE_SIZE);
	lightmapManager->update();

	EXPECT_TRUE(isTileLit(WALL_X, WALL_Y));
	EXPECT_TRUE(isTileUnlit(WALL_X - 1, WALL_Y + 1));
}

TEST_F(BoundRayCast_WallTileTest, wall_blocks_light_pointing_southeast)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X - 1) * TILE_SIZE, (WALL_Y - 1) * TILE_SIZE, 2 * PI, RANGE_IN_TILES * TILE_SIZE);
	lightmapManager->update();

	EXPECT_TRUE(isTileLit(WALL_X, WALL_Y));
	EXPECT_TRUE(isTileUnlit(WALL_X + 1, WALL_Y + 1));
}
