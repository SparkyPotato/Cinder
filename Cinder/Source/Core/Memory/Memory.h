#pragma once

#include "Core/Components/Component.h"

class Color;

class Memory
{
public:
	static Memory* Get();

	void StartProject();

	template<typename T, typename... Args>
	T* Allocate(Args&&... args)
	{
		auto p = new T(std::forward<Args>(args)...);
		m_ScalarAllocations.emplace_back(p);
		return p;
	}

	template<typename T>
	T* AllocateArr(size_t count)
	{
		auto p = new T[count];
		m_ArrayAllocations.emplace_back(p);
		return p;
	}
	
	Color* AllocateTextureData(uint32_t width, uint32_t height);

private:
	Memory() = default;

	static Memory* s_Memory;

	std::vector<Component*> m_ScalarAllocations;
	std::vector<Component*> m_ArrayAllocations;
	std::vector<Color*> m_Textures;
};
