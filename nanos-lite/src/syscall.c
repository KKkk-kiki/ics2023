#include <common.h>
#include "syscall.h"
#include <fs.h>
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
  for(int i = 0;i < count; i++){
    putch(*((char *)buf + i));
  }
  return count;
}
int sys_brk(intptr_t addr){
  //检查申请的addr是否越界
  return 0;
}

int sys_open(char *pathname, int flags, int mode){
  return fs_open(pathname,  flags,  mode);
}
int sys_close(int fd){
  return fs_close(fd);
};
size_t sys_read(int fd, void *buf, size_t len){
  return fs_read( fd, buf, len);
}
size_t sys_lseek(int fd, size_t offset, int whence){
  return fs_lseek(fd, offset,  whence);
}
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  // printf("syscall ID: %d\n",a[0]);
  switch (a[0]) {
    case SYS_yield:
      c->GPRx = sys_yield(NULL);break;
    case SYS_exit:
      c->GPRx = 0; sys_exit(c->GPRx);break;
    case SYS_write:
      c->GPRx = sys_write(a[1],a[2]);break;
    case SYS_brk:
      c->GPRx = sys_brk(a[1]);break;
    case SYS_open:
      c->GPRx = sys_open((char*)a[1], a[2], a[3]);break;
    case SYS_close:
      c->GPRx = sys_close(a[1]);break;
    case SYS_read:
      c->GPRx = sys_read(a[1], (void *)a[2], a[3]);break;
    case SYS_lseek:
      c->GPRx = sys_lseek(a[1], a[2], a[3]);break;

    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

