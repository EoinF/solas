#pragma once
#include <SolasLib/core.hpp>
#include <gtest/gtest.h>

const auto TILE_SIZE = 4;
const float PI = glm::pi<float>();

struct TestLightData {
	TestLightData(float distance, int brightness, int x, int y) {
		this->distance = distance;
		this->brightness = brightness;
		this->x = x;
		this->y = y;
	}
	float distance;
	int brightness;
	int x;
	int y;
};

class BaseLightingTest : public ::testing::Test {
  protected:
	void SetUp() override { lightmapManager = createTestSubject(); }

	// void TearDown() override {}

  public:
	virtual LightmapManager *createTestSubject() = 0;
	LightmapManager *lightmapManager;

	::testing::AssertionResult isTileLit(std::int64_t x, std::int64_t y) {
		if (lightmapManager->getTileState(x, y).brightness() >= 1) {
			return ::testing::AssertionSuccess();
		} else {
			return ::testing::AssertionFailure()
				   << "Expected tile " << x << "," << y << " to be lit";
		}
	}
	::testing::AssertionResult isTileUnlit(std::int64_t x, std::int64_t y) {
		if (lightmapManager->getTileState(x, y).brightness() == 0) {
			return ::testing::AssertionSuccess();
		} else {
			return ::testing::AssertionFailure()
				   << "Expected tile " << x << "," << y << " NOT to be lit";
		}
	}

	/**
	 * Creates a vector of triples (brightness, tileX, tileY) ordered based on ascending distance
	 * from given src coordinates
	 *
	 * @param srcX The x index of the tile where we want to calculate from
	 * @param srcY The x index of the tile where we want to calculate from
	 * @param rangeInTiles The number of tiles in each of the positive and negative x, y directions
	 * that will be calculated
	 *
	 * @return The vector with the sorted light data
	 */
	std::vector<TestLightData> getOrderedLightData(int srcX, int srcY, int rangeInTiles) {
		std::vector<TestLightData> orderedLightData;
		for (int i = -rangeInTiles; i <= rangeInTiles; i++) {
			for (int j = -rangeInTiles; j <= rangeInTiles; j++) {
				TestLightData lightData(
					glm::length(glm::vec2(i, j)),
					lightmapManager->getTileState(srcX + i, srcY + j).brightness(), srcX + i,
					srcY + j);
				bool wasInserted = false;
				for (auto it = orderedLightData.begin(); it != orderedLightData.end(); it++) {
					if (lightData.distance > (*it).distance) {
						orderedLightData.insert(it, lightData);
						wasInserted = true;
						break;
					}
				}
				if (!wasInserted) {
					orderedLightData.push_back(lightData);
				}
			}
		}
		return orderedLightData;
	}
};
