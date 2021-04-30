#include <SolasLib/core/ChunkMap.hpp>

ChunkMap::ChunkMap(std::int64_t chunkSize, std::int64_t tileSize) {
	this->chunkSize = chunkSize;
	this->tileSize = tileSize;
	this->maxChunks = INT64_MAX / (chunkSize * chunkSize);
	allocateChunk(0, 0);
}

void ChunkMap::allocateChunk(std::int64_t chunkX, std::int64_t chunkY) {
	std::vector<TileLightState> tileArray;
	chunk_index_t chunkIndex = getChunkIndex(chunkX, chunkY);

	// tileArray.resize(chunkSize * chunkSize);
	for (int i = 0; i < chunkSize * chunkSize; i++) {
		tileArray.push_back(TileLightState());
	}

	this->data.insert({chunkIndex, tileArray});
}

std::vector<TileLightState> &ChunkMap::getOrAllocateChunk(std::int64_t chunkX,
														  std::int64_t chunkY) {
	std::int64_t chunkIndex = getChunkIndex(chunkX, chunkY);
	if (this->data.find(chunkIndex) == this->data.end()) {
		allocateChunk(chunkX, chunkY);
	}
	return this->data[chunkIndex];
}
