#include "socket.h"
#include "client.h"

t_client_request*       create_client_request()
{
	t_client_request*   client_request;
	unsigned int magic;

	srand(time(NULL));
	magic = (rand() % INT_MAX);
	client_request = calloc(1, sizeof(t_client_request));
	if (client_request == NULL)
		return NULL;

	client_request->magic = magic;
	client_request->checksum = get_request_checksum(client_request);

	return client_request;
}

int free_client_request(t_client_request* client_request)
{
	free(client_request);
	if (client_request == NULL)
		return SUCCESS_FREE;
	return BAD_FREE;
}
