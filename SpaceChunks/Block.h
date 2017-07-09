#pragma once

/*
	Enum of all the block types in the game
*/
enum BlockType
{
	Air = 0,

	Grass,
	Dirt,

	NumTypes,
};

/*
	Block Class, used to store information about the current block 
*/
class Block
{
private:
	// Is the block Active
	bool m_pActive;
	// The blocks light level (Coming Soon)
	float m_pLightLevel;
	// What type of block it is
	BlockType m_pBlockType;
public:
	Block();
	~Block();

	// Returns if the block is active
	bool IsActive();
	// Returns the block type
	BlockType GetBlockType();

	// Sets block to active
	void SetActive(bool active);
	// Sets the block type
	void SetBlockType(BlockType type);
};

