#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>



typedef enum texture_type
  {
    wall,
    ground,
    ground_shadowed,
    firstPlayer
  } texture_type;

typedef struct s_map
{
  SDL_Rect	src_rect;
  SDL_Rect	dest_rect;
  texture_type  texture_type;
} t_map;

typedef struct s_data
{
  SDL_Texture	*texture;
  SDL_Renderer	*renderer;
  SDL_Window	*window;
  t_map		array_map[14][15];	
} t_data;

// in the picture, the size of the square is 16 for a wall block, so i use 48

const int WINDOW_W = 1024;
const int WINDOW_H = 768;
const int PIXEL_SIZE = 48;
const int I_BEGIN = ((1024 - (15 * 48)) / 2 ) / 48;
const int J_BEGIN = (768 - (13 * 48)) / 48;

void *init_window(void * arg);
void *draw_player_1(void *arg);
void *draw_map_model(void *arg);
void *draw_score(void *arg);
void *draw_timer(void *arg);
void *draw_all(void *arg);
void *rebuild_map(void *arg);
SDL_Rect init_rect(int x, int y, int w, int z);
t_map init_t_map(SDL_Rect src, SDL_Rect dest, texture_type type);

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

void *draw_all(void *arg)
{
  draw_map_model(arg);
  draw_score(arg);
  draw_timer(arg);
  draw_player_1(arg);
  return (NULL);
}


void	*draw_map_model(void *arg)
{
  int	i, j, a, b, error;
  
  t_data *data = (t_data*)arg;
  error = 0;
  a = 1;
  SDL_Rect wall_src_rect = {71, 175, 16, 16};
  SDL_Rect ground_src_rect = {122, 175, 16, 16};
  SDL_Rect ground_shadowed_rect = {105, 175, 16 ,16};
  for (j = J_BEGIN; j < J_BEGIN + 13; j++) {
      b = 0;
    for (i = I_BEGIN; i < I_BEGIN + 15; i++) {
      SDL_Rect dest_rect = {i * PIXEL_SIZE, j * PIXEL_SIZE,
			    PIXEL_SIZE, PIXEL_SIZE};
      if (j == J_BEGIN || j == J_BEGIN + 12 ||
	  i == I_BEGIN || i == I_BEGIN + 14)
	{
	  error = SDL_RenderCopy(data->renderer, data->texture ,
				 &wall_src_rect, &dest_rect);
	  data->array_map[a][b] = init_t_map(wall_src_rect, dest_rect, wall);
	}
      else if (j == J_BEGIN + 1 ||
	       ( j % 2 != J_BEGIN % 2 && i % 2 == I_BEGIN % 2))
	{
	  error = SDL_RenderCopy(data->renderer, data->texture,
				 &ground_shadowed_rect, &dest_rect);
	  data->array_map[a][b] =
	    init_t_map(ground_shadowed_rect, dest_rect, ground_shadowed);
	}
      else if (i % 2 != I_BEGIN % 2)
	{
	  error = SDL_RenderCopy(data->renderer, data->texture,
				 &ground_src_rect, &dest_rect);
	  	  data->array_map[a][b] =
		    init_t_map(ground_src_rect, dest_rect, ground);
	}
      else if (i % 2 == I_BEGIN % 2)
	{
	  error = SDL_RenderCopy(data->renderer, data->texture,
				 &wall_src_rect, &dest_rect);
	   data->array_map[a][b] =
		    init_t_map(wall_src_rect, dest_rect, wall);
	}
      if (error < 0)
	{
	  SDL_ShowSimpleMessageBox(0, "adding texture in renderer error",
				   SDL_GetError(), data->window);
	  break;
	}
      b++;
    }
    if (error < 0)
      break;
    a++;
  }
  if (error < 0)
    return (NULL);
  return ((void*)data);
}


void	*draw_score(void *arg)
{
  int	error;

  error = 0;
  t_data *data = (t_data*) arg;
  SDL_Rect score_panel_rect = {414, 175, 256, 32};
  SDL_Rect dest_rect_score = {0, 0, WINDOW_W , J_BEGIN * PIXEL_SIZE};
  error = SDL_RenderCopy(data->renderer, data->texture,
			 &score_panel_rect, &dest_rect_score);
  if (error < 0)
    SDL_ShowSimpleMessageBox(0, "drawing Score Tab Failed",
			     SDL_GetError(), data->window);
  return (NULL);
}

void	*draw_timer(void *arg)
{
  int	error;

  error = 0;
  t_data *data = (t_data*) arg;
  SDL_Rect timer_panel_rect = {413, 37, 32, 14};
  SDL_Rect dest_rect_score = {0, 0, WINDOW_W , J_BEGIN * PIXEL_SIZE};
  SDL_Rect dest_rect_timer = {(WINDOW_W / 2) - ((timer_panel_rect.w / 2) * 5),
			      (dest_rect_score.h / 2) -
			      ((timer_panel_rect.h / 2) * 5),
			      timer_panel_rect.w * 5,
			      timer_panel_rect.h * 5};
  error = SDL_RenderCopy(data->renderer, data->texture,
			 &timer_panel_rect, &dest_rect_timer);
  if (error < 0)
    SDL_ShowSimpleMessageBox(0, "drawing Timer Failed",
			     SDL_GetError(), data->window);
  return (NULL);
}

void	*draw_player_1(void *arg) {
  int	error;

  error = 0;
  t_data *data = (t_data*)arg;
  SDL_Rect bomberman_white = {70, 38, 16, 24};
  SDL_Rect dest_rect = {(I_BEGIN + 1) * 48, ((J_BEGIN + 1) * 48) - 24 ,
			16 * 3, 24 * 3};
  error = SDL_RenderCopy(data->renderer, data->texture,
			 &bomberman_white, &dest_rect);
  if (error < 0)
    SDL_ShowSimpleMessageBox(0, "drawing Player1 Failed",
			     SDL_GetError(), data->window);
  return (NULL);
}

SDL_Rect init_rect(int x, int y, int w, int z) {
  SDL_Rect temp = {x, y, w, z};
  return (temp);
}

t_map init_t_map(SDL_Rect src, SDL_Rect dest, texture_type type)
{
  t_map map;
  map.src_rect = src;
  map.dest_rect = dest;
  map.texture_type = type;
  return (map);
}

void *rebuild_map(void *arg) {
  t_data *data = (t_data*)arg;
  int i, j;

  for (i = 1; i < 14; i++)
    {
      for (j = 0; j < 15; j++)
	{
	  SDL_RenderCopy(data->renderer, data->texture,
			 &(data->array_map[i][j].src_rect),
			 &(data->array_map[i][j].dest_rect));
	}
    }
  return (NULL);
}
