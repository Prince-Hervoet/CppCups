#pragma once

#include <stdint.h>
#include <stdlib.h>
static char heap[1024];

void InitMemBlock(char* ptr, uint64_t size, uint32_t count, uint32_t perSize);

void writeInBlock(char* run, uint64_t prevIndex, uint64_t nextIndex,
                  uint64_t memSize);