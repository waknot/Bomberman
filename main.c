/*
** main.c for Bomberman in /Users/kumatetsu/projet-etna/DVC4/Bomberman/Bomberman
**
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
**
** Started on  Tue Jun 26 17:26:19 2018 BILLAUD Jean
** Last update Mon Jul  2 18:46:25 2018 BILLAUD Jean
*/

#include <stdlib.h>
#include <stdio.h>
#include "enum.h"
#include "sdl.h"
#include "map.h"
#include "player_info.h"
#include "client_request.h"
#include "server.h"
#include "menu.h"
#include "player.h"
#include "base_map_manager.h"

int main ()
{
  t_sdl *sdl;
  //init sdl
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_JPG);
  TTF_Init();

  sdl = init_sdl();
  sdl = init_window(sdl);
  sdl = init_fronts(sdl);

  //comment maiemacs n to see start_map working ;)
  main_menu(sdl);
  SDL_DestroyTexture(sdl->menu_background);
  SDL_DestroyTexture(sdl->white_back);
  SDL_DestroyTexture(sdl->join_game);
  SDL_DestroyTexture(sdl->create_game);
  SDL_DestroyTexture(sdl->server_welcome);
  SDL_DestroyRenderer(sdl->renderer);
  SDL_DestroyWindow(sdl->window);
  free(sdl);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  return 0;
}
