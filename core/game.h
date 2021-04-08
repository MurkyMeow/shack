#ifndef _GAME_H_
#define _GAME_H_

#include "map.h"
#include "player.h"
#include "raycast.h"
#include "sprite.h"

typedef unsigned int pbuffer_t;
typedef float zbuffer_t;

typedef struct {
  int w;
  int h;
  int r;
  pbuffer_t* pbuffer;
  zbuffer_t* zbuffer;
} screen_t;

typedef struct {
  map_t map;
  player_t player;
  controls_t controls;
  screen_t screen;

  int sprites_count;
  sprite_t* sprites;

  map_sprite_t* map_sprites;
} game_t;

game_t* game_new(
  int map_w,
  int map_h,

  int screen_w,
  int screen_h,
  int screen_r,

  float start_x,
  float start_y,
  float start_angle,

  int sprites_count,
  sprite_t* sprites

);

float* game_wall_distances(game_t* game);

controls_t* game_controls(game_t* game);

void game_tick(game_t* game);

#endif
