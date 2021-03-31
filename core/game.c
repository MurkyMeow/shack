#include "game.h"

#include <math.h>
#include <stdlib.h>

// ==================
// IMPORTS
// ==================
void console_log(int) __attribute__((__import_name__("console_log")));
void console_logf(float) __attribute__((__import_name__("console_log")));

const float FOV = M_PI_4;

game_t* game_new(
  int map_w,
  int map_h,
  int screen_w,
  int screen_r,
  float start_x,
  float start_y,
  float start_angle) {
  game_t* game = malloc(sizeof(game_t));

  map_t map = map_new(map_w, map_h);
  game->map = map;

  player_t player = {start_x, start_y, start_angle};
  game->player = player;

  controls_t controls = {0, 0, 0, 0, 0, 0};
  game->controls = controls;

  int distances_size = screen_w / screen_r;
  float* distances_values = malloc(sizeof(float) * distances_size);

  distances_t distances = {distances_size, distances_values};
  game->distances = distances;

  return game;
}

controls_t* game_controls(game_t* game) {
  return &game->controls;
}

float* game_wall_distances(game_t* game) {
  return game->distances.values;
}

void game_set_distances(game_t* game) {
  distances_t* distances = &game->distances;
  player_t player = game->player;
  map_t map = game->map;

  float current_angle = player.angle - FOV / 2;
  float angle_delta = FOV / (float)distances->size;

  for (int i = 0; i < distances->size; i += 1) {
    distances->values[i] = get_wall_distance(&map, &player, current_angle);
    current_angle += angle_delta;
  }
}

void game_tick(game_t* game) {
  player_move(&game->player, &game->controls);
  game_set_distances(game);
}
