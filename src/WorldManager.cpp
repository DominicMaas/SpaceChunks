#include "WorldManager.h"

/*
	Passes in the renderer to help render the chunks
*/
WorldManager::WorldManager(XyEngine *renderer)
{
	// Sets the renderer
	m_renderer = renderer;
}

/*
	Updates the world and rubuild chunks that need to be rebuilded
	- takes in the Camera position and Rotation
*/
void WorldManager::Update(glm::vec3 cameraPosition, glm::vec3 cameraView)
{
	// Updates the Rebuild List
	UpdateRebuildList(); 
	// Updates the Render List
	UpdateRenderList();

	// Checks if the new camera position and rotation does not match the old
	if (m_cameraPosition != cameraPosition || m_cameraView != cameraView)
	{
		
	}

	// Sets the camera position and rotation
	m_cameraPosition = cameraPosition;
	m_cameraView = cameraView;
}

/*
	Goes through all the chunks, seeing which ones need to be rebuilt, then building them
*/
void WorldManager::UpdateRebuildList()
{
	// Perform a loop of all the chunks
	for (int i = 0; i < m_ChunkList.size(); i++)
	{
		// If a chunk needs to be rebuilt, then continue
		if (m_ChunkList.at(i)->ShouldRebuildChunk())
		{
			// Rubuilds the selected chunk
			m_ChunkList[i]->Rebuild();
		}
	}
}

/*
	Renders only the chunks that can be seen on screen
*/
void WorldManager::UpdateRenderList()
{
	// Set the number of chunks in the frustum to zero
	chunksFrustum = 0;

	// Perform a loop of all the chunks
	for (int i = 0; i < m_ChunkList.size(); i++)
	{
		// If the chunk is in the frustum, continue
		if (Frustum::GetFrustum(m_renderer).cubeInFrustum(m_ChunkList.at(i)->GetPosition().x, m_ChunkList.at(i)->GetPosition().y, m_ChunkList.at(i)->GetPosition().z, m_ChunkList.at(i)->GetPosition().x + CHUNK_X, m_ChunkList.at(i)->GetPosition().y + CHUNK_Y, m_ChunkList.at(i)->GetPosition().z + CHUNK_Z))
		{
			// If the chunk is in the players view distance, continue
			if (abs(m_ChunkList.at(i)->GetCenter().x - m_cameraPosition.x) < CHUNK_X * VIEW_DISTANCE  && abs(m_ChunkList.at(i)->GetCenter().z - m_cameraPosition.z) < CHUNK_Z * VIEW_DISTANCE)
			{
				// If the chunk is loaded, continue
				if (m_ChunkList.at(i)->IsChunkLoaded())
				{
					// Renders the chunk
					m_ChunkList.at(i)->Render();

					// Increment number of chunks rendered
					chunksFrustum++;
				}
			}
		}
	}

	
	// If a chunk needs to be rebuilt, then continue
	if (GetChunk(m_cameraPosition) == NULL)
	{
		int x = (int)floor(m_cameraPosition.x);
		int y = (int)floor(m_cameraPosition.y);
		int z = (int)floor(m_cameraPosition.z);

		x %= CHUNK_X;
		y %= CHUNK_Y;
		z %= CHUNK_Z;

		//m_ChunkList.push_back(new Chunk(glm::vec3(x,y,z), m_renderer, this, 0));

		//printf("Hit\n");
	}
	
}

/*
	Creates all the chunks, putting them in their correct positions
*/
void WorldManager::UpdateSetupList()
{
	// Used to give the chunks their ID and to taget chunks to create them
	int i = 0;

	// Repeat for World Size
	for (int x = 0; x < SECLECTED_WORLD_SIZE; x++)
	{
		// Repeat for World Size
		for (int z = 0; z < SECLECTED_WORLD_SIZE; z++)
		{
			// Pushback a chunk into the chunk vector
			// Pass in renderer, worldmanager, chunk id and coords
			m_ChunkList.push_back(new Chunk(glm::vec3(x * CHUNK_X, 0, z * CHUNK_Z), m_renderer, this, i));
			// Creates chunk
			//m_ChunkList.at(i)->CreateChunk();
			// Increment Loaded chunks
			chunksLoaded++;
			// Increment Chunk ID/Chunk Target variable
			i++;
		}
	}
}

