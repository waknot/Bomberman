/*
** client_socket_thread.c for  in /home/notwak42/Projects/C/Bomberman/BombGit/Bomberman/client
** 
** Made by MASERA Mathieu
** Login   <masera_m@etna-alternance.net>
** 
** Started on  Wed Jul  4 00:13:33 2018 MASERA Mathieu
** Last update Thu Jul  5 12:39:54 2018 MASERA Mathieu
*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "sdl.h"
#include "my_put.h"
#include "client.h"
#include "game_info.h"
#include "game_info_serialization.h"
#include "thread.h"
#include "base_map.h"

void *thread_listen_serv(void *struct_listener_c)
{
  int		iterator;
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
	    for (iterator = 0; iterator < 4; iterator++)
	      {
	      
		if (game_info->players[iterator].fd != 0)
		  {
		    printf("Im player: %d \n", game_info->id_client);

		  }
	      }
	    if (game_info->game_status == 0 && game_info->players != NULL) {
	      draw_players((void*)(thread_cl->data));
	    }
	  }
        }
    }
  pthread_exit(NULL);
}
