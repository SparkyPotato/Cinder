#include "PCH.h"
#include "Core/Memory/Memory.h"

Memory* Memory::s_Memory = nullptr;

Memory* Memory::Get()
{
	if (!s_Memory)
	{
		s_Memory = new Memory;
	}

	return s_Memory;
}

void Memory::StartRange()
{
	for (auto ptr : m_Allocations)
	{
		free(ptr);
	}
	m_Allocations.clear();
}

MemoryArena::MemoryArena()
{
	m_Data = reinterpret_cast<uint8_t*>(malloc(1024 * 1024));
	m_Allocate = m_Data;
}

MemoryArena::~MemoryArena()
{
	free(m_Data);
}

void MemoryArena::Reset()
{
	m_Allocate = m_Data;
}
