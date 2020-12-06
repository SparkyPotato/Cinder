#include "PCH.h"
#include "Core/Memory/Memory.h"

MemoryArena::~MemoryArena()
{
	Block* block = m_Block.Next;
	while (block)
	{
		Block* prev = block;
		block = block->Next;
		delete prev;
	}
}

void MemoryArena::Reset()
{
	Block* block = &m_Block;
	while (block)
	{
		block->Pointer = block->Data;
		block = block->Next;
	}
}
