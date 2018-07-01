/*
** thread.c for Project-Master in /home/enach/CLionProjects/Bomberman/bm_test/server
**
** Made by hochar_n
** Login   <hochar_n@etna-alternance.net>
**
** Started on  Sun Jul  1 17:46:08 2018 hochar_n
** Last update Sun Jul  1 23:30:17 2018 hochar_n
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "my_put.h"
#include "server.h"
#include "thread.h"
#include "game_info.h"
#include "game_info_serialization.h"

void			my_sleep(int sec, int milli)
{
  int			nano;
  struct timespec	req = {0};

  nano = milli * 1000000;
  req.tv_sec = sec;
  req.tv_nsec = nano;
  nanosleep(&req, NULL);
}

void	*threaded_ticker(void *server)
{
  char	log[50];
  t_srv **srv;
  int	*tk;
  int   socket;
  char	*serialized_game_info;

  srv = (t_srv**)server;
  tk = (*srv)->tick;
  my_putstr("\nthreaded tick begin!\n");
  while (1)
    {
      sprintf(log, "\nTick: %d", (*tk));
      my_putstr(log);
      sprintf(log, "\n number of clients: %d\n", (*srv)->n_clients);
      my_putstr(log);
      process_requests(srv);
      my_sleep(0, 5000);
      if ((*srv)->game_info == NULL)
        continue;
      for (int i = 0; i < (*srv)->n_clients; i++)
      {
        socket = (*srv)->clients[i]->fd;
        serialized_game_info = serialize_game_info((*srv)->game_info);
        write(socket, serialized_game_info, 1024);
      }
      ++(*tk);
    }
}

void process_requests(t_srv **server)
{
  int i;

  for (i = 0; i < 8; ++i)
    {
      if ((*server)->requests[i] == NULL)
	continue;
      if ((*server)->requests[i]->command == START_GAME)
	{
	  if ((*server)->n_clients >= 2 && (*server)->n_clients <= 4)
	    {
	      create_game_info(server);
	      my_putstr("\n creation of game requested");
	    }
	}
      else if ((*server)->game_info == NULL)
	continue;
      handle_requests((*server)->game_info, (*server)->requests[i]);
    }
    else if ((*server)->game_info == NULL)
    {
      free((*server)->requests[i]);
      continue;
    }
    handle_requests((*server)->game_info, (*server)->requests[i]);
    free((*server)->requests[i]);
  }
}