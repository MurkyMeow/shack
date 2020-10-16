#include <math.h>

double get_wall_distance(const unsigned int map[], unsigned int map_size_x, unsigned int map_size_y, double pov_x, double pov_y, int pov_angle) {
  double ray_dir_x = cos(pov_angle);
  double ray_dir_y = sin(pov_angle);

  int map_pos_x = floor(pov_x);
  int map_pos_y = floor(pov_y);

  int map_step_x = ray_dir_x < 0 ? -1 : 1;
  int map_step_y = ray_dir_y < 0 ? -1 : 1;

  double side_step_x = 1 / ray_dir_x;
  double side_step_y = 1 / ray_dir_y;

  int side_dist_x = map_step_x * (map_pos_x + map_step_x * 0.5 + 0.5 - pov_x) * side_step_x;
  int side_dist_y = map_step_y * (map_pos_y + map_step_y * 0.5 + 0.5 - pov_y) * side_step_y;

  int side = -1;

  while (1) {
    if (side_dist_x < side_dist_y) {
      map_pos_x += map_step_x;
      side_dist_x += side_step_x;
      side = 0;
    } else {
      map_pos_y += map_step_y;
      side_dist_y += side_step_y;
      side = 1;
    }

    int map_index = map_pos_x + map_size_x * map_pos_y;
    int map_data = map[map_index];

    if (map_data > 0) {
      double hit_dist = side == 0 ? map_pos_x - pov_x + (0.5 - map_step_x * 0.5) / ray_dir_x
                                  : map_pos_y - pov_y + (0.5 - map_step_y * 0.5) / ray_dir_y;
      return hit_dist;
    }
  }

  return 0;
}
