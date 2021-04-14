#include "game.h"

#include <math.h>
#include <stdlib.h>

#include "raycast.h"

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
  zbuffer_t* zbuffer = game->screen.zbuffer;

  map_sprite_t* map_sprites = game->map_sprites;
  player_t player = game->player;
  map_t map = game->map;

  for (int i = 0; i < map.w * map.h; i += 1) {
    map_sprites[i].is_rendered = 0;
  }

  cast_rays(zbuffer, zbuffer_size, map.w, map.values, map_sprites, player.x, player.y, player.dir_x, player.dir_y);
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
      int offset_y = y * screen_w;
      for (int x = start_x; x < start_x + screen_r; x += 1) {
        pbuffer[offset_y + x] = 0xFF000000;
      }
    }
  }

  // draw sprites
  map_t map = game->map;
  map_sprite_t* map_sprites = game->map_sprites;

  for (int i = 0; i < map.w * map.h; i += 1) {
    map_sprite_t map_sprite = map_sprites[i];

    if (map_sprite.is_rendered) {
      int sprite_size = abs((int)((float)screen_h / map_sprite.distance));

      int draw_start_y = screen_h / 2 - sprite_size / 2;
      int draw_end_y = draw_start_y + sprite_size;
      int draw_start_x = (screen_r * map_sprite.screen_stripe) - sprite_size / 2;
      int draw_end_x = draw_start_x + sprite_size;

      if (draw_start_y < 0) {
        draw_start_y = 0;
      }
      if (draw_end_y > screen_h) {
        draw_end_y = screen_h;
      }
      if (draw_start_x < 0) {
        draw_start_x = 0;
      }
      if (draw_end_x > screen_w) {
        draw_end_x = screen_w;
      }

      for (int y = draw_start_y; y < draw_end_y; y += 1) {
        int offset_y = y * screen_w;
        for (int x = draw_start_x; x < draw_end_x; x += 1) {
          pbuffer[offset_y + x] = 0xFF0000FF;
        }
      }
    }
  }
}

void game_tick(game_t* game) {
  player_move(&game->player, &game->controls);
  game_set_zbuffer(game);
  game_set_pbuffer(game);
}
