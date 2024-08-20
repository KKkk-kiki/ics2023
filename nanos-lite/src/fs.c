#include <fs.h>
#include "device.h"
typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  {"/dev/events", 0, 0, events_read, invalid_write},
  {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
  {"/dev/fb", 0, 0, invalid_read, fb_write},
#include "files.h"
};

#define FILENUM sizeof(file_table)/sizeof(Finfo)


void init_fs() {
  //initialize the size of /dev/fb
  AM_GPU_CONFIG_T cfg = io_read(AM_GPU_CONFIG);
  int fd = fs_open("/dev/fb", 0, 0);
  file_table[fd].size = sizeof(uint32_t)*cfg.width*cfg.height;
}

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
//打开pathname，返回文件描述符
int fs_open(const char *pathname, int flags, int mode){
  for(int i = 3; i < FILENUM ;i++){
    if(strcmp(file_table[i].name , pathname) == 0){
      file_table[i].open_offset = 0;
      return i;
    }  
  }
  panic("file %s is not found\n",pathname);
};

size_t fs_read(int fd, void *buf, size_t len){
  size_t read_len = len;
  size_t open_offset = file_table[fd].open_offset;
  size_t disk_offset = file_table[fd].disk_offset;
  size_t size = file_table[fd].size;
  if (file_table[fd].read != NULL){
    return file_table[fd].read(buf,0,len);
  }
  else{
    if (open_offset > size){
    return 0;
    }
    if(open_offset + len > size){
    read_len = size - open_offset;
    }
    file_table[fd].open_offset += read_len;
    return ramdisk_read(buf, disk_offset + open_offset, read_len);
  }
};


size_t fs_write(int fd, const void *buf, size_t len){
  size_t write_len = len;
  size_t open_offset = file_table[fd].open_offset;
  size_t disk_offset = file_table[fd].disk_offset;
  size_t size = file_table[fd].size;
  if (file_table[fd].write != NULL){
    return file_table[fd].write(buf,open_offset,len);
  }
  else{
    if (open_offset > size){
    return 0;
    }
    if(open_offset + len > size){
    write_len = size - open_offset;
    }
    file_table[fd].open_offset += write_len;
    return ramdisk_write(buf, disk_offset + open_offset, write_len);
  }
  
};

size_t fs_lseek(int fd, size_t offset, int whence){
  if(fd <= 2){
    Log("ignore lseek %s",file_table[fd].name);
    return 0;
  }
  size_t new_offset;
  if(whence == SEEK_SET){
    new_offset = offset;
  }
  else if(whence == SEEK_CUR){
    new_offset = file_table[fd].open_offset + offset;
  }
  else if(whence == SEEK_END){
    new_offset = file_table[fd].size + offset;
  }
  else{
    Log("Invalid whence value %d",whence);
    return -1;
  }

  if(new_offset < 0 || new_offset > file_table[fd].size){
    Log("Seek position out of bound");
    return -1;
  }

  file_table[fd].open_offset = new_offset;
  return new_offset;
};
int fs_close(int fd){
  return 0;
};