#include <NDL.h>
#include <SDL.h>

int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained) {
  panic("Not implemented");
  return 0;
}

void SDL_CloseAudio() {
  panic("Not implemented");
}

void SDL_PauseAudio(int pause_on) {
  panic("Not implemented");
}

void SDL_MixAudio(uint8_t *dst, uint8_t *src, uint32_t len, int volume) {
  panic("Not implemented");
}

SDL_AudioSpec *SDL_LoadWAV(const char *file, SDL_AudioSpec *spec, uint8_t **audio_buf, uint32_t *audio_len) {
  panic("Not implemented");
  return NULL;
}

void SDL_FreeWAV(uint8_t *audio_buf) {
  panic("Not implemented");
}

void SDL_LockAudio() {
  panic("Not implemented");
}

void SDL_UnlockAudio() {
  panic("Not implemented");
}
