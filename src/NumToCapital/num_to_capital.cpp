#include "num_to_capital.hpp"

#include <array>
#include <stack>

namespace let_me_see {

const std::array<std::string, 3> kCapitalUnits{u8"拾", u8"佰", u8"仟"};
const std::array<std::string, 2> kCapitalBigUnits{u8"万", u8"亿"};
const std::array<std::string, 10> kCapitalNumbers{
    u8"零", u8"壹", u8"贰", u8"叁", u8"肆",
    u8"伍", u8"陆", u8"柒", u8"捌", u8"玖"};

static bool isTrue(bool value) { return value; }
static bool isFalse(bool value) { return !value; }

std::vector<std::string> NumToCapital(long long num) {
  if (num == 0) return {kCapitalUnits[0]};
  const int capital_units_size = kCapitalUnits.size();
  const int capital_big_units_size = kCapitalBigUnits.size();
  std::stack<std::string> result_stack;
  int num_cache[] = {0, 0, 0, 0};
  int big_index = -1;  // 万以上的索引
  bool is_last_zero = false;
  while (num > 0) {
    int i = 0;
    bool is_all_zero = true;
    int base_index = -1;  // 十百千索引
    for (i = 0; (i < 4 && num > 0); ++i) {
      num_cache[i] = num % 10;
      if (num_cache[i] > 0) is_all_zero = false;
      num /= 10;
    }

    if (big_index >= 0 && isFalse(is_all_zero))
      result_stack.push(kCapitalBigUnits[big_index]);
    big_index += 1;
    if (big_index == capital_big_units_size) big_index = 0;

    for (int p = 0; p < i; ++p, ++base_index) {
      int num = num_cache[p];
      if (base_index >= 0 && num > 0)
        result_stack.push(kCapitalUnits[base_index]);
      if (num > 0 || (p > 0 && num != num_cache[p - 1]))
        result_stack.push(kCapitalNumbers[num]);
    }
  }

  std::vector<std::string> result;
  while (result_stack.size() > 0) {
    result.push_back(result_stack.top());
    result_stack.pop();
  }
  return result;
}
}  // namespace let_me_see