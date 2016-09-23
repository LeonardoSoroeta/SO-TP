#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "types.h"
#include "comm.h"

void newSession(Connection *);


int main(int argc, char *argv[])
{
	int newpid;
	Listener * listener;
	Connection * connection;
	char * address = "/tmp/listener";

	listener = comm_listen(address);

	/*        */printf("[server] awaiting connection requests\n");

	while(1) {

		connection = comm_accept(listener);

		newpid = fork();

		if(newpid == 0) {

			newSession(connection);

			break;
		
		} 

	}

}

void newSession(Connection * connection) {

	Data * data;

	/*        */printf("[session %d] new client session started\n", getpid());


	data = receiveData(connection);

	/*        */printf("[session %d] data received. opcode = %d\n", getpid(), data->opcode);

	data->opcode = 999;

	/*        */printf("[session %d] sending response with opcode = 999\n", getpid());

	sendData(connection, data);

	/*        */printf("[session %d] session done\n", getpid());

}