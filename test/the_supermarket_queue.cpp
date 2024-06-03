#include <queue>
#include <vector>

long queueTime(std::vector<int> customers, int n) {
  std::priority_queue<long, std::vector<long>, std::greater<long>> min_queue;
  long ans = 0;
  for (int i = 0; i < n; ++i) min_queue.push(0);
  for (int i = 0; i < customers.size(); ++i) {
    long num = min_queue.top() + customers[i];
    min_queue.pop();
    min_queue.push(num);
    ans = std::max(ans, num);
  }
  return ans;
}
