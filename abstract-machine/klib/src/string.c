#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  const char *str = s;
  while(*str != '\0'){
    str++;
  }
  return str - s;
  //panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  char *ret = dst;
  if ((dst == NULL) || (src == NULL)){
    return NULL;
  }
  while(*src != '\0'){
    *dst = *src;
    dst++;
    src++;
  }
  *dst = '\0';
  return ret;
  //panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  char *ret = dst;
  if ((dst == NULL) || (src == NULL)){
    return NULL;
  }
  while(n > 0){
    *dst = *src;
    dst++;
    src++;
    n--;
  }
  *dst = '\0';
  return ret;
  //panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
    char *ptr = dst;
    if ((dst == NULL) || (src == NULL)){
      return NULL;
    }
    // 找到 dest 字符串的末尾
    while (*ptr != '\0') {
        ptr++;
    }
    // 将 src 字符串的内容追加到 dest 的末尾
    while (*src != '\0') {
        *ptr = *src;
        ptr++;
        src++;
    }
    // 确保 dest 字符串以空字符结尾
    *ptr = '\0';
    return dst;
  //panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {

  unsigned char c1, c2;
  do{
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
	return c1 - c2;
  }while (c1 == c2);
  
  return c1 - c2;
  //panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
  if (s1 == NULL || s2 == NULL) {
    return 0;
  }
  while (n != 0 && *s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
    --n;
    ++s1;
    ++s2;
  }
  // 当比较了n次后，即新n变为0时，此时两个字符串也是相等得，memcmp同理
  return *s1 == *s2 || n == 0 ? 0 : (unsigned char)*s1 < (unsigned char)*s2 ? -1 : 1;
  //panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
  //panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
  unsigned char *d = (unsigned char *)dst;
  const unsigned char *s = (const unsigned char *)src;

  if (d < s) {
    // 如果目标地址在源地址之前，从前向后复制
    for (size_t i = 0; i < n; i++) {
      d[i] = s[i];
    }
  } else if (d > s) {
    // 如果目标地址在源地址之后，从后向前复制
    for (size_t i = n; i > 0; i--) {
      d[i - 1] = s[i - 1];
    }
  }
  // 如果目标地址和源地址相同，不需要复制

  return dst;
  //panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  unsigned char *d = (unsigned char *)out;
  const unsigned char *s = (const unsigned char *)in;

  for (size_t i = 0; i < n; i++) {
    d[i] = s[i];
  }

  return out;
  //panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  if (s1 == NULL || s2 == NULL) {
    return 0;
  }
  const unsigned char *src1 = s1;
  const unsigned char *src2 = s2;
  while (n != 0 && *src1 != '\0' && *src2 != '\0' && *src1 == *src2) {
    --n;
    ++src1;
    ++src2;
  }
  return *src1 == *src2 || n == 0 ? 0 : *src1 < *src2 ? -1 : 1;
}


#endif
