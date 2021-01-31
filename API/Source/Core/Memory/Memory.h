//    Copyright 2021 Shaye Garg
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#pragma once

constexpr size_t BLOCK_SIZE = 1024 * 1024; // 1 MB blocks

/// Memory Arena for fast allocation/
class MemoryArena
{
public:
	MemoryArena();
	~MemoryArena();

	/// Reset the arena for reuse.
	void Reset();

	/// Allocate an object.
	///
	/// \tparam T Any type with a size < 1MB.
	/// \param args Constructor arguments.
	/// 
	/// \return The created object.
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

	/// Allocate an array of objects.
	///
	/// \tparam T Any type.
	/// \param count The size of array. sizeof(T) * count must be < 1 MB
	/// 
	/// \return The created array.
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
