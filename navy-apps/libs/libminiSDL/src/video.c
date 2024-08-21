#include <NDL.h>
#include <sdl-video.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect) {
  assert(dst && src);
  assert(dst->format->BitsPerPixel == src->format->BitsPerPixel);
  // 获取源矩形和目标矩形
    SDL_Rect srcRect = {0, 0, src->w, src->h};
    if (srcrect) {
        srcRect = *srcrect;
    }

    SDL_Rect dstRect = {0, 0, srcRect.w, srcRect.h};
    if (dstrect) {
        dstRect = *dstrect;
    }

    // 确保目标矩形的宽度和高度与源矩形一致
    if (dstRect.w == 0) {
        dstRect.w = srcRect.w;
    }
    if (dstRect.h == 0) {
        dstRect.h = srcRect.h;
    }

    // 计算源矩形和目标矩形的边界
    int srcX = srcRect.x;
    int srcY = srcRect.y;
    int dstX = dstRect.x;
    int dstY = dstRect.y;
    int width = srcRect.w;
    int height = srcRect.h;

    // 获取源表面和目标表面的像素数据
    uint8_t *srcPixels = (uint8_t *)src->pixels;
    uint8_t *dstPixels = (uint8_t *)dst->pixels;

    // 计算每像素字节数
    int bytesPerPixel = src->format->BytesPerPixel;

    // 逐行复制像素数据
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // 计算源像素和目标像素的地址
            uint8_t *srcPixel = srcPixels + (srcY + y) * src->pitch + (srcX + x) * bytesPerPixel;
            uint8_t *dstPixel = dstPixels + (dstY + y) * dst->pitch + (dstX + x) * bytesPerPixel;

            // 复制像素数据
            memcpy(dstPixel, srcPixel, bytesPerPixel);
        }
    }
    int w = 0;
    int h = 0;
  //设置画布全屏幕
    NDL_OpenCanvas(&w, &h);
    NDL_DrawRect((uint32_t *)dst->pixels, dstrect->x, dstrect->y, dstrect->w, dstrect->h) ;


}
#include<stdio.h>
void SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, uint32_t color) {


  if(dst == NULL){
    return;
  }
  if(dstrect == NULL){
    dstrect = &(SDL_Rect){0,0,dst->w,dst->h};
  }

    //   // 获取表面的像素格式
    SDL_PixelFormat *format = dst->format;

    // // 将 RGBA 颜色转换为表面的像素格式
    uint32_t pixel_color = SDL_MapRGBA(format, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, (color >> 24) & 0xFF);
    for (int y = dstrect->y; y < dstrect->y + dstrect->h; y++) {
    for (int x = dstrect->x; x < dstrect->x + dstrect->w; x++) {
        int offset = y * dst->pitch + x * format->BytesPerPixel;
        printf("BP %d \n",format->BytesPerPixel);
        switch (format->BytesPerPixel) {
            case 1: // 8-bit
                *(uint8_t *)(dst->pixels + offset) = (uint8_t)0xffffff;
                break;
            case 2: // 16-bit
                *(uint16_t *)(dst->pixels + offset) = (uint16_t)0xffffff;
                break;
            // // case 3: // 24-bit
            //     if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            //         *(dst->pixels + offset) = (pixel_color >> 16) & 0xFF;
            //         *(dst->pixels + offset + 1) = (pixel_color >> 8) & 0xFF;
            //         *(dst->pixels + offset + 2) = pixel_color & 0xFF;
            //     } else {
            //         *(dst->pixels + offset) = pixel_color & 0xFF;
            //         *(dst->pixels + offset + 1) = (pixel_color >> 8) & 0xFF;
            //         *(dst->pixels + offset + 2) = (pixel_color >> 16) & 0xFF;
            //     }
            //     break;
            case 4: // 32-bit
                *(uint32_t *)(dst->pixels + offset) = 0xffffff;
                break;
        }
    }
}
    int w = 0;
    int h = 0;
  //设置画布全屏幕
    NDL_OpenCanvas(&w, &h);
    NDL_DrawRect((uint32_t *)dst->pixels, dstrect->x, dstrect->y, dstrect->w, dstrect->h) ;


}

void SDL_UpdateRect(SDL_Surface *s, int x, int y, int w, int h) {
  //默认打开画布居中
  NDL_OpenCanvas(&w, &h);
  NDL_DrawRect((uint32_t *)s->pixels, x, y, w, h) ;
}

// APIs below are already implemented.

static inline int maskToShift(uint32_t mask) {
  switch (mask) {
    case 0x000000ff: return 0;
    case 0x0000ff00: return 8;
    case 0x00ff0000: return 16;
    case 0xff000000: return 24;
    case 0x00000000: return 24; // hack
    default: assert(0);
  }
}

SDL_Surface* SDL_CreateRGBSurface(uint32_t flags, int width, int height, int depth,
    uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
  assert(depth == 8 || depth == 32);
  SDL_Surface *s = malloc(sizeof(SDL_Surface));
  assert(s);
  s->flags = flags;
  s->format = malloc(sizeof(SDL_PixelFormat));
  assert(s->format);
  if (depth == 8) {
    s->format->palette = malloc(sizeof(SDL_Palette));
    assert(s->format->palette);
    s->format->palette->colors = malloc(sizeof(SDL_Color) * 256);
    assert(s->format->palette->colors);
    memset(s->format->palette->colors, 0, sizeof(SDL_Color) * 256);
    s->format->palette->ncolors = 256;
  } else {
    s->format->palette = NULL;
    s->format->Rmask = Rmask; s->format->Rshift = maskToShift(Rmask); s->format->Rloss = 0;
    s->format->Gmask = Gmask; s->format->Gshift = maskToShift(Gmask); s->format->Gloss = 0;
    s->format->Bmask = Bmask; s->format->Bshift = maskToShift(Bmask); s->format->Bloss = 0;
    s->format->Amask = Amask; s->format->Ashift = maskToShift(Amask); s->format->Aloss = 0;
  }

  s->format->BitsPerPixel = depth;
  s->format->BytesPerPixel = depth / 8;

  s->w = width;
  s->h = height;
  s->pitch = width * depth / 8;
  assert(s->pitch == width * s->format->BytesPerPixel);

  if (!(flags & SDL_PREALLOC)) {
    s->pixels = malloc(s->pitch * height);
    assert(s->pixels);
  }

  return s;
}

