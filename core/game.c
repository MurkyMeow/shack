#include "game.h"

#include <math.h>
#include <stdlib.h>

#include "map.h"
#include "player.h"
#include "sprite.h"

const float FOV = M_PI_4;

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

) {
  game_t* game = malloc(sizeof(game_t));

  map_t map = map_new(map_w, map_h);
  game->map = map;

  player_t player = {start_x, start_y, cosf(start_angle), sinf(start_angle)};
  game->player = player;

  controls_t controls = {0, 0, 0, 0, 0, 0};
  game->controls = controls;

  int zbuffer_size = screen_w / screen_r;
  float* zbuffer = malloc(sizeof(float) * zbuffer_size);

  pbuffer_t* screen_buffer = malloc(sizeof(int) * screen_w * screen_h);
  screen_t screen = {screen_w, screen_h, screen_r, screen_buffer, zbuffer};
  game->screen = screen;

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

pbuffer_t* game_pbuffer(game_t* game) {
  return game->screen.pbuffer;
}

void game_set_zbuffer(game_t* game) {
  int zbuffer_size = game->screen.w / game->screen.r;
  float* zbuffer = game->screen.zbuffer;

  map_sprite_t* map_sprites = game->map_sprites;
  player_t player = game->player;
  map_t map = game->map;

  float plane_x = -player.dir_y;
  float plane_y = player.dir_x;

  int i = 0;

  for (i = 0; i < map.w * map.h; i += 1) {
    map_sprites[i].is_rendered = 0;
  }

  for (i = 0; i < zbuffer_size; i += 1) {
    float cam_x = 2 * (float)i / (float)zbuffer_size - 1;
    float ray_dir_x = player.dir_x + plane_x * cam_x;
    float ray_dir_y = player.dir_y + plane_y * cam_x;
    zbuffer[i] = cast_ray(&map, map_sprites, player.x, player.y, ray_dir_x, ray_dir_y);
  }
}

void game_set_pbuffer(game_t* game) {
  screen_t screen = game->screen;

  int screen_w = screen.w;
  int screen_h = screen.h;
  int screen_r = screen.r;
  pbuffer_t* pbuffer = screen.pbuffer;
  zbuffer_t* zbuffer = screen.zbuffer;

  // clear screen
  for (int i = 0; i < screen_w * screen_h; i += 1) {
    pbuffer[i] = 0;
  }

  // draw walls
  for (int i = 0; i < screen_w / screen_r; i += 1) {
    int height = (int)((float)screen_h / zbuffer[i]);

    if (height > screen_h) {
      height = screen_h;
    }

    int start_y = (screen_h - height) / 2;
    int start_x = i * screen_r;

    for (int y = start_y; y < start_y + height; y += 1) {
      for (int x = start_x; x < start_x + screen_r; x += 1) {
        pbuffer[y * screen_w + x] = 0xFF000000;
      }
    }
  }
}

void game_tick(game_t* game) {
  player_move(&game->player, &game->controls);
  game_set_zbuffer(game);
  game_set_pbuffer(game);
}
