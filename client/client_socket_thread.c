/*
** client_socket_thread.c for  in /home/notwak42/Projects/C/Bomberman/BombGit/Bomberman/client
** 
** Made by MASERA Mathieu
** Login   <masera_m@etna-alternance.net>
** 
** Started on  Wed Jul  4 00:13:33 2018 MASERA Mathieu
** Last update Wed Jul  4 19:50:06 2018 MASERA Mathieu
*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "sdl.h"
#include "client.h"
#include "game_info.h"
#include "game_info_serialization.h"
#include "thread.h"
#include "base_map.h"

void *thread_listen_serv(void *struct_listener_c)
{
  int		quit = 0;
  fd_set	fd_read;
  t_game_info	*game_info;
  t_thread_cl	*thread_cl = (t_thread_cl*)struct_listener_c;

  game_info = malloc(sizeof(t_game_info));
  printf("OK %d", thread_cl->socket);
  while (!quit)
    {
      FD_ZERO(&fd_read);
      FD_SET(thread_cl->socket, &fd_read);

      if (select((thread_cl->socket + 1), &fd_read, NULL, NULL, NULL) == -1)
	quit = 1;

      if (FD_ISSET(thread_cl->socket, &fd_read))
        {
	  if (get_message(thread_cl->socket) == 0)
            {
	      quit = 1;
            } else {
	  game_info = get_game_info();
	  if (game_info->game_status == 0 && game_info->players != NULL) {
	    draw_players((void*)(thread_cl->data));
	  }
	  if (game_info != NULL && game_info->players != NULL)
	    if (game_info->players[0] != NULL){
	      printf("waaaaaaaaaaa %d num_player \n", game_info->players[0]->num_player);
	    }
	  }
	}
    }
  pthread_exit(NULL);
}
