#include <string>

bool isLetter(char c) {
  if (c >= 'a' && c <= 'z') return true;
  if (c >= 'A' && c <= 'Z') return true;
  return false;
}

std::string pig_it(std::string str) {
  bool is_word = false;
  std::string ans;
  int len = 0, i = 0;
  for (i = 0; i < str.size(); ++i) {
    char c = str[i];
    if (isLetter(c)) {
      is_word = true;
      len++;
    } else if (c == ' ') {
      if (is_word) {
        for (int k = i - len + 1; k < i; ++k) ans.append(1, str[k]);
        ans.append(1, str[i - len]);
        ans.append("ay");
        len = 0;
      }
      ans.append(1, c);
      is_word = false;
    } else {
      ans.append(1, c);
    }
  }
  if (is_word) {
    for (int k = i - len + 1; k < i; ++k) ans.append(1, str[k]);
    ans.append(1, str[i - len]);
    ans.append("ay");
  }
  return ans;
}