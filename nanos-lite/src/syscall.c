#include <common.h>
#include "syscall.h"
extern void yield();
void halt(int code);
uintptr_t sys_yield(Context *c){
  yield();
  return 0;
}
void sys_exit(int ret){
  halt(ret);
  return ;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_yield:
      c->GPRx = sys_yield(NULL);break;
    case SYS_exit:
      c->GPRx = 0; sys_exit(c->GPRx);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

