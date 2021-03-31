#include "player.h"

#include <math.h>
#include <stdlib.h>

const float MOVE_SPEED = 0.1F;
const float SENSETIVITY = 0.075F;

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
