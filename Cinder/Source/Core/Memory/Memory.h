#pragma once

constexpr size_t BLOCK_SIZE = 1024 * 1024; // 1 MB blocks

class MemoryArena
{
public:
	MemoryArena();
	~MemoryArena();

	void Reset();

	template<typename T, typename... Args>
	T* Allocate(Args&&... args)
	{
		size_t size = sizeof(T);
		size = ((size + 15) & (~15)); // Align size to 16 bytes

		ASSERT(size < BLOCK_SIZE, "Allocation too large.");

		if (m_AllocationBlock->Pointer + size > m_AllocationBlock->Data + BLOCK_SIZE)
		{
			if (!m_AllocationBlock->Next)
			{
				Verbose("Expanding memory arena.");
				m_AllocationBlock->Next = new Block;
			}
			m_AllocationBlock = m_AllocationBlock->Next;
		}

		auto ptr = new(m_AllocationBlock->Pointer) T(std::forward<Args>(args)...);
		m_AllocationBlock->Pointer += size;

		return ptr;
	}

	template<typename T>
	T* AllocateArray(size_t count)
	{
		size_t size = sizeof(T) * count;
		size = ((size + 15) & (~15)); // Align size to 16 bytes

		ASSERT(size < BLOCK_SIZE, "Allocation too large.");

		if (m_AllocationBlock->Pointer + size > m_AllocationBlock->Data + BLOCK_SIZE)
		{
			Verbose("Expanding memory arena.");
			m_AllocationBlock->Next = new Block;
			m_AllocationBlock = m_AllocationBlock->Next;
		}

		T* ptr = new(m_AllocationBlock->Pointer) T();
		for (size_t i = 1; i < count; i++)
		{
			new(&ptr[i]) T();
		}

		m_AllocationBlock->Pointer += size;

		return ptr;
	}

private:
	struct Block
	{
		uint8_t Data[BLOCK_SIZE];
		uint8_t* Pointer = Data;
		Block* Next = nullptr;
	};

	Block* m_Block;
	Block* m_AllocationBlock = nullptr;
};
