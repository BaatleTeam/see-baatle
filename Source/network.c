#include "network.h"

void* context = NULL;
void* reqSocket = NULL;

void initNetowrkContext() {
    if (context != NULL || context != NULL)
        return;
    context = zmq_ctx_new();
    reqSocket = zmq_socket (context, ZMQ_REQ);
    zmq_connect (reqSocket, "tcp://localhost:5555");
}

void closeNetwork() {
    zmq_close (reqSocket);
    zmq_ctx_destroy (context);
}


char* checkServerConnection(char* msg) {
    enum {cap = 32};
    zmq_send (reqSocket, msg, strlen(msg), 0);

    char buffer [cap];
    int size = zmq_recv (reqSocket, buffer, 31, 0);
    if (size == -1)
        return NULL;
    buffer[size < cap ? size : cap - 1] = '\0';

    char* returnedString = calloc(size+1, sizeof(char));
    strncpy(returnedString, buffer, size);
    returnedString[size] = '\0';
    return returnedString;
}