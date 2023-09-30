#pragma once
#include <string.h>

typedef struct {
  int len;       // 长度
  int capacity;  // 容量
  char *data;
} CupString;

CupString *MakeCupString();
int Len(CupString *cs);
int Capacity(CupString *cs);
void ClearCupString(CupString *cs);
void FreeCupString(CupString *cs);
void Append(CupString *cs, char c);
void Set(CupString *cs, int index, char c);
void CopyCharPtr(CupString *cs, char *str, int size);
char At(CupString *cs, int index);

static void expand(CupString *cs, int newCapacity);