#include <vector>

std::vector<int> tribonacci(std::vector<int> signature, int n) {
  std::vector<int> ans;
  if (n == 0) return ans;
  ans.push_back(signature[0]);
  if (n == 1) return ans;
  ans.push_back(signature[1]);
  if (n == 2) return ans;
  ans.push_back(signature[2]);
  if (n == 3) return ans;
  for (int i = 3; i < n; ++i) {
    ans.push_back((ans[i - 1] + ans[i - 2] + ans[i - 3]));
  }
  return ans;
}