#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct {
  float x;
  float y;
  float dir_x;
  float dir_y;
} player_t;

typedef struct {
  unsigned int m_left;
  unsigned int m_right;
  unsigned int m_forward;
  unsigned int m_backward;
  unsigned int r_left;
  unsigned int r_right;
} controls_t;

void player_move(player_t* player, const controls_t* controls);

#endif
