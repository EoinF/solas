#pragma once

#include <map>
#include <vector>
#include <SolasLib/core/TileLightState.hpp>

typedef std::int64_t chunk_index_t;

class ChunkMap {
  private:
	std::map<chunk_index_t, std::vector<TileLightState>> data;
	std::int64_t maxChunks;

  public:
	std::int64_t tileSize;
	std::int64_t chunkSize;
	chunk_index_t getChunkIndex(std::int64_t chunkX, std::int64_t chunkY) const {
		return chunkX + chunkY * maxChunks;
	}

	void allocateChunk(std::int64_t chunkX, std::int64_t chunkY);
	std::vector<TileLightState> &getOrAllocateChunk(std::int64_t chunkX, std::int64_t chunkY);

	ChunkMap(std::int64_t chunkSize, std::int64_t tileSize);

	TileLightState &getTileLightStateUnsafe(std::int64_t tileX, std::int64_t tileY) {
		double chunkSizeF = chunkSize;
		std::int64_t chunkX = floor(tileX / chunkSizeF);
		std::int64_t chunkY = floor(tileY / chunkSizeF);
		chunk_index_t chunkIndex = getChunkIndex(chunkX, chunkY);
		std::int64_t chunkTileX = tileX - chunkX * chunkSize;
		std::int64_t chunkTileY = tileY - chunkY * chunkSize;

		return data[chunkIndex][chunkTileX + chunkTileY * chunkSize];
	}

	TileLightState &getTileLightState(std::int64_t x, std::int64_t y) {
		double chunkSizeF = chunkSize;
		std::int64_t chunkX = floor(x / chunkSizeF);
		std::int64_t chunkY = floor(y / chunkSizeF);
		std::int64_t chunkTileX = x - chunkSize * chunkX;
		std::int64_t chunkTileY = y - chunkSize * chunkY;
		return getOrAllocateChunk(chunkX, chunkY)[chunkTileX + chunkTileY * chunkSize];
	}

	void clearTileState() {
		for (auto &chunk : this->data) {
			for (int i = 0; i < chunkSize * chunkSize; i++) {
				chunk.second[i].isWall = false;
			}
		}
	}
};
