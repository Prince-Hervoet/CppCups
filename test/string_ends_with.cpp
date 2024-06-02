#include <cstddef>
#include <iostream>
#include <string>
bool solution(std::string const &str, std::string const &ending) {
  if (ending.size() == 0) return true;
  size_t a = str.size() - 1;
  size_t b = ending.size() - 1;
  while (a >= 0 && b >= 0) {
    std::cout << str[a] << " " << ending[b] << std::endl;
    if (str[a] != ending[b]) return false;
    if (a == 0 || b == 0) break;
    a--;
    b--;
  }
  return true;
}

int main() {
  std::cout << solution("abcde", "cde") << std::endl;
  return 0;
}