#ifndef _BASE_64_H_
#define _BASE_64_H_

#include <string>

namespace let_me_see {
std::string Base64Encode(std::string& target);
std::string Base64Decode(std::string& base64_str);
}  // namespace let_me_see

#endif