#pragma once

// Includes for the World Manager Class
#include "XyEngine.h"
#include "Chunk.h"
#include "Block.h"
#include "Player.h"
#include "Frustum.h"

// Defines world sizes (The bigger the world, the longer it takes to create)
#define WORLD_TINY 8
#define WORLD_SMALL 16
#define WORLD_LARGE 32
#define WORLD_HUGE 48

// The Selected World Size
const int SECLECTED_WORLD_SIZE = WORLD_SMALL;

// How many chunks can the player see
#define VIEW_DISTANCE 11

// Define Chunk class to prevent compile Issues (Probably a better way to do it)
class Chunk;

/*
	The WorldManager class creates the world using the World Size defines and the view distance define
	- Chunks are stored in a vector
	- Only Chunks in the fustrum are loaded
	- Only Chunks in the players view distance are rendered
*/
class WorldManager
{
private:
	// A Vector containing all the loaded chunks
	std::vector<Chunk*> m_ChunkList;

	// Create the main renderer to actually render and position the chunks to the screen
	XyEngine *m_renderer;

	// Vector3 Containing the camera position
	glm::vec3 m_cameraPosition;
	// Vector3 Containing the camera rotation? (Cant remember)
	glm::vec3 m_cameraView;

	// The number of chunks that can be loaded per frame
	int ASYNC_NUM_CHUNKS_PER_FRAME = 3;

	// The total number of chunks loaded
	int chunksLoaded;
	// The total number of chunks in the fustrum
	int chunksFrustum;
public:
	// Passes in the renderer to help render the chunks
	WorldManager(XyEngine *renderer);

	// Returns the chunk vector
	std::vector<Chunk*> GetChunkList() { return m_ChunkList;   }

	// Returns the view distance for the chunk
	int GetViewDistance() { return VIEW_DISTANCE; }
	// Returns the total number of loaded chunks
	int GetLoadedChunks() { return chunksLoaded;  }
	// Returns the total number of chunks taht can bee seen
	int GetFrustumChunks() { return chunksFrustum; }

	/*
		Updates the world and rubuild chunks that need to be rebuilded
		- takes in the Camera position and Rotation
	*/
	void Update(glm::vec3 cameraPosition, glm::vec3 cameraView);

	/*
		Goes through all the chunks, seeing which ones need to be rebuilt, then building them
	*/
	void UpdateRebuildList();

	/*
		Renders only the chunks that can be seen on screen
	*/
	void UpdateRenderList();

	/*
		Creates all the chunks, putting them in their correct positions
	*/
	void UpdateSetupList();
	
	/*
		A very hackish method to get really bad physics to work
		- Needs to be replaced with good system
		- Not recomended to use in other projects
	*/
	void UpdatePhysics(Player* player);

	/*
		Places a block of the chosen type in the world
		@param x y z World position to place the block
		@param type What type of block to place
	*/
	void SetBlock(int x, int y, int z, BlockType type);

	/*
		Returns the blocktype of the block at the given XYZ Coords
		@param x y z World position to check for block
	*/
	BlockType GetBlock(int x, int y, int z);

	Chunk *GetChunk(glm::vec3 pos);
};