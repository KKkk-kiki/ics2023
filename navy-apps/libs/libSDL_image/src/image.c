#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
// #include <stb_image.h>
SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);
  return NULL;
}

SDL_Surface* IMG_Load(const char *filename) {
      FILE *fp = fopen(file, "rb");
    if (!fp) {
        fprintf(stderr, "Failed to open file: %s\n", file);
        return NULL;
    }

    // 获取文件大小
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 申请内存
    void *buf = malloc(size);
    if (!buf) {
        fclose(fp);
        fprintf(stderr, "Failed to allocate memory for file: %s\n", file);
        return NULL;
    }

    // 读取文件内容到内存
    if (fread(buf, 1, size, fp) != size) {
        fclose(fp);
        free(buf);
        fprintf(stderr, "Failed to read file: %s\n", file);
        return NULL;
    }

    fclose(fp);

    // 使用 stb_image 加载图像
    int width, height, channels;
    unsigned char *pixels = stbi_load_from_memory(buf, size, &width, &height, &channels, STBI_rgb_alpha);
    free(buf);

    if (!pixels) {
        fprintf(stderr, "Failed to load image: %s\n", file);
        return NULL;
    }

    // 创建 SDL_Surface
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(pixels, width, height, 32, width * 4,
                                                    0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    if (!surface) {
        stbi_image_free(pixels);
        fprintf(stderr, "Failed to create SDL_Surface from image: %s\n", file);
        return NULL;
    }

    return surface;
}

int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}
