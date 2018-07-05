/*
** client_get_game_info.c for Bomberman in /Users/kumatetsu/projet-etna/DVC4/Bomberman/Bomberman
**
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
**
** Started on  Mon Jul  2 16:17:01 2018 BILLAUD Jean
** Last update Thu Jul  5 18:59:02 2018 MASERA Mathieu
*/

#include "sdl.h"
#include "client.h"
#include "game_info.h"
#include "game_info_serialization.h"
#include "my_put.h"

int get_message(int s)
{
  char buff[sizeof(t_game_info)];
  int r;
  t_game_info *game_info;

  game_info = NULL;
  r = recv(s, buff, sizeof(t_game_info) - 1 , 0);
  if (r > 0)
  {
    printf("%d", r);
    deserialize_game_info(buff);
    game_info = get_game_info();
    printf("game_status %d \n", game_info->game_status);
    printf("tick_time %d \n", game_info->tick_time);
    printf("checksum %ld \n", (long)game_info->checksum);
//    if (game_info->players[0] != NULL){
//      printf("x_pos first_player %d x_pos \n", game_info->players[0]->x_pos);
//    }
  
    return 1;
  }
  else
    return 0;
}
