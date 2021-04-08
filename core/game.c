#include "game.h"

#include <math.h>
#include <stdlib.h>

#include "sprite.h"

const float FOV = M_PI_4;

game_t* game_new(
  int map_w,
  int map_h,
  int screen_w,
  int screen_r,
  float start_x,
  float start_y,
  float start_angle,

  int sprites_count,
  sprite_t* sprites

) {
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

  int map_size = map_w * map_h;

  map_sprite_t* map_sprites = malloc(sizeof(sprite_t) * map_size);

  for (int i = 0; i < sprites_count; i += 1) {
    sprite_t sprite = sprites[i];

    int map_x = (int)sprite.x;
    int map_y = (int)sprite.y;
    map_sprite_t map_sprite = {&sprites[i], 0, 0};  // or &sprite

    map_sprites[map_y * map_w + map_x] = map_sprite;
  }

  game->sprites_count = sprites_count;
  game->sprites = sprites;

  game->map_sprites = map_sprites;

  return game;
}

controls_t* game_controls(game_t* game) {
  return &game->controls;
}

float* game_wall_distances(game_t* game) {
  return game->distances.values;
}

map_sprite_t* game_map_sprites(game_t* game) {
  return game->map_sprites;
}

void game_set_distances(game_t* game) {
  distances_t* distances = &game->distances;
  map_sprite_t* map_sprites = game->map_sprites;
  player_t player = game->player;
  map_t map = game->map;

  float current_angle = player.angle - FOV / 2;
  float angle_delta = FOV / (float)distances->size;

  int i = 0;

  for (i = 0; i < map.w * map.h; i += 1) {
    map_sprites[i].is_rendered = 0;
  }

  for (i = 0; i < distances->size; i += 1) {
    distances->values[i] = cast_ray(&map, map_sprites, player.x, player.y, current_angle);
    current_angle += angle_delta;
  }
}

void game_tick(game_t* game) {
  player_move(&game->player, &game->controls);
  game_set_distances(game);
}
