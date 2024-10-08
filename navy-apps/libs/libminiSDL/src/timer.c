#include <NDL.h>
#include <sdl-timer.h>
#include <stdio.h>
#include <sys/time.h>
SDL_TimerID SDL_AddTimer(uint32_t interval, SDL_NewTimerCallback callback, void *param) {
  return NULL;
}

int SDL_RemoveTimer(SDL_TimerID id) {
  return 1;
}

uint32_t SDL_GetTicks() {
  struct timeval tv;
  static uint32_t start_ticks = 0;
    if (start_ticks == 0) {
        start_ticks = NDL_GetTicks(&tv,NULL); // 获取系统启动以来的 ticks 数
    }

  uint32_t current_ticks = NDL_GetTicks(&tv,NULL); // 获取当前 ticks 数
  return (current_ticks - start_ticks); //毫秒
}

void SDL_Delay(uint32_t ms) {
  uint32_t start = SDL_GetTicks();
  while (SDL_GetTicks() - start < ms) {
    // 忙等待
  }
  return ;
}
