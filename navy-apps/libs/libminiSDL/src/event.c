#include <NDL.h>
#include <SDL.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  printf("Not implemented");
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  printf("Not implemented");
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  printf("Not implemented");
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  printf("Not implemented");
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  printf("Not implemented");
  return NULL;
}
