#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// ==================
// IMPORTS
// ==================
void console_log(int) __attribute__((__import_name__("console_log")));
void console_logf(float) __attribute__((__import_name__("console_log")));

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
  int size;
  float* values;
} distances_t;

typedef struct {
  map_t map;
  player_t player;
  controls_t controls;
  distances_t distances;
} game_t;

float get_wall_distance(const map_t* map, const player_t* player, float angle) {
  float ray_dir_x = cosf(angle);
  float ray_dir_y = sinf(angle);

  float map_x = floorf(player->x);
  float map_y = floorf(player->y);

  float map_step_x = ray_dir_x < 0 ? -1 : 1;
  float map_step_y = ray_dir_y < 0 ? -1 : 1;

  float side_step_x = fabsf(1 / ray_dir_x);
  float side_step_y = fabsf(1 / ray_dir_y);

  float side_dist_x = map_step_x * (map_x + map_step_x * 0.5F + 0.5F - player->x) * side_step_x;
  float side_dist_y = map_step_y * (map_y + map_step_y * 0.5F + 0.5F - player->y) * side_step_y;

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

    int map_index = (int)map_x + map->w * (int)map_y;
    int map_data = map->values[map_index];

    if (map_data > 0) {
      float hit_dist = side == 0 ? (map_x - player->x + 0.5F - map_step_x * 0.5F) / ray_dir_x
                                 : (map_y - player->y + 0.5F - map_step_y * 0.5F) / ray_dir_y;
      return hit_dist;
    }
  }
}

void set_wall_distances(game_t* game) {
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

// ==================
// EXPORTS
// ==================
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

player_t* player_new(float x, float y, float angle) {
  player_t* player = malloc(sizeof(player_t));

  player->x = x;
  player->y = y;
  player->angle = angle;

  return player;
}

void player_move(player_t* player, const controls_t* controls) {
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

void game_tick(game_t* game) {
  player_move(&game->player, &game->controls);
  set_wall_distances(game);
}
