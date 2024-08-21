#include <NDL.h>
#include <SDL.h>

int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained) {
  printf("Not implemented");
  return 0;
}

void SDL_CloseAudio() {
  printf("Audio implemented");
}

void SDL_PauseAudio(int pause_on) {
  printf("Audio implemented");
}

void SDL_MixAudio(uint8_t *dst, uint8_t *src, uint32_t len, int volume) {
  printf("Audio implemented");
}

SDL_AudioSpec *SDL_LoadWAV(const char *file, SDL_AudioSpec *spec, uint8_t **audio_buf, uint32_t *audio_len) {
  printf("Audio implemented");
  return NULL;
}

void SDL_FreeWAV(uint8_t *audio_buf) {
  printf("Audio implemented");
}

void SDL_LockAudio() {
  printf("Audio implemented");
}

void SDL_UnlockAudio() {
  printf("Audio implemented");
}
