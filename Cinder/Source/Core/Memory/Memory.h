#pragma once

class MemoryArena
{
public:
	MemoryArena();
	MemoryArena(size_t size);
	~MemoryArena();

	template<typename T, typename... Args>
	T* Allocate(Args&&... args)
	{
		ASSERT(m_Data + m_Used + sizeof(T) < m_Data + m_Size, "Memory Arena full!");

		uint8_t* data = m_Data + std::atomic_fetch_add(&m_Used, sizeof(T));
		return new (data) T(std::forward<Args>(args)...);
	}

	void Reset();

private:
	uint8_t* m_Data;
	std::atomic<size_t> m_Used;
	size_t m_Size;
};
