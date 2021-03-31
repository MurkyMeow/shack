#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include "map.h"
#include "player.h"

float get_wall_distance(const map_t* map, const player_t* player, float angle);

#endif
