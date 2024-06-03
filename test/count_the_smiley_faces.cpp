#include <vector>

int countSmileys(std::vector<std::string> arr) {
  int ans = 0;
  for (int i = 0; i < arr.size(); ++i) {
    std::string& str = arr[i];
    if (str.size() < 2 || str.size() > 3) continue;
    if (str.size() == 2) {
      if (str[0] != ';' && str[0] != ':') continue;
      if (str[1] != ')' && str[1] != 'D') continue;
    } else if (str.size() == 3) {
      if (str[0] != ';' && str[0] != ':') continue;
      if (str[1] != '-' && str[1] != '~') continue;
      if (str[2] != ')' && str[2] != 'D') continue;
    }
    ans++;
  }
  return ans;
}