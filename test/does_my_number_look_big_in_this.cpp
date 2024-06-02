bool narcissistic(int value) {
  int len = 0, ans = 0, temp = 1;
  int n_value = value;
  while (n_value > 0) {
    n_value /= 10;
    len++;
  }
  n_value = value;
  while (n_value > 0) {
    int a = n_value % 10;
    n_value /= 10;
    for (int i = 0; i < len; ++i) temp = temp * a;
    ans += temp;
    temp = 1;
  }
  return ans == value;
}