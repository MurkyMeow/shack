#include "map.h"

#include <stdlib.h>

map_t map_new(int w, int h) {
  int* values = malloc(sizeof(int) * w * h);

  for (int i = 0; i < w * h; i += 1) {
    int x = i % h;
    int y = i / w;
    if (x == 0 || x == 9 || y == 0 || y == 9) {
      values[i] = 1;
    }
  }

  map_t map = {w, h, values};

  return map;
}
