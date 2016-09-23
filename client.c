#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

#include "types.h"
#include "comm.h"


int main(int argc, char *argv[]) {

    Data * data;
    Data * serverResponse;
    Connection * connection;
    char * address = "/tmp/listener";

    //Simulating client request and Data
    data = malloc(sizeof(Data));

    data->size = sizeof(Data);
    data->opcode = 666;
    data->client_pid = getpid();

    /*        */printf("[client] connecting to server on address %s\n", address);

    connection = comm_connect(address);

    /*        */printf("[client] connection established. sending data with opcode 666\n");

    sendData(connection, data);

    /*        */printf("[client] data sent. awaiting response\n");

    serverResponse = receiveData(connection);

    /*        */printf("[client] data received. opcode: %d\n", serverResponse->opcode);

    /*        */printf("[client] done\n");

}