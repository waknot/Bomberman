/*
** base_map_manager.c for  in /home/notwak42/Projects/C/Bomberman/BombGit/Bomberman/base_map
** 
** Made by MASERA Mathieu
** Login   <masera_m@etna-alternance.net>
** 
** Started on  Wed Jul  4 09:30:34 2018 MASERA Mathieu
** Last update Thu Jul  5 19:48:06 2018 MASERA Mathieu
*/


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>
#include "request.h"
#include "player.h"
#include "sdl.h"
#include "base_map.h"
#include "game_info.h"

void *draw_all(void *arg)
{
  draw_map_model(arg);
  draw_pannel(arg);
  draw_timer(arg);
  white_bomber_sprite(arg);
  red_bomber_sprite(arg);
  black_bomber_sprite(arg);
  blue_bomber_sprite(arg);
  
  //draw_player_1(arg);
  //draw_player_2(arg);
  //draw_player_3(arg);
  //draw_player_4(arg);

  return (NULL);
}

void *draw_players(void *arg) {
  t_game_info *game_info;
  t_data *data = (t_data*)arg;
  
  game_info = get_game_info();
  SDL_RenderClear(data->renderer);
  rebuild_map(arg);
  printf("yatota");
  if (game_info != NULL && game_info->game_status == 0) {
    printf("hahahahh");
    if (game_info->nb_client > 0) {
      draw_player_1(arg);
      printf("hohohohoho");
    }
    if (game_info->nb_client > 1) {
      draw_player_2(arg);
      printf("huhuhuhuu");
    }
    if (game_info->nb_client > 2) {
      draw_player_3(arg);
      printf("hehehehehe");
    }
    if (game_info->nb_client > 3) {
      draw_player_4(arg);
      printf("opapapap");
    }
   SDL_RenderPresent(data->renderer);
   SDL_SetRenderTarget(data->renderer, NULL);
  }
  return (NULL);
}

void *rebuild_map(void *arg) {
  t_data *data = (t_data*)arg;
  int i, j;

  for (i = 0; i < 14; i++)
    {
      for (j = 0; j < 15; j++)
	{
	  if (data->renderer != NULL)
	    {
	      SDL_RenderCopy(data->renderer, data->texture,
			     &(data->array_map[i][j].src_rect),
			     &(data->array_map[i][j].dest_rect));
	    }
	}
    }
  return (NULL);
}
