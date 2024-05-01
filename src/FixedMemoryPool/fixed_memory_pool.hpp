#ifndef _FIXED_MEMORY_POOL_H_
#define _FIXED_MEMORY_POOL_H_
#define MY_NUM_TYPE unsigned int
#define ALLOC_RUN_COUNT 20

namespace let_me_see {
typedef struct ImbedPtr {
  struct ImbedPtr* next;
} ImbedPtr;

class FixedMemoryBlock {
 public:
  FixedMemoryBlock* next_block = nullptr;
  ImbedPtr* next_free = nullptr;
};

class FixedMemoryPool {
 private:
  MY_NUM_TYPE call_malloc_count = 0;  // 调用malloc的次数
  MY_NUM_TYPE alloc_size = 0;         // 每次调用要申请的内存大小
  MY_NUM_TYPE block_size = 0;         // 底层申请的内存块大小
  FixedMemoryBlock* next_block = nullptr;  // 指向下一个block

 public:
  FixedMemoryPool() {}
  FixedMemoryPool(MY_NUM_TYPE alloc_size) : alloc_size(alloc_size) {
    block_size = ALLOC_RUN_COUNT * alloc_size;
  }

  void* GetMemory();

  void FreeMemory(void* ptr);

  void Reset();

 private:
  void* createMemory();
};
}  // namespace let_me_see

#endif