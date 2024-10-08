#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
    for(int i = 0; i < len; i++)
      putch(*((char*)buf + i));
    return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if(ev.keycode !=0 ){
    return snprintf((char*)buf,len,"%s %s",ev.keydown ?"kd":"ku",keyname[ev.keycode]);
  }
  else{
    return 0;
  } 
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T cfg = io_read(AM_GPU_CONFIG);
  return snprintf((char*)buf,len,"Width:%d\n HEIGHT:%d",cfg.width,cfg.height);
}

size_t fb_write(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T cfg = io_read(AM_GPU_CONFIG);
  int x = offset % cfg.width;
  int y = (int)(offset/cfg.width);
  io_write(AM_GPU_FBDRAW, x, y, buf, len, 1, true);
  return 0;
}

size_t am_ioe_read(void *buf, size_t offset, size_t len) {
  // io_write(AM_GPU_FBDRAW, x, y, buf, len, 1, true);
  ioe_read ((int) offset, buf);
  return 0;
}


size_t am_ioe_write(void *buf, size_t offset, size_t len) {

  // io_write(AM_GPU_FBDRAW, x, y, buf, len, 1, true);
  ioe_write((int) offset, buf);
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
