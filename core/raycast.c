#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// ==================
// IMPORTS
// ==================
void console_log(float) __attribute__((__import_name__("console_log")));

const float FOV = M_PI_4;
const float MOVE_SPEED = 0.1F;
const float SENSETIVITY = 0.075F;

typedef struct {
  int w;
  int h;
  int* values;
} map_t;

typedef struct {
  float x;
  float y;
  float angle;
} player_t;

typedef struct {
  unsigned int m_left;
  unsigned int m_right;
  unsigned int m_forward;
  unsigned int m_backward;
  unsigned int r_left;
  unsigned int r_right;
} controls_t;

typedef struct {
  int w;
  int h;
  int r;
  float* wall_distances;
} screen_t;

typedef struct {
  map_t* map;
  player_t* player;
  controls_t* controls;
  screen_t screen;
} game_t;

float get_wall_distance(map_t* map, player_t* player, float angle) {
  float ray_dir_x = cosf(angle);
  float ray_dir_y = sinf(angle);

  int map_x = (int)player->x;
  int map_y = (int)player->y;

  int map_step_x = ray_dir_x < 0 ? -1 : 1;
  int map_step_y = ray_dir_y < 0 ? -1 : 1;

  float side_step_x = fabsf(1 / ray_dir_x);
  float side_step_y = fabsf(1 / ray_dir_y);

  float side_dist_x = map_step_x * (map_x + map_step_x * 0.5 + 0.5 - player->x) * side_step_x;
  float side_dist_y = map_step_y * (map_y + map_step_y * 0.5 + 0.5 - player->y) * side_step_y;

  int side = -1;

  while (1) {
    if (side_dist_x < side_dist_y) {
      map_x += map_step_x;
      side_dist_x += side_step_x;
      side = 0;
    } else {
      map_y += map_step_y;
      side_dist_y += side_step_y;
      side = 1;
    }

    int map_index = map_x + map->w * map_y;
    int map_data = map->values[map_index];

    if (map_data > 0) {
      float hit_dist = side == 0 ? (map_x - player->x + 0.5 - map_step_x * 0.5) / ray_dir_x
                                 : (map_y - player->y + 0.5 - map_step_y * 0.5) / ray_dir_y;
      return hit_dist;
    }
  }
}

// ==================
// EXPORTS
// ==================
map_t* map_new(int w, int h) {
  int* values = malloc(sizeof(int) * w * h);

  map_t* map = malloc(sizeof(map_t));

  map->w = w;
  map->h = h;
  map->values = values;

  for (int i = 0; i < w * h; i += 1) {
    int x = i % h;
    int y = i / w;
    if (x == 0 || x == 9 || y == 0 || y == 9) {
      values[i] = 1;
    }
  }

  return map;
}

player_t* player_new(float x, float y, float angle) {
  player_t* player = malloc(sizeof(player_t));

  player->x = x;
  player->y = y;
  player->angle = angle;

  return player;
}

controls_t* controls_new() {
  controls_t* controls = malloc(sizeof(controls_t));
  return controls;
}

void player_move(player_t* player, controls_t* controls) {
  float dir_x = cosf(player->angle) * MOVE_SPEED;
  float dir_y = sinf(player->angle) * MOVE_SPEED;

  if (controls->m_forward) {
    player->x += dir_x;
    player->y += dir_y;
  }
  if (controls->m_backward) {
    player->x -= dir_x;
    player->y -= dir_y;
  }

  if (controls->m_left) {
    player->x += dir_y;
    player->y -= dir_x;
  }
  if (controls->m_right) {
    player->x -= dir_y;
    player->y += dir_x;
  }

  if (controls->r_left) {
    player->angle -= SENSETIVITY;
  }
  if (controls->r_right) {
    player->angle += SENSETIVITY;
  }
}

game_t* game_new(
    map_t* map,

    int screen_w,
    int screen_h,
    int screen_r,

    player_t* player) {
  game_t* game = malloc(sizeof(game_t));

  game->map = map;

  float* wall_distancecs = malloc(sizeof(float) * screen_w / screen_r);

  screen_t screen = {screen_w, screen_h, screen_r, wall_distancecs};
  game->screen = screen;

  game->player = player;

  return game;
}

float* game_wall_distances(game_t* game) {
  return game->screen.wall_distances;
}

void set_wall_distances(game_t* game) {
  int buffer_size = game->screen.w / game->screen.r;

  float current_angle = game->player->angle - FOV / 2;
  float angle_delta = FOV / buffer_size;

  for (int i = 0; i < buffer_size; i += 1) {
    game->screen.wall_distances[i] = get_wall_distance(game->map, game->player, current_angle);
    current_angle += angle_delta;
  }
}
