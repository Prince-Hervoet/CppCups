#include <iostream>

namespace let_me_see {

static const char* kBase64Chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char kVar1 = 252;
static const unsigned char kVar0x3 = 3;
static const unsigned char kVar3 = 240;
static const unsigned char kVar0xF = 0xF;
static const unsigned char kVar5 = 192;
static const unsigned char kVar6 = 63;
static const char kAssignChar = '=';

static const unsigned char kVar0x30 = 0x30;
static const unsigned char kVar0x3c = 0x3C;

static inline bool isBase64Char(char c) {
  if ((c >= '/' && c <= '9') || (c == '+') || (c >= 'A' && c <= 'Z') ||
      (c >= 'a' && c <= 'z'))
    return true;
  return false;
}

std::string Base64Encode(const std::string& target) {
  if (target.length() == 0) return "";
  std::string result;
  int str_len = target.length();
  unsigned char aux_var = 0;
  for (int i = 0; i < str_len; i = i + 3) {
    unsigned char current_char = target.at(i);

    aux_var = ((kVar1 & current_char) >> 2);
    result.append(1, kBase64Chars[aux_var]);

    aux_var ^= aux_var;
    aux_var = ((kVar0x3 & current_char) << 4);
    if (i + 1 >= str_len) {
      result.append(1, kBase64Chars[aux_var]);
      result.append(2, kAssignChar);
      break;
    }
    current_char = target.at(i + 1);
    aux_var = (aux_var | ((kVar3 & current_char) >> 4));
    result.append(1, kBase64Chars[aux_var]);

    aux_var ^= aux_var;
    aux_var = ((kVar0xF & current_char) << 2);
    if (i + 2 >= str_len) {
      result.append(1, kBase64Chars[aux_var]);
      result.append(1, kAssignChar);
      break;
    }
    current_char = target.at(i + 2);
    aux_var = (aux_var | ((kVar5 & current_char) >> 6));
    result.append(1, kBase64Chars[aux_var]);

    aux_var ^= aux_var;
    aux_var = (kVar6 & current_char);
    result.append(1, kBase64Chars[aux_var]);

    aux_var ^= aux_var;
  }
  return result;
}

std::string Base64Decode(std::string& base64_str) {
  if (base64_str.length() == 0) return "";
  int str_len = base64_str.length();
  if (str_len % 4 != 0) throw "It's not a vaild base64 string.";
  unsigned char aux_var = 0;
  std::string result;
  for (int i = 0; i < str_len; i = i + 4) {
    unsigned char current_char = base64_str.at(i);
    if (!isBase64Char(current_char)) throw "It's not a vaild base64 string.";
    aux_var = ((aux_var | current_char) << 2);

    current_char = base64_str.at(i + 1);
    if (!isBase64Char(current_char)) throw "It's not a vaild base64 string.";
    aux_var = (aux_var | ((kVar0x30 & current_char) >> 4));
    result.append(1, aux_var);
    aux_var ^= aux_var;

    aux_var = ((kVar0xF | current_char) << 4);
    current_char = base64_str.at(i + 2);
    if (!isBase64Char(current_char)) throw "It's not a vaild base64 string.";
    aux_var = (aux_var | ((kVar0x3c & current_char) >> 2));
    result.append(1, aux_var);

    aux_var ^= aux_var;

    aux_var = ((kVar0x3 & current_char) << 6);
    current_char = base64_str.at(i + 3);
    if (!isBase64Char(current_char)) throw "It's not a vaild base64 string.";
    aux_var = (aux_var | current_char);
    result.append(1, aux_var);
  }
  return result;
}
}  // namespace let_me_see

int main() {
  std::string result = let_me_see::Base64Encode(
      "Aaslkdefj2lkkhjsddoisahjdsf01%^&%^&$okjasdsdfljk12089udfokhasdf;''''p[[]"
      "skkdjfBCD");
  std::cout << result << std::endl;
  return 0;
}