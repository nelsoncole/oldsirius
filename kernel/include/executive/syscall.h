#ifndef __syscall_h__
#define __syscall_h__
#include <typedef.h>
extern void int69();
#define SYSCALL_VETOR_NUM 0x70
int syscall_handler(uint32_t num);

#define DECL_SYSCALL0(fn) int syscall_##fn();
#define DECL_SYSCALL1(fn,p1) int syscall_##fn(p1);
#define DECL_SYSCALL2(fn,p1,p2) int syscall_##fn(p1,p2);
#define DECL_SYSCALL3(fn,p1,p2,p3) int syscall_##fn(p1,p2,p3);
#define DECL_SYSCALL4(fn,p1,p2,p3,p4) int syscall_##fn(p1,p2,p3,p4);
#define DECL_SYSCALL5(fn,p1,p2,p3,p4,p5) int syscall_##fn(p1,p2,p3,p4,p5);

#define DEFN_SYSCALL0(fn, num) \
int syscall_##fn() \
{ \
  int eax; \
  __asm__ __volatile__("int $0x69" : "=a" (eax) : "0" (num)); \
  return eax; \
}

#define DEFN_SYSCALL1(fn, num, P1) \
int syscall_##fn(P1 p1) \
{ \
  int eax; \
  __asm__ __volatile__("int $0x69" : "=a" (eax) : "0" (num), "b" ((int)p1)); \
  return eax; \
}

#define DEFN_SYSCALL2(fn, num, P1, P2) \
int syscall_##fn(P1 p1, P2 p2) \
{ \
  int eax; \
  __asm__ __volatile__("int $0x69" : "=a" (eax) : "0" (num), "b" ((int)p1), "c" ((int)p2)); \
  return eax; \
}

#define DEFN_SYSCALL3(fn, num, P1, P2, P3) \
int syscall_##fn(P1 p1, P2 p2, P3 p3) \
{ \
  int eax; \
  __asm__ __volatile__("int $0x69" : "=a" (eax) : "0" (num), "b" ((int)p1),\
  "c" ((int)p2), "d"((int)p3)); \
  return eax; \
}

#define DEFN_SYSCALL4(fn, num, P1, P2, P3, P4) \
int syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4) \
{ \
  int eax; \
  __asm__ __volatile__("int $0x69" : "=a" (eax) : "0" (num), "b" ((int)p1),\
  "c" ((int)p2), "d" ((int)p3), "S" ((int)p4)); \
  return eax; \
}

#define DEFN_SYSCALL5(fn, num) \
int syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) \
{ \
  int eax; \
  __asm__ __volatile__("int $0x69" : "=a" (eax) : "0" (num), "b" ((int)p1),\
  "c" ((int)p2), "d" ((int)p3), "S" ((int)p4), "D" ((int)p5)); \
  return eax; \
}

DECL_SYSCALL0(cls);
DECL_SYSCALL1(putchar,int);
DECL_SYSCALL1(puts, const char*);
#endif
