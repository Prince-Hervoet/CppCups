#include <cstddef>
#include <string>

char toUpper(char c) {
  if (c >= 'a' && c <= 'z') return c & (223);
  return c;
}

bool isLetter(char a) {
  if (a >= 'a' && a <= 'z') return true;
  if (a >= 'A' && a <= 'Z') return true;
  return false;
}

std::string to_camel_case(std::string text) {
  if (text.size() == 0) return "";
  std::string ans;
  bool has_first = false;
  bool is_word = false;
  for (size_t i = 0; i < text.size(); ++i) {
    char c = text[i];
    if (isLetter(c)) {
      if (!is_word && has_first) c = toUpper(c);
      is_word = true;
      ans.append(1, c);
    } else {
      if (is_word) {
        is_word = false;
        has_first = true;
      }
    }
  }
  return ans;
}

int main() {}