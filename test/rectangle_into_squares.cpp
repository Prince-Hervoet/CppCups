#include <algorithm>
#include <vector>

class SqInRect {
 public:
  static std::vector<int> sqInRect(int lng, int wdth) {
    if (lng == wdth) return {};
    int m_value = 0;
    std::vector<int> ans;
    while (lng > 0 && wdth > 0) {
      m_value = std::min(lng, wdth);
      ans.push_back(m_value);
      if (lng > wdth)
        lng -= m_value;
      else
        wdth -= m_value;
    }
    return ans;
  }
};
