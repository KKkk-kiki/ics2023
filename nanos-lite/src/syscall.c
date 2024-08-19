#include <common.h>
#include "syscall.h"
extern void yield();
uintptr_t sys_yield(Context *c){
  yield();
  return 0;
}

void do_syscall(Context *c) {
  printf("do_syscall!\n");
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_yield:
      c->GPRx = sys_yield(NULL);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

