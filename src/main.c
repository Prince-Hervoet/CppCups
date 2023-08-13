#include "cup_string.h"

int main() {
  CupString* cs = MakeCupString();
  int len = Len(cs);
  FreeCupString(cs);
  return 0;
}