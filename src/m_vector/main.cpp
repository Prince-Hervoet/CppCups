#include "m_vector.hpp"

int main() {
  letMeSee::Mvector<int> mv;
  mv.PushBack(1239);
  mv.PushBack(1223);
  mv.PushBack(1239343);
  std::cout << mv.Get(0) << std::endl;
  std::cout << mv.Capacity() << std::endl;
  return 0;
}