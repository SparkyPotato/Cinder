#pragma once

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

private:
	Memory() = default;

	static Memory* s_Memory;

	std::vector<void*> m_ScalarAllocations;
	std::vector<void*> m_ArrayAllocations;
};
