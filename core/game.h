#ifndef _GAME_H_
#define _GAME_H_

#include "map.h"
#include "player.h"
#include "raycast.h"

typedef struct {
  int size;
  float* values;
} distances_t;

typedef struct {
  map_t map;
  player_t player;
  controls_t controls;
  distances_t distances;
} game_t;

game_t* game_new(
  int map_w,
  int map_h,

  int screen_w,
  int screen_r,

  float start_x,
  float start_y,
  float start_angle);

float* game_wall_distances(game_t* game);

controls_t* game_controls(game_t* game);

void game_tick(game_t* game);

#endif
