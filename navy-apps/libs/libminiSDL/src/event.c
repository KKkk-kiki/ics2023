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
  char buf[64];
  char *k_type;
  char *k_name;
  // ((char*)buf,len,"%s %s",ev.keydown ?"kd":"ku",keyname[ev.keycode]);
  NDL_PollEvent(buf, sizeof(buf));
  sscanf(buf,"%s %s",k_type,k_name);
  printf("%s %s\n",k_type,k_name);
  // if(NDL_PollEvent(buf, sizeof(buf))){
  //     sscanf(buf,"%s %s",k_type,k_name);
  //     if(strcmp(k_type,"kd") ==0 ){
  //       event->type == SDL_KEYDOWN;
  //     }
  //     if(strcmp(k_type,"ku") ==0 ){
  //       event->type == SDL_KEYUP;
  //     }
  //     for(int i = 0; i < sizeof(keyname)/sizeof(char *);i++){
  //       if(strcmp(keyname[i],k_name)==0){
  //         event->key.keysym.sym = i;
  //         break;
  //       }
  //     }
  //   return 1;
  // }
  return 0;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  printf("Not implemented");
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  printf("Not implemented");
  return NULL;
}
