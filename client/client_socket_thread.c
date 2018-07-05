/*
** client_socket_thread.c for  in /home/notwak42/Projects/C/Bomberman/BombGit/Bomberman/client
** 
** Made by MASERA Mathieu
** Login   <masera_m@etna-alternance.net>
** 
** Started on  Wed Jul  4 00:13:33 2018 MASERA Mathieu
** Last update Thu Jul  5 19:57:12 2018 MASERA Mathieu
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

void		*thread_listen_serv(void *struct_listener_c)
{
  int		iterator;
  int		quit = 0;
  int		socket;
  fd_set	fd_read;
  t_game_info	*game_info;
  t_thread_cl	*thread_cl = (t_thread_cl*)struct_listener_c;

  socket = thread_cl->socket;
  printf("OK %d", socket);
  while (!quit)
    {
      FD_ZERO(&fd_read);
      FD_SET(socket, &fd_read);
      
      if (select((socket + 1), &fd_read, NULL, NULL, NULL) == -1)
	quit = 1;
      
      if (FD_ISSET(socket, &fd_read))
        {
	  if (get_message(socket) == 0)
            {
	      quit = 1;
            }
	  game_info = get_game_info();
	  printf("checksum %d\n", game_info->checksum);
	  printf("tick_time %d\n", game_info->tick_time);
	  printf("game_status %d\n", game_info->game_status);
	  printf("id_client %d\n", game_info->id_client);
	  printf("file descriptor %d\n", game_info->players[0].fd);
	  printf("nb_client %d\n", game_info->nb_client);
	  printf("connected %d\n", game_info->players[0].connected);

	  for (iterator = 0; iterator < 4; iterator++)
	    {
	      
	      if (game_info->players[iterator].connected != 0)

		{
		  printf("Im player: %d \n", game_info->id_client);
		  //printf("waaaaaaaaaaa %d num_player \n", game_info->players[0]->num_player);
		}
	    }
	   if (game_info->game_status == 0 && game_info->players != NULL) {
	     draw_players((void*)(thread_cl->data));
	    }
        }
    }
  pthread_exit(NULL);
}
