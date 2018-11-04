#pragma once

// All the block types in the game
enum BlockType {
	Air = 0,
	Grass,
	Dirt,
	NumTypes,
};

class Block {
public:
	bool active();
	void setActive(bool active);

	BlockType type();
	void setType(BlockType type);	
private:
	bool m_active;
	BlockType m_type;
};