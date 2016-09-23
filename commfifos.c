#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "types.h"
#include "comm.h"


typedef struct {

    int requester_pid;

} ConnectionRequest;


Connection * comm_connect(Address * address) {

    int listener_fd;
    char client_incoming[32];
    char client_outgoing[32];
    ConnectionRequest connectionRequest;
    Connection * connection;

    connection = malloc(sizeof(Connection));

    connectionRequest.requester_pid = getpid();

    sprintf(client_incoming, "/tmp/client_%d_incoming", connectionRequest.requester_pid);
    sprintf(client_outgoing, "/tmp/client_%d_outgoing", connectionRequest.requester_pid);

    listener_fd = open(address->filename, O_WRONLY);

    write(listener_fd, &connectionRequest, sizeof(ConnectionRequest));

    close(listener_fd);

    raise(SIGSTOP);

    connection->incoming_fd = open(client_incoming, O_RDONLY);
    connection->outgoing_fd = open(client_outgoing, O_WRONLY);

    return connection;

}

Listener * comm_listen(Address * address) {

    Listener * listener;

    listener = malloc(sizeof(Listener));

    mknod(address->filename, S_IFIFO | 0666, 0);

    listener->listener_fd = open(address->filename, O_RDONLY);

    return listener;
}

Connection * comm_accept(Listener * listener) {

    char client_incoming[32];
    char client_outgoing[32];
    ConnectionRequest connectionRequest;
    Connection * connection;

    connection = malloc(sizeof(Connection));

    read(listener, &connectionRequest, sizeof(ConnectionRequest));

    sprintf(client_incoming, "/tmp/client_%d_incoming", connectionRequest.requester_pid);
    sprintf(client_outgoing, "/tmp/client_%d_outgoing", connectionRequest.requester_pid);

    mknod(client_incoming, S_IFIFO | 0666, 0);
    mknod(client_outgoing, S_IFIFO | 0666, 0);

    // connection ready -> signal requester
    kill(connectionRequest.requester_pid, SIGCONT);

    connection->outgoing_fd = open(client_incoming, O_WRONLY);
    connection->incoming_fd = open(client_outgoing, O_RDONLY);

    return connection;

}

int comm_write(Connection * connection, char * dataToWrite, int size) {

    return write(connection->outgoing_fd, dataToWrite, size);

}

int comm_read(Connection * connection, char * dataToRead, int size) {

    return read(connection->incoming_fd, dataToRead, size);

}

int comm_disconnect(Connection * connection) {

    close(connection->incoming_fd);
    close(connection->outgoing_fd);

    return 0;

}