#pragma once

class Memory
{
public:
	static Memory* Get();

	void StartRange();

	template<typename T, typename... Args>
	T* Allocate(Args&&... args)
	{
		void* ptr = malloc(sizeof(T));
		m_Allocations.emplace_back(ptr);

		return new (ptr) T(std::forward<Args>(args)...);
	}

	template<typename T>
	T* AllocateArray(size_t count)
	{
		void* ptr = malloc(sizeof(T) * count);
		m_Allocations.emplace_back(ptr);

		auto construct = reinterpret_cast<T*>(ptr);
		for (; construct != construct + count; construct++)
		{
			new (construct) T(std::forward<Args>(args)...);
		}

		return reinterpret_cast<T*>(ptr);
	}

private:
	Memory() = default;
	static Memory* s_Memory;

	std::vector<void*> m_Allocations;
};

class MemoryArena
{
public:
	MemoryArena();
	~MemoryArena();

	template<typename T, typename... Args>
	T* Allocate(Args&&... args)
	{
		ASSERT(m_Allocate + sizeof(T) < m_Data + 1024 * 1024, "Memory Arena full!");

		uint8_t* data = m_Allocate;
		m_Allocate += sizeof(T);
		return new (data) T(std::forward<Args>(args)...);
	}

	void Reset();

private:
	uint8_t* m_Data;
	uint8_t* m_Allocate;
};
