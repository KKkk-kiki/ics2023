#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
static int evtdev = -1;
static int fbdev = -1;
//屏幕大小
static int screen_w = 0, screen_h = 0;
//画布大小
static int canvas_w;
static int canvas_h;
//画布原点位置,画布左上角为原点
static int canvas_x;
static int canvas_y;

// 以毫秒为单位返回系统时间
uint32_t NDL_GetTicks(struct timeval *tv, struct timezone *tz) {
  int result =  gettimeofday(tv, tz);
  uint32_t usec =  tv->tv_usec;
  uint32_t sec =  tv->tv_sec;
  uint32_t ms1 = usec / (uint32_t )1000;
  uint32_t ms2 = sec * 1000;
  uint32_t ms = sec * 1000 + usec / (uint32_t )1000;

  printf("here!!\n");
  printf("ticks: %u\n",ms);
  return ms;
}

int NDL_PollEvent(char *buf, int len) {
  int fd = open("/dev/events", 0,"r");
  int ret = read(fd, buf, len);
  close(fd);
  return ret;
}

//打开(w,h)大小的画布
void NDL_OpenCanvas(int *w, int *h) {

  int fd = open("/proc/dispinfo", 0,"r");
  char buf[24];
  read(fd, buf, sizeof(buf));
  close(fd);
  sscanf(buf, "Width:%d\n HEIGHT:%d",&screen_w,&screen_h);
  if((*w == 0) && (*h == 0)){
    *w = screen_w;
    *h = screen_h;
    canvas_w = screen_w;
    canvas_h = screen_h;
  }
  else{
    canvas_w = *w;
    canvas_h = *h;
  }
  canvas_x = (screen_w - canvas_w)/2;
  canvas_y = (screen_h - canvas_h)/2;
  // if (getenv("NWM_APP")) {
  //   int fbctl = 4;
  //   fbdev = 5;
  //   screen_w = *w; screen_h = *h;
  //   char buf[64];
  //   int len = sprintf(buf, "%d %d", screen_w, screen_h);
  //   // let NWM resize the window and create the frame buffer
  //   write(fbctl, buf, len);
  //   while (1) {
  //     // 3 = evtdev
  //     int nread = read(3, buf, sizeof(buf) - 1);
  //     if (nread <= 0) continue;
  //     buf[nread] = '\0';
  //     if (strcmp(buf, "mmap ok") == 0) break;
  //   }
  //   close(fbctl);
  // }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  int fd = open("/dev/fb", 0,"r");
  for (int i = 0; i < h && y + i < canvas_h ; i++){
    lseek(fd,((y + i +canvas_y) * screen_w + canvas_x + x),0);
    write(fd,pixels + i*w,( w < canvas_w-x ? w : canvas_w-x));
  }
  close(fd);
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
