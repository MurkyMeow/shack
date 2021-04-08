#include "raycast.h"

#include <math.h>

#include "sprite.h"

float cast_ray(const map_t* map, map_sprite_t* map_sprites, float x, float y, float angle) {
  float ray_dir_x = cosf(angle);
  float ray_dir_y = sinf(angle);

  float map_x = floorf(x);
  float map_y = floorf(y);

  float map_step_x = ray_dir_x < 0 ? -1 : 1;
  float map_step_y = ray_dir_y < 0 ? -1 : 1;

  float side_step_x = fabsf(1 / ray_dir_x);
  float side_step_y = fabsf(1 / ray_dir_y);

  float side_dist_x =
    map_step_x * (map_x + map_step_x * 0.5F + 0.5F - x) * side_step_x;
  float side_dist_y =
    map_step_y * (map_y + map_step_y * 0.5F + 0.5F - y) * side_step_y;

  int side = -1;

  int map_w = map->w;
  int* map_values = map->values;

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

    int map_index = (int)map_x + map_w * (int)map_y;
    int map_data = map_values[map_index];

    float hit_dist =
      side == 0
        ? (map_x - x + 0.5F - map_step_x * 0.5F) / ray_dir_x
        : (map_y - y + 0.5F - map_step_y * 0.5F) / ray_dir_y;

    if (map_sprites[map_index].original) {
      map_sprites[map_index].is_rendered = 1;
      map_sprites[map_index].distance = hit_dist;
    }

    if (map_data > 0) {
      return hit_dist;
    }
  }
}
