#include <map>
#include <vector>

std::pair<std::size_t, std::size_t> two_sum(const std::vector<int>& numbers,
                                            int target) {
  std::map<int, int> mp;
  int a = 0, b = 0;
  for (int i = 0; i < numbers.size(); ++i) {
    int remain = target - numbers[i];
    if (mp.count(remain) == 0) {
      mp.insert({numbers[i], i});
      continue;
    }
    a = i;
    b = mp[remain];
    break;
  }
  return {a, b};
}