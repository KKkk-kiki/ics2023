#include <NDL.h>
#include <SDL.h>
#include <string.h>
#define keyname(k) #k,
static int keystatus = 0;//0为up,1为down

#define SDL_NUM_SCANCODES 512
static uint8_t key_state[SDL_NUM_SCANCODES] = {0};


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
      for(int i = 0; i < sizeof(keyname)/sizeof(char *);i++){
            key_state[i] = 0; 
    }
  // ((char*)buf,len,"%s %s",ev.keydown ?"kd":"ku",keyname[ev.keycode]);
  // NDL_PollEvent(buf, sizeof(buf));
  // sscanf(buf,"%s %s",k_type,k_name);

  // if(NDL_PollEvent(buf, sizeof(buf))){
  //     sscanf(buf,"%s %s",k_type,k_name);
  //     if(strcmp(k_type,"kd") ==0 ){
  //       keystatus = 1;
  //     }
  //     if((strcmp(k_type,"ku") ==0) && (keystatus==1) ){
  //       ev->type = SDL_KEYDOWN;
  //       keystatus = 0;
  //       // printf("%s %s\n",k_type,k_name);
  //       for(int i = 0; i < sizeof(keyname)/sizeof(char *);i++){
  //         if(strcmp(keyname[i],k_name)==0){
  //           ev->key.keysym.sym = i;
  //           break;
  //         }
  //       }
  //     }
  //   return 1;
  // }
  // else{
  //   ev->type = SDL_NOEVENT;
  //   return 0;
  // }
    if(NDL_PollEvent(buf, sizeof(buf))){
      sscanf(buf,"%s %s",k_type,k_name);
      if(strcmp(k_type,"kd") ==0 ){
        ev->type = SDL_KEYDOWN;
      for (int i = 0; i < sizeof(keyname) / sizeof(char *); i++) {
        if (strcmp(keyname[i], k_name) == 0) {
          ev->key.keysym.sym = i;
          key_state[i] = 1; // 更新键盘状态
          printf("i:%d\n",i);
          break;
        }
      }
      return 1;
      }
    //   if((strcmp(k_type,"ku") ==0)){
    //     ev->type = SDL_KEYUP;
    //   for(int i = 0; i < sizeof(keyname)/sizeof(char *);i++){
    //     if(strcmp(keyname[i],k_name)==0){
    //         ev->key.keysym.sym = i;
    //         key_state[i] = 0; 
    //         break;
    //     }
    // }
    //     // printf("%s %s\n",k_type,k_name);
    //     return 1;
    //   }
  }
  else{
    ev->type = SDL_NOEVENT;
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

  if (numkeys != NULL) {
    *numkeys = SDL_NUM_SCANCODES;
  }

  // 这里可以添加逻辑来更新 key_state 数组
  // 例如，从 NDL_PollEvent 获取的按键状态更新到 key_state

  return key_state;
}
