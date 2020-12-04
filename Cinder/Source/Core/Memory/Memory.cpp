#include "PCH.h"
#include "Memory.h"

#include "Core/Math/Color.h"

Memory* Memory::s_Memory = nullptr;

Memory* Memory::Get()
{
	if (!s_Memory)
	{
		s_Memory = new Memory;
	}

	return s_Memory;
}

Color* Memory::AllocateTextureData(uint32_t width, uint32_t height)
{
	Color* ret = new Color[width * height];
	m_Textures.emplace_back(ret);
	return ret;
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
	for (auto ptr : m_Textures)
	{
		delete[] ptr;
	}

	m_ScalarAllocations.clear();
	m_ArrayAllocations.clear();
	m_Textures.clear();
}