SDL_Surface* SDL_CreateRGBSurfaceFrom(void *pixels, int width, int height, int depth,
    int pitch, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
  SDL_Surface *s = SDL_CreateRGBSurface(SDL_PREALLOC, width, height, depth,
      Rmask, Gmask, Bmask, Amask);
  assert(pitch == s->pitch);
  s->pixels = pixels;
  return s;
}

void SDL_FreeSurface(SDL_Surface *s) {
  if (s != NULL) {
    if (s->format != NULL) {
      if (s->format->palette != NULL) {
        if (s->format->palette->colors != NULL) free(s->format->palette->colors);
        free(s->format->palette);
      }
      free(s->format);
    }
    if (s->pixels != NULL && !(s->flags & SDL_PREALLOC)) free(s->pixels);
    free(s);
  }
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, uint32_t flags) {
  if (flags & SDL_HWSURFACE) NDL_OpenCanvas(&width, &height);
  return SDL_CreateRGBSurface(flags, width, height, bpp,
      DEFAULT_RMASK, DEFAULT_GMASK, DEFAULT_BMASK, DEFAULT_AMASK);
}

void SDL_SoftStretch(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect) {
  assert(src && dst);
  assert(dst->format->BitsPerPixel == src->format->BitsPerPixel);
  assert(dst->format->BitsPerPixel == 8);

  int x = (srcrect == NULL ? 0 : srcrect->x);
  int y = (srcrect == NULL ? 0 : srcrect->y);
  int w = (srcrect == NULL ? src->w : srcrect->w);
  int h = (srcrect == NULL ? src->h : srcrect->h);

  assert(dstrect);
  if(w == dstrect->w && h == dstrect->h) {
    /* The source rectangle and the destination rectangle
     * are of the same size. If that is the case, there
     * is no need to stretch, just copy. */
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_BlitSurface(src, &rect, dst, dstrect);
  }
  else {
    assert(0);
  }
}

void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors, int firstcolor, int ncolors) {
  assert(s);
  assert(s->format);
  assert(s->format->palette);
  assert(firstcolor == 0);

  s->format->palette->ncolors = ncolors;
  memcpy(s->format->palette->colors, colors, sizeof(SDL_Color) * ncolors);

  if(s->flags & SDL_HWSURFACE) {
    assert(ncolors == 256);
    for (int i = 0; i < ncolors; i ++) {
      uint8_t r = colors[i].r;
      uint8_t g = colors[i].g;
      uint8_t b = colors[i].b;
    }
    SDL_UpdateRect(s, 0, 0, 0, 0);
  }
}

static void ConvertPixelsARGB_ABGR(void *dst, void *src, int len) {
  int i;
  uint8_t (*pdst)[4] = dst;
  uint8_t (*psrc)[4] = src;
  union {
    uint8_t val8[4];
    uint32_t val32;
  } tmp;
  int first = len & ~0xf;
  for (i = 0; i < first; i += 16) {
#define macro(i) \
    tmp.val32 = *((uint32_t *)psrc[i]); \
    *((uint32_t *)pdst[i]) = tmp.val32; \
    pdst[i][0] = tmp.val8[2]; \
    pdst[i][2] = tmp.val8[0];

    macro(i + 0); macro(i + 1); macro(i + 2); macro(i + 3);
    macro(i + 4); macro(i + 5); macro(i + 6); macro(i + 7);
    macro(i + 8); macro(i + 9); macro(i +10); macro(i +11);
    macro(i +12); macro(i +13); macro(i +14); macro(i +15);
  }

  for (; i < len; i ++) {
    macro(i);
  }
}

SDL_Surface *SDL_ConvertSurface(SDL_Surface *src, SDL_PixelFormat *fmt, uint32_t flags) {
  assert(src->format->BitsPerPixel == 32);
  assert(src->w * src->format->BytesPerPixel == src->pitch);
  assert(src->format->BitsPerPixel == fmt->BitsPerPixel);

  SDL_Surface* ret = SDL_CreateRGBSurface(flags, src->w, src->h, fmt->BitsPerPixel,
    fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);

  assert(fmt->Gmask == src->format->Gmask);
  assert(fmt->Amask == 0 || src->format->Amask == 0 || (fmt->Amask == src->format->Amask));
  ConvertPixelsARGB_ABGR(ret->pixels, src->pixels, src->w * src->h);

  return ret;
}

uint32_t SDL_MapRGBA(SDL_PixelFormat *fmt, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  assert(fmt->BytesPerPixel == 4);
  uint32_t p = (r << fmt->Rshift) | (g << fmt->Gshift) | (b << fmt->Bshift);
  if (fmt->Amask) p |= (a << fmt->Ashift);
  return p;
}

int SDL_LockSurface(SDL_Surface *s) {
  return 0;
}

void SDL_UnlockSurface(SDL_Surface *s) {
}
