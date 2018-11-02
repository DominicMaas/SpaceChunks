#include "Chunk.hpp"
#include "GLM/gtc/noise.hpp"

BlockType Chunk::GetTheoreticalBlock(glm::vec3 pos)
{
	int chunkSeed = m_pRenderer->GetSeed();

	glm::vec3 grain0Offset(chunkSeed * 100, chunkSeed * 100, chunkSeed * 100);
	glm::vec3 grain1Offset(chunkSeed * 100, chunkSeed * 100, chunkSeed * 100);
	glm::vec3 grain2Offset(chunkSeed * 100, chunkSeed * 100, chunkSeed * 100);

	return GetTheoreticalBlock(pos, grain0Offset, grain1Offset, grain2Offset);
}

BlockType Chunk::GetTheoreticalBlock(glm::vec3 pos, glm::vec3 offset0, glm::vec3 offset1, glm::vec3 offset2)
{

	float heightBase = 10;
	float maxHeight = CHUNK_Y - 10;
	float heightSwing = maxHeight - heightBase;

	float mountainValue = CalculateNoiseValue(pos, offset1, 0.006f);

	float someOtherValue = CalculateNoiseValue(pos, offset0, 0.012f);

	

	mountainValue = sqrt(mountainValue);

	mountainValue -= someOtherValue;

	mountainValue *= heightSwing;
	mountainValue += heightBase;

	//caves?

	if (mountainValue >= pos.y)
		return BlockType::Grass;
	return BlockType::Air;

}

Chunk::Chunk(glm::vec3 pos, Engine *engine, World *world, int id)
{
	m_position = pos;
	m_pRenderer = engine;
	Chunk::m_pWorld = world;
	m_pChunkGlobalID = id;

	CreateChunk();
}

void Chunk::CreateChunk()
{
	m_pBlocks = new Block**[CHUNK_X];
	for (int i = 0; i < CHUNK_X; i++)
	{
		m_pBlocks[i] = new Block*[CHUNK_Y];

		for (int j = 0; j < CHUNK_Y; j++)
		{
			m_pBlocks[i][j] = new Block[CHUNK_Z];
		}
	}

	m_pChanged = true;
	m_pChunkID = glGenLists(1);

	m_pRenderer->LoadTexture(m_pTextureID, "img/textures.jpg");
	int chunkSeed = m_pRenderer->GetSeed();

	glm::vec3 grain0Offset(chunkSeed * 100, chunkSeed * 100, chunkSeed * 100);
	glm::vec3 grain1Offset(chunkSeed * 100, chunkSeed * 100, chunkSeed * 100);
	glm::vec3 grain2Offset(chunkSeed * 100, chunkSeed * 100, chunkSeed * 100);

	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				glm::vec3 pos(x, y, z);
				pos += m_position;

				m_pBlocks[x][y][z].SetActive(true);
				m_pBlocks[x][y][z].SetBlockType(GetTheoreticalBlock(pos, grain0Offset, grain1Offset, grain2Offset));
			}
		}
	}

	m_pLoaded = true;
}

glm::vec3 Chunk::GetCenter()
{
	return glm::vec3(m_position.x - (CHUNK_X / 2), m_position.y - (CHUNK_Y / 2), m_position.z - (CHUNK_Z / 2));
}

float Chunk::CalculateNoiseValue(glm::vec3 pos, glm::vec3 offset, float scale)
{
	float noiseX = abs((pos.x + offset.x) * scale);
	float noiseY = abs((pos.y + offset.y) * scale);
	float noiseZ = abs((pos.z + offset.z) * scale);

	return std::max(0.0f, glm::simplex(glm::vec3(noiseX, noiseY, noiseZ)));
}

