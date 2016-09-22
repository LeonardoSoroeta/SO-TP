#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

typedef struct {

	int incoming_fd, outgoing_fd;

} Connection;

typedef struct {

	char * filename;

} Address;

typedef struct {

	int listener_fd;

} Listener;

Connection * comm_connect(Address * address);

Listener * comm_listen(Address * address);

Connection * comm_accept(Listener * listener);

int comm_write(Connection * connection, char * dataToWrite, int size);

int comm_read(Connection * connection, char * dataToRead, int size);

int comm_disconnect(Connection * connection);

#endif
