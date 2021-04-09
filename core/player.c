#include "player.h"

#include <math.h>
#include <stdlib.h>

const float MOVE_SPEED = 0.1F;
const float ROT_SPEED = 0.075F;

void player_move(player_t* player, const controls_t* controls) {
  float dir_x = player->dir_x;
  float dir_y = player->dir_y;

  if (controls->m_forward) {
    player->x += dir_x * MOVE_SPEED;
    player->y += dir_y * MOVE_SPEED;
  }
  if (controls->m_backward) {
    player->x -= dir_x * MOVE_SPEED;
    player->y -= dir_y * MOVE_SPEED;
  }

  if (controls->m_left) {
    player->x += dir_y * MOVE_SPEED;
    player->y -= dir_x * MOVE_SPEED;
  }
  if (controls->m_right) {
    player->x -= dir_y * MOVE_SPEED;
    player->y += dir_x * MOVE_SPEED;
  }

  if (controls->r_left) {
    player->dir_x = dir_x * cosf(ROT_SPEED) + dir_y * sinf(ROT_SPEED);
    player->dir_y = -dir_x * sinf(ROT_SPEED) + dir_y * cosf(ROT_SPEED);
  }
  if (controls->r_right) {
    player->dir_x = dir_x * cosf(ROT_SPEED) - dir_y * sinf(ROT_SPEED);
    player->dir_y = dir_x * sinf(ROT_SPEED) + dir_y * cosf(ROT_SPEED);
  }
}
