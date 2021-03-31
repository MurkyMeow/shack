#ifndef _MAP_H_
#define _MAP_H_

typedef struct {
  int w;
  int h;
  int* values;
} map_t;

map_t map_new(int w, int h);

#endif
