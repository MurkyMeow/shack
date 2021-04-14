#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include "player.h"
#include "screen.h"
#include "sprite.h"

void cast_rays(
  zbuffer_t* zbuffer,
  int zbuffer_size,

  int map_w,
  const int* map_values,
  map_sprite_t* map_sprites,

  float x,
  float y,
  float dir_x,
  float dir_y

);

#endif
