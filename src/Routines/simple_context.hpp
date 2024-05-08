#ifndef _SIMPLE_CONTEXT_H_
#define _SIMPLE_CONTEXT_H_

#define REGISTER_TYPE_NAME void*
#define REGISTER_MESSAGES     \
  REGISTER_TYPE_NAME rsp = 0; \
  REGISTER_TYPE_NAME rbp = 0; \
  REGISTER_TYPE_NAME rbx = 0; \
  REGISTER_TYPE_NAME rcx = 0; \
  REGISTER_TYPE_NAME rdx = 0; \
  REGISTER_TYPE_NAME rsi = 0; \
  REGISTER_TYPE_NAME rdi = 0; \
  REGISTER_TYPE_NAME r8 = 0;  \
  REGISTER_TYPE_NAME r9 = 0;  \
  REGISTER_TYPE_NAME r10 = 0; \
  REGISTER_TYPE_NAME r11 = 0; \
  REGISTER_TYPE_NAME r12 = 0; \
  REGISTER_TYPE_NAME r13 = 0; \
  REGISTER_TYPE_NAME r14 = 0; \
  REGISTER_TYPE_NAME r15 = 0; \
  REGISTER_TYPE_NAME rip = 0

namespace let_me_see {
struct SimpleContext {
  REGISTER_MESSAGES;
};

using SimpleContextPtr = SimpleContext*;

extern "C" {
extern void GetContext(SimpleContextPtr ctx);
extern void SetContext(SimpleContextPtr ctx);
extern void SwapContext(SimpleContextPtr src, SimpleContextPtr dest);
}

}  // namespace let_me_see

#endif