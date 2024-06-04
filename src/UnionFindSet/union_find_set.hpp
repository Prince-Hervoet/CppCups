#ifndef _UNION_FIND_SET_H_
#define _UNION_FIND_SET_H_

namespace let_me_see {
class UnionFindSet {
  int* buffer = nullptr;

  UnionFindSet(int length) { buffer = new int[length]; }
};
}  // namespace let_me_see

#endif