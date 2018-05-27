#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

int main ()
{
  int		quit = 0;
  SDL_Window	*window;
  SDL_Event	event;
  SDL_Renderer	*renderer;
  SDL_Surface	*sprites_img;
  SDL_Texture	*texture, *sprite_texture;
  int		i;
  int		j;
  
  j = 0;

  //init sdl
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);
  
  //init the window
  window = SDL_CreateWindow(
			    "Bomberman",
			    SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    1024,
			    768,
			    SDL_WINDOW_SHOWN
			    );  
  if (!window) {
    printf("windows not created");
    return 1;
  }
  
  //where the image will be rendered
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer){
    SDL_ShowSimpleMessageBox(0, "renderer init error", SDL_GetError(), window);
  }
  
  sprites_img = IMG_Load("bombermanSprite.PNG");
  if (!sprites_img){
    SDL_ShowSimpleMessageBox(0, "img init error", SDL_GetError(), window);
  }
  
  sprite_texture = SDL_CreateTextureFromSurface(renderer, sprites_img);
  
 if (!sprite_texture){
    SDL_ShowSimpleMessageBox(0, "texture image init error", SDL_GetError(), window);
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			      SDL_TEXTUREACCESS_TARGET, 1024, 768);
  if (!texture){
    SDL_ShowSimpleMessageBox(0, "texture init error", SDL_GetError(), window);
  }

  if (SDL_SetRenderTarget(renderer, texture) < 0) {
    SDL_ShowSimpleMessageBox(0, "setting texture in renderer error",
			     SDL_GetError(), window);
  }
  
  SDL_Rect wall_src_rect = {71, 175, 16, 16};
  //  SDL_Rect ground_src_rect = {122, 175, 16, 16};
  for (i = 0; i < 15; i++) { 
    SDL_Rect dest_rect = {i * 48, j * 48 , 48, 48};
    if (SDL_RenderCopy(renderer, sprite_texture , &wall_src_rect, &dest_rect) < 0)
      {
	SDL_ShowSimpleMessageBox(0, "adding texture in renderer error",
				 SDL_GetError(), window);
      }
    
    if (i == 14 && j == 0) {
      i = -1;
      j = 12;
    }
  }
  
  /* 
  ** important, use this to apply modification on last textures
  ** placed on the renderer
  */
  
  SDL_SetRenderTarget(renderer, NULL);

  SDL_RenderPresent(renderer);
  while(!quit) {
    SDL_WaitEvent(&event);

    switch(event.type)
      {
      case SDL_QUIT:
	quit = 1;
	break;
      }
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(sprites_img);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
