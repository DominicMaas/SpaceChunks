#include "Block.h"

Block::Block()
{
}


Block::~Block()
{
}

/* 
	Returns if the block is active 
*/
bool Block::IsActive()
{
	return this->m_pActive;
}

/*
	// Sets block to active
*/
void Block::SetActive(bool active)
{
	this->m_pActive = active;
}

/*
	// Sets the block type
*/
void Block::SetBlockType(BlockType type)
{
	this->m_pBlockType = type;
}

/*
	// Returns the block type
*/
BlockType Block::GetBlockType()
{
	return this->m_pBlockType;
}