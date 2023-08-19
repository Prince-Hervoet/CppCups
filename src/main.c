#include "cup_skiplist.h"

int main() {
  CupSkipList* sl = MakeCupSkipList();
  char a[] = "123";
  Insert(sl, a, NULL, 1);
  return 0;
}