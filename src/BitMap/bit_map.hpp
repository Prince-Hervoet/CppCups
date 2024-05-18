#pragma once

#include <cstddef>
#include <vector>

namespace let_me_see {

#define FLAG 32
#define CAL_ARRAY_SIZE(x) (((x) + 31) / FLAG)

class BitMap {
 private:
  std::vector<uint32_t> bits;
  size_t bit_size = 0;

 public:
  BitMap() {}
  explicit BitMap(size_t bit_size)
      : bit_size(bit_size), bits(CAL_ARRAY_SIZE(bit_size), 0) {}

 public:
  void Set(size_t pos) {
    if (pos >= bit_size) resizeBits(pos + 1);
    bits[pos / FLAG] |= (1 << (pos % FLAG));
  }

  void Reset(size_t pos) {
    if (pos >= bit_size) return;
    bits[pos / FLAG] &= ~(1 << (pos % FLAG));
  }

  int Get(size_t pos) const {
    if (pos >= bit_size) return -1;
    if ((bits[pos / FLAG] & (1 << (pos % FLAG))) == 0) return 0;
    return 1;
  }

  size_t Size() const { return bit_size; }

 private:
  void resizeBits(size_t new_size) {
    size_t new_size_in_bits = CAL_ARRAY_SIZE(new_size);
    bits.resize(new_size_in_bits, 0);
    bit_size = new_size;
  }
};
}  // namespace let_me_see