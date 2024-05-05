#ifndef _SIMPLE_CONTEXT_H_
#define _SIMPLE_CONTEXT_H_

#define REGISTER_MESSAGES \
  void* rsp = 0;          \
  void* rbp = 0;          \
  void* rbx = 0;          \
  void* rcx = 0;          \
  void* rdx = 0;          \
  void* rsi = 0;          \
  void* rdi = 0;          \
  void* r8 = 0;           \
  void* r9 = 0;           \
  void* r10 = 0;          \
  void* r11 = 0;          \
  void* r12 = 0;          \
  void* r13 = 0;          \
  void* r14 = 0;          \
  void* r15 = 0;          \
  void* rip = 0

namespace let_me_see {
struct SimpleContext {
  REGISTER_MESSAGES;
};

using SimpleContextPtr = SimpleContext*;

extern "C" {
extern void GetContext(SimpleContextPtr ctx);
extern void SetContext(SimpleContextPtr ctx);
extern void SwapContext(SimpleContextPtr src, SimpleContextPtr desc);
}

}  // namespace let_me_see

#endif