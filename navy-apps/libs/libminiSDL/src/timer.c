#include <NDL.h>
#include <sdl-timer.h>
#include <stdio.h>

SDL_TimerID SDL_AddTimer(uint32_t interval, SDL_NewTimerCallback callback, void *param) {
  return NULL;
}

int SDL_RemoveTimer(SDL_TimerID id) {
  return 1;
}

uint32_t SDL_GetTicks() {
        printf("!!\n");
  struct timeval *tv = NULL;
  struct timezone *tz = NULL;
  static uint32_t start_ticks = 0;
    if (start_ticks == 0) {
             printf("here!!\n");
        start_ticks = NDL_GetTicks(tv,tz); // 获取系统启动以来的 ticks 数
    }

  uint32_t current_ticks = NDL_GetTicks(tv,tz); // 获取当前 ticks 数
  return (current_ticks - start_ticks); // 转换为毫秒
}

void SDL_Delay(uint32_t ms) {
  printf("Not implemented");
}
