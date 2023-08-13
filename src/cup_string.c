#include "cup_string.h"

#include <stdlib.h>

#define EXPAND_FLAG 1024
#define INIT_CAPACITY 16

CupString* MakeCupString() {
  CupString* cs = (CupString*)malloc(sizeof(CupString));
  cs->len = 0;
  cs->capacity = INIT_CAPACITY;
  cs->data = (char*)malloc(INIT_CAPACITY);
  return cs;
}

int Len(CupString* cs) {
  if (cs == NULL) return 0;
  return cs->len;
}

int Capacity(CupString* cs) {
  if (cs == NULL) return 0;
  return cs->capacity;
}

void ClearCupString(CupString* cs) {
  if (cs == NULL) return;
  if (cs->data != NULL) free(cs->data);
  cs->len = 0;
  cs->data = (char*)malloc(cs->capacity);
}

void FreeCupString(CupString* cs) {
  if (cs == NULL) return;
  if (cs->data != NULL) free(cs->data);
  free(cs);
}

void Append(CupString* cs, char c) {
  if (cs == NULL) return;
  if (cs->len == cs->capacity) {
    expand(cs, -1);
  }
  (cs->data)[cs->len] = c;
  cs->len += 1;
}

void Set(CupString* cs, int index, char c) {
  if (cs == NULL) return;
  if (index < 0 || index >= cs->len) {
    return 0;
  }
  (cs->data)[index] = c;
}

void CopyCharPtr(CupString* cs, char* str, int size) {
  if (str == NULL || size == 0) return;
  for (int i = 0; i < size; i++) {
    Append(cs, str[i]);
  }
}

char At(CupString* cs, int index) {
  if (cs == NULL) return 0;
  if (index < 0 || index >= cs->len) {
    return 0;
  }
  return (cs->data)[index];
}

static void expand(CupString* cs, int newCapacity) {
  if (cs == NULL) return;
  int newCapacityA = 0;
  if (newCapacity == -1) {
    if (cs->capacity < EXPAND_FLAG) {
      newCapacityA = ((cs->capacity) << 1);
    } else {
      newCapacityA = (int)((cs->capacity) * 1.5);
    }
  } else {
    newCapacityA = newCapacity >= cs->len ? newCapacity : cs->len;
  }
  char* newSpace = (char*)malloc(newCapacityA);
  for (int i = 0; i < (cs->len); i++) {
    newSpace[i] = (cs->data)[i];
  }
  cs->capacity = newCapacityA;
  free(cs->data);
  cs->data = newSpace;
}