void Chunk::Rebuild()
{
	m_pChanged = false;

	glNewList(m_pChunkID, GL_COMPILE);
	glBegin(GL_QUADS);

	int faces = 0;

	for (int _x = 0; _x < CHUNK_X; _x++)
	{
		for (int _y = 0; _y < CHUNK_Y; _y++)
		{
			for (int _z = 0; _z < CHUNK_Z; _z++)
			{
				if (m_pBlocks[_x][_y][_z].GetBlockType() == BlockType::Air)
					continue;

				float x = (float)_x;
				float y = (float)_y;
				float z = (float)_z;
				/*
				float texCoordsDirt[8] = 
				{
				//  X     Y
					0.0f, 1.0f,
					0.0f, 0.0f,
					0.0625f, 0.0f,
					0.0625f, 1.0f			
				};

				float texCoordsGrass[8] =
				{
					//  X     Y
					0.125f, 1.0f,
					0.125f, 0.0f,
					0.1875f, 0.0f,
					0.1875f, 1.0f
				};

				float texCoordsSide[8] =
				{
					//  X     Y
					0.0625f, 1.0f,
					0.0625f, 0.0f,
					0.125f, 0.0f,
					0.125f, 1.0f
				};
				*/

				float texCoordsDirt[8] =
				{
					//  X     Y
					0.125f, 1.0f,
					0.125f, 0.0f,
					0.1875f, 0.0f,
					0.1875f, 1.0f
				};

				float texCoordsGrass[8] =
				{
					//  X     Y
					0.125f, 1.0f,
					0.125f, 0.0f,
					0.1875f, 0.0f,
					0.1875f, 1.0f
				};

				float texCoordsSide[8] =
				{
					//  X     Y
					0.125f, 1.0f,
					0.125f, 0.0f,
					0.1875f, 0.0f,
					0.1875f, 1.0f
				};

				if (IsTransparent(_x, _y, _z - 1))
				{
					glNormal3f(0.0f, 0.0f, -1.0f);
					glTexCoord2f(texCoordsSide[0], texCoordsSide[1]);
					glVertex3f(x + 1.0f, y, z);
					glTexCoord2f(texCoordsSide[2], texCoordsSide[3]);
					glVertex3f(x, y, z);
					glTexCoord2f(texCoordsSide[4], texCoordsSide[5]);
					glVertex3f(x, y + 1.0f, z);
					glTexCoord2f(texCoordsSide[6], texCoordsSide[7]);
					glVertex3f(x + 1.0f, y + 1.0f, z);
					faces++;
				}

				if (IsTransparent(_x, _y, _z + 1))
				{
					glNormal3f(0.0f, 0.0f, 1.0f);
					glTexCoord2f(texCoordsSide[0], texCoordsSide[1]);
					glVertex3f(x, y, z + 1.0f);
					glTexCoord2f(texCoordsSide[2], texCoordsSide[3]);
					glVertex3f(x + 1.0f, y, z + 1.0f);
					glTexCoord2f(texCoordsSide[4], texCoordsSide[5]);
					glVertex3f(x + 1.0f, y + 1.0f, z + 1.0f);
					glTexCoord2f(texCoordsSide[6], texCoordsSide[7]);
					glVertex3f(x, y + 1.0f, z + 1.0f);
					faces++;
				}
				if (IsTransparent(_x + 1, _y, _z))
				{
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2f(texCoordsSide[0], texCoordsSide[1]);
					glVertex3f(x + 1.0f, y, z + 1.0f);
					glTexCoord2f(texCoordsSide[2], texCoordsSide[3]);
					glVertex3f(x + 1.0f, y, z);
					glTexCoord2f(texCoordsSide[4], texCoordsSide[5]);
					glVertex3f(x + 1.0f, y + 1.0f, z);
					glTexCoord2f(texCoordsSide[6], texCoordsSide[7]);
					glVertex3f(x + 1.0f, y + 1.0f, z + 1.0f);
					faces++;
				}

				if (IsTransparent(_x - 1, _y, _z))
				{
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2f(texCoordsSide[0], texCoordsSide[1]);
					glVertex3f(x, y, z);
					glTexCoord2f(texCoordsSide[2], texCoordsSide[3]);
					glVertex3f(x, y, z + 1.0f);
					glTexCoord2f(texCoordsSide[4], texCoordsSide[5]);
					glVertex3f(x, y + 1.0f, z + 1.0f);
					glTexCoord2f(texCoordsSide[6], texCoordsSide[7]);
					glVertex3f(x, y + 1.0f, z);
					faces++;
				}

				if (IsTransparent(_x, _y - 1, _z))
				{
					glNormal3f(0.0f, -1.0f, 0.0f);
					glTexCoord2f(texCoordsDirt[0], texCoordsDirt[1]);
					glVertex3f(x, y, z);
					glTexCoord2f(texCoordsDirt[2], texCoordsDirt[3]);
					glVertex3f(x + 1.0f, y, z);
					glTexCoord2f(texCoordsDirt[4], texCoordsDirt[5]);
					glVertex3f(x + 1.0f, y, z + 1.0f);
					glTexCoord2f(texCoordsDirt[6], texCoordsDirt[7]);
					glVertex3f(x, y, z + 1.0f);
					faces++;
				}

				if (IsTransparent(_x, _y + 1, _z))
				{
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2f(texCoordsGrass[0], texCoordsGrass[1]);
					glVertex3f(x + 1.0f, y + 1.0f, z);
					glTexCoord2f(texCoordsGrass[2], texCoordsGrass[3]);
					glVertex3f(x, y + 1.0f, z);
					glTexCoord2f(texCoordsGrass[4], texCoordsGrass[5]);
					glVertex3f(x, y + 1.0f, z + 1.0f);
					glTexCoord2f(texCoordsGrass[6], texCoordsGrass[7]);
					glVertex3f(x + 1.0f, y + 1.0f, z + 1.0f);
					faces++;
				}
			}
		}
	}

	glEnd();
	glEndList();
}

