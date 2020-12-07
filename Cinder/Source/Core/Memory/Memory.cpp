#include "PCH.h"
#include "Core/Memory/Memory.h"

MemoryArena::MemoryArena()
{
	m_Block = new Block;
	m_AllocationBlock = m_Block;
}

MemoryArena::~MemoryArena()
{
	Block* block = m_Block;
	while (block)
	{
		Block* prev = block;
		block = block->Next;
		delete prev;
	}
}

void MemoryArena::Reset()
{
	Block* block = m_Block;
	while (block)
	{
		block->Pointer = block->Data;
		block = block->Next;
	}
}
