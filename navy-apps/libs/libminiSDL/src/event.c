#include <NDL.h>
#include <SDL.h>
#include <string.h>
#define keyname(k) #k,
static int keystatus = 0;//0为up,1为down
static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  printf("Not implemented");
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  char buf[64];
  char k_type[10];
  char k_name[10];
  // ((char*)buf,len,"%s %s",ev.keydown ?"kd":"ku",keyname[ev.keycode]);
  // NDL_PollEvent(buf, sizeof(buf));
  // sscanf(buf,"%s %s",k_type,k_name);
  if(NDL_PollEvent(buf, sizeof(buf))){
      sscanf(buf,"%s %s",k_type,k_name);
      if(strcmp(k_type,"kd") ==0 ){
        keystatus = 1;
      }
      if((strcmp(k_type,"ku") ==0) && (keystatus==1) ){
        ev->type = SDL_KEYDOWN;
        keystatus = 0;
        // printf("%s %s\n",k_type,k_name);
        for(int i = 0; i < sizeof(keyname)/sizeof(char *);i++){
          if(strcmp(keyname[i],k_name)==0){
            ev->key.keysym.sym = i;
            break;
          }
        }
      }
    return 1;
  }
  else{
    ev->type = SDL_KEYUP;
    return 0;
  }
}

int SDL_WaitEvent(SDL_Event *event) {
  char buf[64];
  char k_type[10];
  char k_name[10];
  // ((char*)buf,len,"%s %s",ev.keydown ?"kd":"ku",keyname[ev.keycode]);
  // NDL_PollEvent(buf, sizeof(buf));
  // sscanf(buf,"%s %s",k_type,k_name);
  if(NDL_PollEvent(buf, sizeof(buf))){
      sscanf(buf,"%s %s",k_type,k_name);
      if(strcmp(k_type,"kd") ==0 ){
        keystatus = 1;
      }
      if((strcmp(k_type,"ku") ==0) && (keystatus==1) ){
        event->type = SDL_KEYDOWN;
        keystatus = 0;
        // printf("%s %s\n",k_type,k_name);
        for(int i = 0; i < sizeof(keyname)/sizeof(char *);i++){
          if(strcmp(keyname[i],k_name)==0){
            event->key.keysym.sym = i;
            break;
        }
      }
      }
    return 1;
  }
  else{
    event->type = SDL_KEYUP;
  }
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
