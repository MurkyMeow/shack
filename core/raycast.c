#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// ==================
// IMPORTS
// ==================
void console_log(float) __attribute__((__import_name__("console_log")));

typedef struct {
  int* values;
  int size_x;
  int size_y;
} map_t;

typedef struct {
  float x;
  float y;
  float fov;
  float angle;
} pov_t;

typedef struct {
  int width;
  int height;
  int resolution;
} screen_t;

float get_wall_distance(int* map_values, int map_w, pov_t pov, float angle) {
  float ray_dir_x = cosf(angle);
  float ray_dir_y = sinf(angle);

  int map_x = (int)pov.x;
  int map_y = (int)pov.y;

  int map_step_x = ray_dir_x < 0 ? -1 : 1;
  int map_step_y = ray_dir_y < 0 ? -1 : 1;

  float side_step_x = fabsf(1 / ray_dir_x);
  float side_step_y = fabsf(1 / ray_dir_y);

  float side_dist_x = map_step_x * (map_x + map_step_x * 0.5 + 0.5 - pov.x) * side_step_x;
  float side_dist_y = map_step_y * (map_y + map_step_y * 0.5 + 0.5 - pov.y) * side_step_y;

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

    int map_index = map_x + map_w * map_y;
    int map_data = map_values[map_index];

    if (map_data > 0) {
      float hit_dist = side == 0 ? (map_x - pov.x + 0.5 - map_step_x * 0.5) / ray_dir_x
                                 : (map_y - pov.y + 0.5 - map_step_y * 0.5) / ray_dir_y;
      return hit_dist;
    }
  }
}

// ==================
// EXPORTS
// ==================
int* create_map(int size_x, int size_y) {
  int* result = malloc(sizeof(int) * size_x * size_y);

  for (int i = 0; i < size_x * size_y; i += 1) {
    int x = i % size_y;
    int y = i / size_x;
    if (x == 0 || x == 9 || y == 0 || y == 9) {
      result[i] = 1;
    }
  }

  return result;
}

float* create_distances_buffer(int screen_w, int screen_r) {
  return malloc(sizeof(float) * screen_w / screen_r);
}

void set_wall_distances(
    int screen_w,
    int screen_r,
    int map_w,
    int map_h,
    int* map_values,
    float pov_x,
    float pov_y,
    float angle,
    float* distances_buffer) {
  pov_t pov = {.x = pov_x, .y = pov_y, .fov = M_PI / 4};

  int buffer_size = screen_w / screen_r;

  float current_angle = angle - pov.fov / 2;
  float angle_delta = pov.fov / buffer_size;

  for (int i = 0; i < buffer_size; i += 1) {
    distances_buffer[i] = get_wall_distance(map_values, map_w, pov, current_angle);
    current_angle += angle_delta;
  }
}
