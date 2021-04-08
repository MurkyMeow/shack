#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include "map.h"
#include "player.h"
#include "sprite.h"

float cast_ray(const map_t* map, map_sprite_t* map_sprites, float x, float y, float angle);

#endif
