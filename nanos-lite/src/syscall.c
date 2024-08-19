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
int sys_write(intptr_t buf, size_t count){
  // printf("sys_write\n");
  // printf("count: %d\n",count);
  // printf("%s\n",buf);
  for(int i = 0;i < count; i++){
    putch(*((char *)buf + i));
  }
  return count;
}
int sys_brk(intptr_t addr){
  //检查申请的addr是否越界
  return 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case SYS_yield:
      c->GPRx = sys_yield(NULL);break;
    case SYS_exit:
      c->GPRx = 0; sys_exit(c->GPRx);break;
    case SYS_write:
      c->GPRx = sys_write(a[1],a[2]);break;
    case SYS_brk:
      c->GPRx = sys_brk(a[1]);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

