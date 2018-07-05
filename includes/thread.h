#ifndef _THREAD_H_
#define _THREAD_H_

#include "request.h"
#include "base_map.h"

typedef	struct	s_thread_cl
{
  t_data	*data;
  int		socket;
}		t_thread_cl;

void *thread_listen_serv(void *s_info);
void *server_thread(void *s_info);
void *threaded_ticker(void *tick);
void *threaded_main_loop(void *server);

#endif /* !_THREAD_H_ */
