#include <stdio.h>
#include "enum.h"
#include "constant.h"
// pour game_info.h
#include "sdl.h" // pour player_info.h et collision.h
//
#include "player_info.h"
#include "map.h"
#include "client_request.h" // pour server.h
#include "server.h"
//
#include "game_info.h"
#include "static_wall_rect.h"
#include "collision.h"

int		has_collision_with_wall(const SDL_Rect zone)
{
  int		i;
  SDL_Rect      *walls;

  walls = get_walls();
  for (i = 0; i < 82; i++)
    {
      if (SDL_HasIntersection(&zone, &walls[i]))
	return 1;
    }
  return 0;
}

int		has_collision_with_destructible(const SDL_Rect zone)
{
  int		i;
  t_game_info   *game_info;

  game_info = get_game_info();
  for (i = 0; i < INLINE_MATRIX; ++i)
    {
      if (!game_info->map_destroyable[i].exist && !game_info->map_destroyable[i].bomb)
	continue;
      const SDL_Rect element = {game_info->map_destroyable[i].x, game_info->map_destroyable[i].y, PIXEL_SIZE, PIXEL_SIZE};
      if (SDL_HasIntersection(&zone, &element))
	return 1;
    }
  return 0;
}

// retourne -1 si pas de collision, le num_player sinon
int		has_collision_with_player(const SDL_Rect zone, int num_player)
{
  int		y;
  t_game_info   *game_info;

  game_info = get_game_info();
  // Loop over players
  for (y = 0; y < (int)(sizeof(game_info->players)/sizeof(game_info->players[0])); ++y)
    {
      // si on a pas passé -1 comme num_player, on ne check pas avec le player ayant sa propre id
      if ((-1 == num_player || game_info->players[y].num_player != num_player) && &game_info->players[y] != NULL)
	{
	  //printf("\nin players stuff\n");
	  const SDL_Rect other_player = {game_info->players[y].x, game_info->players[y].y, PIXEL_SIZE, PIXEL_SIZE};
	  if (SDL_HasIntersection(&zone, &other_player))
	    return game_info->players[y].num_player;
	}
    }
  return -1;
}
