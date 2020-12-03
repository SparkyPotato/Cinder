#include "PCH.h"
#include "Memory.h"

Memory* Memory::s_Memory = nullptr;

Memory* Memory::Get()
{
	if (!s_Memory)
	{
		s_Memory = new Memory;
	}

	return s_Memory;
}

void Memory::StartProject()
{
	for (auto ptr : m_ScalarAllocations)
	{
		delete ptr;
	}
	for (auto ptr : m_ArrayAllocations)
	{
		delete[] ptr;
	}

	m_ScalarAllocations.clear();
	m_ArrayAllocations.clear();
}
