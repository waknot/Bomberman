#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>
#include "base_map.h"

int		main ()
{
  int		quit;
  SDL_Event	event;
  t_data	*data;

  quit = 0;
  data = malloc(sizeof(t_data));
  init_window((void*)data);
  draw_all((void*)data);
  SDL_SetRenderTarget(data->renderer, NULL);
  SDL_RenderPresent(data->renderer);
  SDL_RenderClear(data->renderer);
  while(!quit)
    {
      SDL_WaitEvent(&event);
      switch(event.type)
	{
	case SDL_QUIT:
	  quit = 1;
	  break;
	case SDL_KEYDOWN:
          SDL_RenderClear(data->renderer);
	  rebuild_map((void*)data);
	  	  SDL_RenderPresent(data->renderer);

 	  SDL_SetRenderTarget(data->renderer, NULL);
	break;
	} 
    }
  SDL_DestroyTexture(data->texture);
  SDL_DestroyRenderer(data->renderer);
  SDL_DestroyWindow(data->window);
  IMG_Quit();
  SDL_Quit();
  return 0;
}

void		*init_window(void * arg) {
  SDL_Window	*window;
  SDL_Renderer	*renderer;
  SDL_Texture	*sprite_texture;
  SDL_Surface	*sprites_img;

  t_data *data = (t_data*)arg;
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);
  window = SDL_CreateWindow(
			    "Bomberman",
			    SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    WINDOW_W,
			    WINDOW_H,
			    SDL_WINDOW_SHOWN
			    );  
  if (!window)
    {
      printf("windows not created");
      return (NULL);
    }
  
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer)
    {
      SDL_ShowSimpleMessageBox(0, "renderer init error", SDL_GetError(), window);
    }
 
  sprites_img = IMG_Load("bombermanSprite.PNG");
  if (!sprites_img)
    {
      SDL_ShowSimpleMessageBox(0, "img init error", SDL_GetError(), window);
    }

  //we create the image as a texture to insert it in renderer
  sprite_texture = SDL_CreateTextureFromSurface(renderer, sprites_img);
  
  if (!sprite_texture)
    {
      SDL_ShowSimpleMessageBox(0, "texture image init error",
			       SDL_GetError(), window);
    } 
  SDL_FreeSurface(sprites_img);
  SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
  SDL_RenderClear(renderer);
  data->texture = sprite_texture;
  data->renderer = renderer;
  data->window = window;
  /* 
  ** important, use this to apply modification on last textures
  ** placed on the renderer
  */
  SDL_SetRenderTarget(data->renderer, NULL);
  return (NULL);
}


