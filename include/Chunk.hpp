#pragma once

#include "Engine.hpp"
#include "World.hpp"
#include "Block.hpp"

class Chunk {
public:
	Chunk(glm::vec3 pos, Engine *engine, World *world, int id);
	~Chunk();

	void create();

	void render();
	void rebuild();

	glm::vec3 center();
	glm::vec3 position();

	Chunk* FindChunk(glm::vec3 pos);

	BlockType GetTheoreticalBlock(glm::vec3 pos, glm::vec3 offset0, glm::vec3 offset1, glm::vec3 offset2);
	BlockType GetTheoreticalBlock(glm::vec3 pos);

	BlockType GetBlock(int x, int y, int z);
	BlockType GetBlock(glm::vec3 pos);

	void SetBlock(int x, int y, int z, BlockType type);
	
	float CalculateNoiseValue(glm::vec3 pos, glm::vec3 offset, float scale);
	bool IsTransparent(int x, int y, int z);

	bool ShouldRebuildChunk();
	bool IsChunkLoaded();

	Chunk* GetChunk(int x, int y, int z);

	Block*** blockArray()
	{
		return m_blocks;
	}

	int chunkId() 
	{
		return m_chunkGlobalID;
	}

private:	
	// The chunk Position
	glm::vec3 m_position;
	// 3D array of blocks
	Block*** m_blocks;
	// Boolean for when vhunk is modified
	bool m_changed;
	// Is the chunk actaully loaded
	bool m_loaded = false;

	// The chunk ID
	int m_chunkGlobalID;

	Engine *m_renderer;
	World* m_world;

	GLuint m_chunkID;
};