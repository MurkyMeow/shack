#ifndef _SPRITE_H_
#define _SPRITE_H_

typedef struct {
  float x;
  float y;
  int texid;
} sprite_t;

typedef struct {
  sprite_t* original;
  int is_rendered;
  int screen_stripe;
  float distance;
} map_sprite_t;

#endif
