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

#include "PCH.h"
#include "Core/Memory/Memory.h"

MemoryArena::MemoryArena()
{
	m_Block = new Block;
	m_AllocationBlock = m_Block;
}

MemoryArena::~MemoryArena()
{
	Block* block = m_Block;
	while (block)
	{
		Block* prev = block;
		block = block->Next;
		delete prev;
	}
}

void MemoryArena::Reset()
{
	m_AllocationBlock = m_Block;
	Block* block = m_Block;
	while (block)
	{
		block->Pointer = block->Data;
		block = block->Next;
	}
}
