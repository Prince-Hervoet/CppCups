#include <map>
#include <vector>

class Same {
 public:
  static bool comp(std::vector<int>& a, std::vector<int>& b) {
    std::map<int, int> mp;
    for (int i = 0; i < b.size(); ++i) {
      int bi = b[i];
      if (mp.count(bi) == 0)
        mp.insert({bi, 1});
      else
        mp[bi] += 1;
    }
    for (int i = 0; i < a.size(); ++i) {
      int s = a[i] * a[i];
      if (mp.count(s) == 0) return false;
      mp[s] -= 1;
      if (mp[s] == 0) mp.erase(s);
    }
    if (mp.size() > 0) return false;
    return true;
  }
};
