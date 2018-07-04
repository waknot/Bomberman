/*
** start_map.c for  in /home/notwak42/Projects/C/Bomberman/BombGit/Bomberman
** 
** Made by MASERA Mathieu
** Login   <masera_m@etna-alternance.net>
** 
** Started on  Wed Jul  4 09:30:11 2018 MASERA Mathieu
** Last update Wed Jul  4 19:13:46 2018 MASERA Mathieu
*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>
#include "sdl.h"
#include "client.h"
#include "player_info.h"
#include "player.h"
#include "base_map.h"
#include "game_info.h"
#include "thread.h"
#include "game_info_serialization.h"


int start_map(t_sdl *sdl, int socket, t_player_request *cr)
{
  int quit;
  SDL_Event event;
  t_data *data;
  pthread_t listen_server;
  t_thread_cl *thread_cl_struct;
  t_game_info *game_info;
  
  quit = 0;
  data = malloc(sizeof(t_data));
  thread_cl_struct = malloc(sizeof(t_thread_cl));
  game_info = malloc(sizeof(t_game_info));
  data->renderer = sdl->renderer;
  data->window = sdl->window;
  init_sprites_sheet((void *)data);
  draw_all((void *)data);
  thread_cl_struct->data = data;
  thread_cl_struct->socket = socket;
  SDL_SetRenderTarget(data->renderer, NULL);
  SDL_RenderPresent(data->renderer);
  SDL_RenderClear(data->renderer);
  if (pthread_create(&listen_server, NULL, thread_listen_serv, (void*)thread_cl_struct))
    {
      quit = 1;
    }
  while (!quit)
    {
      game_info = get_game_info();
      if (game_info != NULL && game_info->game_status > 0) {
	while (SDL_PollEvent(&event))
	  {
	    switch (event.type)
	    {
	    case SDL_QUIT:
	      quit = 1;
	      break;
	    case SDL_KEYUP:
	      {
		SDL_RenderClear(data->renderer);
		rebuild_map((void *)data);
		move_player_stop((void *)data);
		SDL_RenderPresent(data->renderer);
		SDL_SetRenderTarget(data->renderer, NULL);
		break;
	      }
	    case SDL_KEYDOWN:
	      switch (event.key.keysym.sym)
		{
		case SDLK_UP:
		  SDL_RenderClear(data->renderer);
		  rebuild_map((void *)data);
		  move_player_up((void *)data);
		  send_request(socket, cr);
		  break;
		case SDLK_LEFT:
		  SDL_RenderClear(data->renderer);
		  rebuild_map((void *)data);
		  move_player_left((void *)data);
		  send_request(socket, cr);
		  break;
		case SDLK_RIGHT:
		  SDL_RenderClear(data->renderer);
		  rebuild_map((void *)data);
		  move_player_right((void *)data);
		  send_request(socket, cr);
		  break;
		case SDLK_DOWN:
		  SDL_RenderClear(data->renderer);
		  rebuild_map((void *)data);
		  move_player_down((void *)data);
		  send_request(socket, cr);
		  break;
		}
	      SDL_RenderPresent(data->renderer);
	      SDL_SetRenderTarget(data->renderer, NULL);
	    }
	  }
      }
    }
  pthread_cancel(listen_server);
  SDL_DestroyTexture(data->texture);
  free(data);
  free(thread_cl_struct);
  return 0;
}

void *init_sprites_sheet(void *arg)
{
  SDL_Texture *sprite_texture;
  SDL_Surface *sprites_img;

  t_data *data = (t_data *)arg;
  sprites_img = NULL;
  sprite_texture = NULL;
  IMG_Init(IMG_INIT_PNG);

  sprites_img = IMG_Load("./ressources/bombermanSprite.PNG");
  if (!sprites_img)
  {
    SDL_ShowSimpleMessageBox(0, "img init error", SDL_GetError(),
                             data->window);
  }

  //we create the image as a texture to insert it in renderer
  sprite_texture = SDL_CreateTextureFromSurface(data->renderer,
                                                sprites_img);

  if (!sprite_texture)
  {
    SDL_ShowSimpleMessageBox(0, "texture image init error",
                             SDL_GetError(), data->window);
  }
  SDL_FreeSurface(sprites_img);
  SDL_SetRenderDrawColor(data->renderer, 90, 90, 90, 255);
  SDL_RenderClear(data->renderer);
  data->texture = sprite_texture;
  /*
  ** important, use this to apply modification on last textures
  ** placed on the renderer
  */
  SDL_SetRenderTarget(data->renderer, NULL);
  return (NULL);
}
