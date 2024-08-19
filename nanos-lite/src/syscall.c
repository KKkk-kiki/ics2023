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
size_t sys_write(int fd, void *buf, size_t len){
  // for(int i = 0;i < count; i++){
  //   putch(*((char *)buf + i));
  // }
  return fs_write(fd, buf, len);
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
    case SYS_yield://1
      c->GPRx = sys_yield(NULL);break;
    case SYS_exit://0
      c->GPRx = 0; sys_exit(c->GPRx);break;
    case SYS_write://4
      c->GPRx = sys_write(a[1],(void*)a[2],a[3]);break;
    case SYS_brk://9
      c->GPRx = sys_brk(a[1]);break;
    case SYS_open://2
      c->GPRx = sys_open((char*)a[1], a[2], a[3]);break;
    case SYS_close://7
      c->GPRx = sys_close(a[1]);break;
    case SYS_read://3
      c->GPRx = sys_read(a[1], (void *)a[2], a[3]);break;
    case SYS_lseek://8
      c->GPRx = sys_lseek(a[1], a[2], a[3]);break;

    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