glm::vec3 Chunk::GetPosition()
{
	return m_position;
}

void Chunk::Render()
{		
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	m_pRenderer->PushMatrix();
	m_pRenderer->TranslateWorldMatrix(m_position);

	m_pRenderer->BindTexture(m_pTextureID, 1);
		glCallList(m_pChunkID);

	m_pRenderer->PopMatrix();

	glDisable(GL_TEXTURE_2D);
}

bool Chunk::IsTransparent(int x, int y, int z)
{
	if (y < 0) return false;

	uint16_t block = GetBlock(x, y, z);
	switch (block)
	{
	default:
	case 0:
		return true;

	case 1: return false;
	}
}

Chunk* Chunk::FindChunk(glm::vec3 pos)
{
	for (int i = 0; i < m_pWorld->GetChunkList().size(); i++)
	{
		glm::vec3 cpos = m_pWorld->GetChunkList().at(i)->GetPosition();

		if ((m_position.x < cpos.x) || (m_position.z < cpos.z) || (m_position.x >= cpos.x + CHUNK_X) || (m_position.z >= cpos.z + CHUNK_Z))
		{
			return m_pWorld->GetChunkList().at(i);
		}
		else
		{
			return NULL;
		}
		return NULL;	
	}
	return NULL;
}


BlockType Chunk::GetBlock(int x, int y, int z)
{
	if ((y < 0) || (y >= CHUNK_Y))
		return BlockType::Air;
	
	
	if ((x < 0) || (z < 0) || (x >= CHUNK_X) || (z >= CHUNK_Z))
	{
		glm::vec3 worldPos(x, y, z); 
		worldPos += m_position;

		Chunk* chunk = Chunk::FindChunk(worldPos);

		if (chunk == this)
		{
			return BlockType::Air;
		}		

		if (chunk == NULL)
		{
			return GetTheoreticalBlock(worldPos);
		}

		return chunk->GetBlock(worldPos);
 
		return BlockType::Air;
	}
	
	
	return m_pBlocks[x][y][z].GetBlockType();
}

BlockType Chunk::GetBlock(glm::vec3 pos)
{
	pos -= m_position;

	int x = (int)floorf(pos.x);
	int y = (int)floorf(pos.y);
	int z = (int)floorf(pos.z);
	return GetBlock(x, y, z);

}

void Chunk::SetBlock(int x, int y, int z, BlockType type)
{
	m_pBlocks[x][y][z].SetBlockType(type);
	m_pChanged = true;
}

bool Chunk::ShouldRebuildChunk()
{
	return m_pChanged;
}

bool Chunk::IsChunkLoaded()
{
	return m_pLoaded;
}

Chunk* Chunk::GetChunk(int x, int y, int z)
{
	bool chunkCheck[6];

	float x1 = GetPosition().x;
	float y1 = GetPosition().y;
	float z1 = GetPosition().z;

	float x2 = GetPosition().x + CHUNK_X;
	float y2 = GetPosition().y + CHUNK_Y;
	float z2 = GetPosition().z + CHUNK_Z;

	if (x >= x1)
	{
		chunkCheck[0] = true;
	}
	else
	{
		chunkCheck[0] = false;
	}

	if (x <= x2)
	{
		chunkCheck[1] = true;
	}
	else{
		chunkCheck[1] = false;
	}

	if (y >= y1)
	{
		chunkCheck[2] = true;
	}
	else
	{
		chunkCheck[2] = false;
	}

	if (y <= y2)
	{
		chunkCheck[3] = true;
	}
	else{
		chunkCheck[3] = false;
	}

	if (z >= z1)
	{
		chunkCheck[4] = true;
	}
	else
	{
		chunkCheck[4] = false;
	}

	if (z <= z2)
	{
		chunkCheck[5] = true;
	}
	else{
		chunkCheck[5] = false;
	}

	if (chunkCheck[0] && chunkCheck[1] && chunkCheck[2] && chunkCheck[3] && chunkCheck[4] && chunkCheck[5])
	{
		return this;
	}
	else
	{
		return NULL;
	}
}

Chunk::~Chunk()
{
	m_pLoaded = false;
	glDeleteLists(m_pChunkID, 1);
	
	for (int i = 0; i < CHUNK_X; ++i)
	{
		for (int j = 0; j < CHUNK_Y; ++j)
		{
			delete[] m_pBlocks[i][j];
		}

		delete[] m_pBlocks[i];
	}
	delete[] m_pBlocks;

}

