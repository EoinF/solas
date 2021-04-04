#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include "../testUtils.hpp"

const auto WALL_X = 25;
const auto WALL_Y = 10;

class SimpleRayCast_WallTileTest : public BaseLightingTest
{
	LightmapManager *createTestSubject()
	{
		return new LightmapManager(TILE_SIZE, CastingAlgorithm::SIMPLE_RAY_CAST, 100);
	}
};

TEST_F(SimpleRayCast_WallTileTest, wall_blocks_light_pointing_left)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X + 2) * TILE_SIZE, WALL_Y * TILE_SIZE, 2 * PI, 3 * TILE_SIZE, glm::vec2(-1, 0));
	lightmapManager->update();

	EXPECT_TRUE(isTileUnlit(WALL_X - 1, WALL_Y));
}

TEST_F(SimpleRayCast_WallTileTest, wall_blocks_light_pointing_right)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X - 2) * TILE_SIZE, WALL_Y * TILE_SIZE, 2 * PI, 3 * TILE_SIZE, glm::vec2(1, 0));
	lightmapManager->update();

	EXPECT_TRUE(isTileUnlit(WALL_X + 1, WALL_Y));
}

TEST_F(SimpleRayCast_WallTileTest, wall_blocks_light_pointing_down)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight(WALL_X * TILE_SIZE, (WALL_Y - 2) * TILE_SIZE, 2 * PI, 3 * TILE_SIZE, glm::vec2(0, 1));
	lightmapManager->update();

	EXPECT_TRUE(isTileUnlit(WALL_X, WALL_Y + 1));
}

TEST_F(SimpleRayCast_WallTileTest, wall_blocks_light_pointing_up)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight(WALL_X * TILE_SIZE, (WALL_Y + 2) * TILE_SIZE, 2 * PI, 3 * TILE_SIZE, glm::vec2(0, -1));
	lightmapManager->update();

	EXPECT_TRUE(isTileUnlit(WALL_X, WALL_Y - 1));
}

TEST_F(SimpleRayCast_WallTileTest, wall_blocks_light_pointing_northwest)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X + 2) * TILE_SIZE, (WALL_Y + 2) * TILE_SIZE, 2 * PI, 3 * TILE_SIZE, glm::vec2(-1, -1));
	lightmapManager->update();

	EXPECT_TRUE(isTileUnlit(WALL_X - 1, WALL_Y - 1));
}

TEST_F(SimpleRayCast_WallTileTest, wall_blocks_light_pointing_northeast)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X - 2) * TILE_SIZE, (WALL_Y + 2) * TILE_SIZE, 2 * PI, 3 * TILE_SIZE, glm::vec2(1, -1));
	lightmapManager->update();

	EXPECT_TRUE(isTileUnlit(WALL_X + 1, WALL_Y - 1));
}

TEST_F(SimpleRayCast_WallTileTest, wall_blocks_light_pointing_southwest)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X + 2) * TILE_SIZE, (WALL_Y - 2) * TILE_SIZE, 2 * PI, 3 * TILE_SIZE, glm::vec2(-1, 1));
	lightmapManager->update();

	EXPECT_TRUE(isTileUnlit(WALL_X - 1, WALL_Y + 1));
}

TEST_F(SimpleRayCast_WallTileTest, wall_blocks_light_pointing_southeast)
{
	this->lightmapManager->updateTile(WALL_X, WALL_Y, true);

	lightmapManager->addLight((WALL_X - 2) * TILE_SIZE, (WALL_Y - 2) * TILE_SIZE, 2 * PI, 3 * TILE_SIZE, glm::vec2(1, 1));
	lightmapManager->update();

	EXPECT_TRUE(isTileUnlit(WALL_X + 1, WALL_Y + 1));
}
