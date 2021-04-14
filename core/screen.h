#ifndef _SCREEN_H_
#define _SCREEN_H_

typedef unsigned int pbuffer_t;
typedef float zbuffer_t;

typedef struct {
  int w;
  int h;
  int r;
  pbuffer_t* pbuffer;
  zbuffer_t* zbuffer;
} screen_t;

#endif
