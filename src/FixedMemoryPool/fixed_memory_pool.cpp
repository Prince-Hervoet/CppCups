#include "fixed_memory_pool.hpp"

#include <cstddef>
#include <cstdlib>
#include <stdexcept>

namespace let_me_see {
void* FixedMemoryPool::GetMemory() {
  if (next_block == nullptr) {
    next_block = (FixedMemoryBlock*)createMemory();
    call_malloc_count += 1;
  }
  ImbedPtr* run = next_block->next_free;
  if (run) {
    next_block->next_free = run->next;
    return run;
  }
  FixedMemoryBlock* block_run = next_block;
  ImbedPtr* will = nullptr;
  while (block_run->next_block) {
    will = block_run->next_block->next_free;
    if (will == nullptr) {
      block_run = block_run->next_block;
      continue;
    }
    FixedMemoryBlock* temp = block_run->next_block;
    block_run->next_block = temp->next_block;
    temp->next_block = next_block;
    next_block = temp;
    return GetMemory();
  }
  block_run = (FixedMemoryBlock*)createMemory();
  call_malloc_count += 1;
  block_run->next_block = next_block;
  next_block = block_run;
  return GetMemory();
}

void FixedMemoryPool::FreeMemory(void* ptr) {
  if (ptr == nullptr)
    return;
  const MY_NUM_TYPE real_size = block_size + sizeof(FixedMemoryBlock);
  FixedMemoryBlock* run = next_block;
  while (run) {
    if ((size_t)ptr < (size_t)run + sizeof(FixedMemoryBlock) ||
        (size_t)ptr > (size_t)run + real_size) {
      run = run->next_block;
      continue;
    }
    break;
  }
  if (run == nullptr)
    return;
  ((ImbedPtr*)ptr)->next = run->next_free;
  run->next_free = (ImbedPtr*)ptr;
}

void* FixedMemoryPool::createMemory() {
  const MY_NUM_TYPE head_size = sizeof(FixedMemoryBlock);
  void* ptr = malloc(block_size + head_size);
  if (ptr == nullptr)
    throw std::invalid_argument("argument too large.");
  ((FixedMemoryBlock*)ptr)->next_block = nullptr;
  ImbedPtr* run = (ImbedPtr*)(((char*)ptr) + head_size);
  ((FixedMemoryBlock*)ptr)->next_free = run;
  for (MY_NUM_TYPE i = 0; i < ALLOC_RUN_COUNT - 1; ++i) {
    run->next = (ImbedPtr*)(((char*)run) + alloc_size);
    run = run->next;
  }
  run->next = nullptr;
  return ptr;
}

void FixedMemoryPool::Reset() {
  if (next_block == nullptr)
    return;
  FixedMemoryBlock* run = next_block;
  FixedMemoryBlock* temp = nullptr;
  while (run) {
    temp = run->next_block;
    free(run);
    run = temp;
  }
  next_block = nullptr;
}
}  // namespace let_me_see
