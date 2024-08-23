#include <am.h>

bool ioe_init() {
  return true;
}

void ioe_read (int reg, void *buf)
{ 
  int fd = open("/dev/ioe_am", 0,0);
  lseek(fd,reg,0);
  read(fd, buf, sizeof(buf));
  close(fd);
}

void ioe_write(int reg, void *buf)
{ 
  int fd = open("/dev/ioe_am", 0,0);
  lseek(fd,reg,0);
  write(fd, buf, sizeof(buf));
  close(fd);
}