/*
	A very hackish method to get really bad physics to work
	- Needs to be replaced with good system
	- Not recomended to use in other projects
*/
void WorldManager::UpdatePhysics(Player* player)
{
	// Perform a loop of all the chunks
	for (int i = 0; i < m_ChunkList.size(); ++i)
	{
		// Get the XYZ position of the player
		int x = (int)floor(player->GetPosition().x);
		int y = (int)floor(player->GetPosition().y);
		int z = (int)floor(player->GetPosition().z);

		// Grabs a chunk at the set XYZ coords
		Chunk* c = m_ChunkList[i]->GetChunk(x, y, z);

		// Check if the chunk exists
		if (c != NULL)
		{
			// If the block below the player is air, set the players position lower (Falling)
			if (c->GetBlock(glm::vec3(x, y - 1, z)) == BlockType::Air)
			{
				player->SetPosition(glm::vec3(player->GetPosition().x, player->GetPosition().y - 0.1f, player->GetPosition().z));
			}

			// If the block below the player is not air, set the players position higher (Inside block)
			if (c->GetBlock(glm::vec3(x, y, z)) != BlockType::Air)
			{
				player->SetPosition(glm::vec3(player->GetPosition().x, player->GetPosition().y + 1.0f, player->GetPosition().z));
			}
		}
		else
		{
			//player->SetPosition(glm::vec3(player->GetPosition().x, player->GetPosition().y - 0.2f, player->GetPosition().z));
		}
	}
}


/*
	Returns the blocktype of the block at the given XYZ Coords
	@param x y z World position to check for block
*/
BlockType WorldManager::GetBlock(int x, int y, int z)
{
	// Perform a loop of all the chunks
	for (int i = 0; i < m_ChunkList.size(); i++)
	{
		// Grabs a chunk at the set XYZ coords
		Chunk* c = m_ChunkList[i]->GetChunk(x, y, z);

		// Check if the chunk exists
		if (c != NULL)
		{
			// Convert the world coords into chunk coords 
			// TODO: Fix Rounding Error
			x %= CHUNK_X;
			y %= CHUNK_Y;
			z %= CHUNK_Z;

			// Returns The block type after adjusting world coords to local coords
			return c->GetBlock(x, y, z);
		}
		else
		{
			// If no chunk exists, return AIR
			return BlockType::Air;
		}
	}
}

Chunk* WorldManager::GetChunk(glm::vec3 pos)
{
	for (int i = 0; i < m_ChunkList.size(); i++)
	{
		// Grabs a chunk at the set XYZ coords
		return m_ChunkList[i]->GetChunk((int)pos.x, (int)pos.y, (int)pos.z);
	}

	return nullptr;
}

/*
	Places a block of the chosen type in the world
	@param x y z World position to place the block
	@param type What type of block to place
*/
void WorldManager::SetBlock(int x, int y, int z, BlockType type)
{
	// Perform a loop of all the chunks
	for (int i = 0; i < m_ChunkList.size(); i++)
	{
		// Grabs a chunk at the set XYZ coords
		Chunk* c = m_ChunkList[i]->GetChunk(x, y, z);

		// Check if the chunk exists
		if (c != NULL)
		{
			// Convert the world coords into chunk coords 
			// TODO: Fix Rounding Error
			x %= CHUNK_X;
			y %= CHUNK_Y;
			z %= CHUNK_Z;

			// Set the block within the chunk at the adjusted XYZ coords, also pass in the block-type
			c->SetBlock(x, y, z, type);
		}
	}
	
}
