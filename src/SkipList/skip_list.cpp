#include <random>

static std::random_device rd;

// 生成 A 到 B 之间的随机整数
static int getRandomNumber(int start, int end) {
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(start, end);
  return dis(gen);
}