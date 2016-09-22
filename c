rm server client
gcc -o server server.c commsockets.c marshalling.c
gcc -o client client.c commsockets.c marshalling.c
