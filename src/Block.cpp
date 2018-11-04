#include "Block.hpp"

bool Block::active()
{
    return this->m_active;
}

void Block::setActive(bool active)
{
    this->m_active = active;
}

void Block::setType(BlockType type)
{
    this->m_type = type;
}

BlockType Block::type()
{
    return this->m_type;
}